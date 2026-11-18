
/*++

Copyright (c) 1990-1992  Microsoft Corporation

Module Name:

    xga.c

Abstract:

    This module contains the code that implements the XGA miniport driver.

    MCA-only build: ISA/EISA (POS-emulation / banked framebuffer) support
    has been removed. This driver targets genuine IBM Micro Channel
    XGA / XGA-2 adapters only, and allows multiple MCA XGA adapters -
    including mixed XGA-1/XGA-2 configurations (e.g. an onboard 512K
    XGA-1 plus an add-in XGA-2 card, as found on a model 8590) - to be
    enumerated. Each adapter's board type and installed VRAM size is
    recorded in a system-wide inventory as it is discovered, so that the
    set of modes offered on every adapter in the machine is constrained
    to what the weakest adapter present can also support, matching the
    behavior of IBM's MXGA driver for OS/2.

    Dual-card (side-by-side extended desktop) support: if exactly two
    MCA XGA/XGA-2 adapters are found, they are merged into a SINGLE
    device extension / \Device\VideoN instance (card 0 = left/primary
    monitor, card 1 = right/secondary monitor) instead of being bound as
    two independent devices. This is required because NT4's GDI/DDI
    model only ever binds one display driver instance to one miniport
    device - xgadll has no way to be handed a second miniport handle by
    the OS. With both cards merged under one device, xgadll (which does
    own that one handle) can address either card's coprocessor/VRAM via
    the CardIndex field of VIDEO_XGA_CARD_REQUEST on the existing
    IOCTL_VIDEO_XGA_MAP_COPROCESSOR / IOCTL_VIDEO_MAP_VIDEO_MEMORY calls,
    and build a combined, extended desktop surface in its own address
    space. A third or subsequent MCA XGA adapter, if present, is ignored
    for dual-card merging purposes (only the first two found are used).

    If only one MCA XGA/XGA-2 adapter is present, this driver behaves
    exactly as the original single-card driver - none of the Card2
    logic is exercised.

Environment:

    Kernel mode

Revision History:

--*/

#include "dderror.h"
#include "devioctl.h"
#include "miniport.h"

#include "ntddvdeo.h"
#include "video.h"
#include "xga.h"
#include "xgaioctl.h"

#include "xgaloger.h"


//
// Function Prototypes
//
// Functions that start with 'Xga' are entry points for the OS port driver.
//

VP_STATUS
XgaFindAdapter(
    PVOID HwDeviceExtension,
    PVOID HwContext,
    PWSTR ArgumentString,
    PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    PUCHAR Again
    );

BOOLEAN
XgaInitialize(
    PVOID HwDeviceExtension
    );

BOOLEAN
XgaStartIO(
    PVOID HwDeviceExtension,
    PVIDEO_REQUEST_PACKET RequestPacket
    );

BOOLEAN
XgaResetHW(
    IN PVOID HwDeviceExtension,
    IN ULONG Columns,
    IN ULONG Rows
    );

//
// Define device driver procedure prototypes.
//

VP_STATUS
XgaGetPosData(
    PVOID HwDeviceExtension,
    PVOID Context,
    VIDEO_DEVICE_DATA_TYPE DeviceDataType,
    PVOID Identifier,
    ULONG IdentifierLength,
    PVOID ConfigurationData,
    ULONG ConfigurationDataLength,
    PVOID ComponentInformation,
    ULONG ComponentInformationLength
    );

VP_STATUS
XgaSetMode(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG ModeNum
    );

VOID
XgaSetModeOnCard(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG ModeNum,
    UCHAR BoardType,
    ULONG IoRegBaseAddress
    );

VP_STATUS
XgaGetBankSelectCode(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    PVIDEO_BANK_SELECT BankSelect,
    ULONG BankSelectSize,
    PULONG OutputSize
    );

VP_STATUS
XgaGetRegistryParamaterCallback(
    PVOID HwDeviceExtension,
    PVOID Context,
    PWSTR ValueName,
    PVOID ValueData,
    ULONG ValueLength
    );

VOID
XgaResetHWCard(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG IoRegBaseAddress,
    PVOID PassThroughPort
    );

#if defined(ALLOC_PRAGMA)
#pragma alloc_text(PAGE,DriverEntry)
#pragma alloc_text(PAGE,XgaFindAdapter)
#pragma alloc_text(PAGE,XgaInitialize)
#pragma alloc_text(PAGE,XgaStartIO)
#pragma alloc_text(PAGE,XgaGetPosData)
#pragma alloc_text(PAGE,XgaSetMode)
#pragma alloc_text(PAGE,XgaSetModeOnCard)
#pragma alloc_text(PAGE,XgaGetBankSelectCode)
#pragma alloc_text(PAGE,XgaGetRegistryParamaterCallback)
#endif

// XgaResetHW / XgaResetHWCard are not pageable


ULONG
DriverEntry (
    PVOID Context1,
    PVOID Context2
    )

/*++

Routine Description:

    Installable driver initialization entry point.
    This entry point is called directly by the I/O system.

Arguments:

    Context1 - First context value passed by the operating system. This is
        the value with which the miniport driver calls VideoPortInitialize().

    Context2 - Second context value passed by the operating system. This is
        the value with which the miniport driver calls VideoPortInitialize().

Return Value:

    Status from VideoPortInitialize()

--*/

{

    VIDEO_HW_INITIALIZATION_DATA hwInitData;
    ULONG initializationStatus;

    PAGED_CODE();

    //
    // Zero out structure.
    //

    VideoPortZeroMemory(&hwInitData, sizeof(VIDEO_HW_INITIALIZATION_DATA));

    //
    // Specify sizes of structure and extension.
    //

    hwInitData.HwInitDataSize = sizeof(VIDEO_HW_INITIALIZATION_DATA);

    //
    // Set entry points.
    //

    hwInitData.HwFindAdapter = XgaFindAdapter;
    hwInitData.HwInitialize  = XgaInitialize;
    hwInitData.HwInterrupt   = NULL;
    hwInitData.HwStartIO     = XgaStartIO;
    hwInitData.HwResetHw     = XgaResetHW;

    //
    // Determine the size we require for the device extension.
    //

    hwInitData.HwDeviceExtensionSize = sizeof(HW_DEVICE_EXTENSION);

    //
    // Always start with parameters for device0 in this case.
    //

//    hwInitData.StartingDeviceNumber = 0;

    //
    // This driver supports Micro Channel XGA/XGA-2 adapters only. Each
    // MCA slot that reports a valid XGA adapter ID will result in
    // XgaFindAdapter() being called again (via *Again) UNTIL two
    // adapters have been found and merged into a single device
    // extension for dual-card extended-desktop support - see
    // XgaFindAdapter() for details. A third or later adapter, if
    // present, is ignored.
    //

    hwInitData.AdapterInterfaceType = MicroChannel;

    initializationStatus = VideoPortInitialize(Context1,
                                               Context2,
                                               &hwInitData,
                                               (PVOID)hwInitData.AdapterInterfaceType);

    return initializationStatus;

} // end DriverEntry()


VOID
XgaProbeCardMemory(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG IoRegBaseAddress,
    PVOID A0000MemoryAddress,
    PULONG FrameBufferLength
    )

/*++

Routine Description:

    Probes a single XGA/XGA-2 card for its true installed VRAM size, using
    the 64K aperture mapped at A0000 for that card. This logic is
    unchanged from the original single-card probe sequence in
    XgaFindAdapter() - it has simply been factored out into its own
    routine so it can be run against either the primary card's resources
    or (when a second card is found) the secondary card's resources,
    without duplicating the sequence of register pokes inline twice.

Arguments:

    HwDeviceExtension - Supplies a pointer to the miniport's device
        extension (only used here to satisfy the XGAOUT/XGAIN macros,
        which are written in terms of a local named hwDeviceExtension -
        see below).

    IoRegBaseAddress - I/O base address of the XGA registers for the card
        being probed.

    A0000MemoryAddress - Virtual address of the 64K aperture mapped at
        A0000 for the card being probed.

    FrameBufferLength - Returns the detected VRAM size in bytes
        (0x80000, 0x100000, or 0x200000).

--*/

{

    //
    // The XGAOUT/XGAIN macros reference a local variable named
    // hwDeviceExtension and use its IoRegBaseAddress field; alias a local
    // device extension whose IoRegBaseAddress is the one we were asked to
    // probe, so these macros operate on the correct card.
    //

    HW_DEVICE_EXTENSION localExtension;
    PHW_DEVICE_EXTENSION hwDeviceExtension = &localExtension;
    volatile PUCHAR videoMemory;
    UCHAR testValue;

    hwDeviceExtension->IoRegBaseAddress = IoRegBaseAddress;

    videoMemory = A0000MemoryAddress;
    testValue = 0xA5;

    //
    // Disable interrupts, blank the palette, and switch into a temporary
    // Extended Graphics Mode so we can probe video memory through the
    // 64K aperture at A0000.
    //

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | INT_ENABLE_REG), 0x00);

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | INDEX_REG), 0x64);

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | DATA_IN_REG), 0x00);

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | OP_MODE_REG), 0x04);

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | APP_CTL_REG), 0x01);

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | APP_INDEX_REG), 0x0C);

    //
    // To test if memory is present, use a test value (make sure the same
    // value is not currently stored in video memory, otherwise change it),
    // store it in video memory, and read it back. If the value is identical
    // to the value we stored then there must be memory present at that
    // location (which, in this case, means we have 1 MEG of Video RAM).
    //

    if (*videoMemory == testValue) {

        testValue >>= 1;
    }

    *videoMemory = testValue;

    if (*videoMemory == testValue) {

        *FrameBufferLength = 0x00100000;

    } else {

        *FrameBufferLength = 0x00080000;
    }

    //
    // Now try the same at another video memory location between 1 and 2 MEG.
    //

    VideoPortWritePortUchar(
        (PUCHAR) (hwDeviceExtension->IoRegBaseAddress | APP_INDEX_REG), 0x1C);

    if (*videoMemory == testValue) {

        testValue >>= 1;
    }

    *videoMemory = testValue;

    if (*videoMemory == testValue) {

        *FrameBufferLength = 0x00200000;
    }

    VideoDebugPrint((2, "XgaProbeCardMemory\n  The amount of memory on the card is %d K\n",
                   *FrameBufferLength >> 10));

} // end XgaProbeCardMemory()


VP_STATUS
XgaFindAdapter(
    PVOID HwDeviceExtension,
    PVOID HwContext,
    PWSTR ArgumentString,
    PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    PUCHAR Again
    )

/*++

Routine Description:

    This routine is called to determine if the adapter for this driver
    is present in the system.
    If it is present, the function fills out some information describing
    the adapter.

    This build only recognizes Micro Channel (MCA) XGA/XGA-2 adapters.

    Dual-card extended desktop support: the FIRST MCA XGA/XGA-2 adapter
    found is set up exactly as in the original single-card driver, using
    the existing (unrenamed) fields of the device extension, and *Again
    is set to request a second call. On that SECOND call, rather than
    being handed a fresh, zeroed device extension as a normal multi-device
    miniport would be, we instead detect that a card has already been
    found (HwDeviceExtension->NumCards == 1) and, if a second MCA XGA
    adapter is present, fold its resources into the SAME device extension
    under the Card2-prefixed fields, then clear *Again so no third call -
    and no second \Device\VideoN instance - is created. If no second
    adapter is found, *Again is simply left at 0 and the driver proceeds
    as a normal single-card device, identical to before this change.

    A third physical MCA XGA adapter, if present, is intentionally
    ignored (XgaGetPosData/VideoPortGetDeviceData will simply not be
    called for it, because *Again is cleared once two cards have been
    merged).

    As each adapter is found, its true installed VRAM size is probed and
    recorded (along with its board type) in the system-wide
    XgaAdapterInventory[] table. XgaInitialize() later uses this inventory
    to compute the set of video modes common to every adapter present in
    the machine, so that mixed configurations (e.g. an onboard 512K XGA-1
    plus an add-in XGA-2 card) only ever offer modes the weakest adapter
    can also run - matching the behavior of IBM's MXGA driver for OS/2.

Arguments:

    HwDeviceExtension - Supplies the miniport driver's adapter storage. This
        storage is initialized to zero before this call ONLY on the very
        first call for this device; see Routine Description above for how
        the second call is distinguished from a brand new device.

    HwContext - Supplies the context value which was passed to
        VideoPortInitialize().

    ArgumentString - Suuplies a NULL terminated ASCII string. This string
        originates from the user.

    ConfigInfo - Returns the configuration information structure which is
        filled by the miniport driver. This structure is initialized with
        any knwon configuration information (such as SystemIoBusNumber) by
        the port driver. Where possible, drivers should have one set of
        defaults which do not require any supplied configuration information.

    Again - Indicates if the miniport driver wants the port driver to call
        its VIDEO_HW_FIND_ADAPTER function again with a new device extension
        and the same config info. This is used by the miniport drivers which
        can search for several adapters on a bus.

Return Value:

    This routine must return:

    NO_ERROR - Indicates a host adapter was found and the
        configuration information was successfully determined.

    ERROR_INVALID_PARAMETER - Indicates an adapter was found but there was an
        error obtaining the configuration information. If possible an error
        should be logged.

    ERROR_DEV_NOT_EXIST - Indicates no host adapter was found for the
        supplied configuration information.

--*/

{

//
// Number of entries in the access range structure passed to the port driver.
//

#define NUM_XGA_ACCESS_RANGES 6

    PHW_DEVICE_EXTENSION hwDeviceExtension = HwDeviceExtension;
    CM_MCA_POS_DATA posData;
    ULONG ioAddress;
    VP_STATUS status;
    VIDEO_ACCESS_RANGE accessRange[NUM_XGA_ACCESS_RANGES];
    PHYSICAL_ADDRESS A0000PhysicalAddress;
    PHYSICAL_ADDRESS passThroughPort;
    BOOLEAN findingSecondCard;

    UCHAR foundBoardType;
    PHYSICAL_ADDRESS foundRomBaseAddress;
    PHYSICAL_ADDRESS foundIoRegBaseAddress;
    PHYSICAL_ADDRESS foundVideoMemoryAddress;
    ULONG foundVideoMemoryLength;
    PHYSICAL_ADDRESS foundCoProcessorAddress;
    ULONG foundIoRegBaseAddressMapped;
    PVOID foundA0000MemoryAddress;
    ULONG foundFrameBufferLength;

    A0000PhysicalAddress.LowPart = 0x000A0000;
    A0000PhysicalAddress.HighPart = 0x00000000;

    passThroughPort.LowPart = 0x000003C3;
    passThroughPort.HighPart = 0x00000000;

    //
    // Indicate we do not wish to be called over, by default. This gets
    // overridden below the first time through, once we know whether we
    // should ask for a second call to look for a second card.
    //

    *Again = 0;

    //
    // Make sure the size of the structure is at least as large as what we
    // are expecting (check version of the config info structure).
    //

    if (ConfigInfo->Length < sizeof(VIDEO_PORT_CONFIG_INFO)) {

        return ERROR_INVALID_PARAMETER;

    }

    //
    // This driver only supports Micro Channel XGA/XGA-2 adapters.
    //

    if ((ULONG)HwContext != MicroChannel) {

        return ERROR_DEV_NOT_EXIST;

    }

    //
    // Determine whether this call is looking for the first card in this
    // device extension, or the second (dual-card merge) card. The port
    // driver re-uses the SAME device extension memory across repeated
    // *Again calls for a given device, so hwDeviceExtension->NumCards
    // reliably tells us which pass we are on.
    //

    findingSecondCard = (hwDeviceExtension->NumCards == 1);

    //
    // Reset the Slot number being examined to zero if we are processing
    // a new bus number. Note this intentionally does NOT reset XgaSlot
    // when looking for the second card on the same bus, since we must
    // continue scanning forward from where the first card was found
    // rather than re-finding the same slot.
    //

    if (BusNumber != (LONG)ConfigInfo->SystemIoBusNumber) {

        BusNumber = ConfigInfo->SystemIoBusNumber;

        if (!findingSecondCard) {

            XgaSlot = 0;
        }
    }

    //
    // Use the POS information from the bus to identify an XGA/XGA-2
    // adapter in the current slot. XgaGetPosData() advances XgaSlot as it
    // scans, so calling it again here (whether this is the first or
    // second card being looked for) picks up scanning from the next
    // un-examined slot.
    //

    if (NO_ERROR != VideoPortGetDeviceData(hwDeviceExtension,
                                           VpBusData,
                                           XgaGetPosData,
                                           &posData)) {

        VideoDebugPrint((2, "Xga: GetDeviceData returned error - probably no (more) XGAs\n"));

        //
        // If we already have one card successfully configured and simply
        // did not find a second one, that is not an error - we just
        // proceed as a single-card device. *Again is already 0 from the
        // top of this routine in that case.
        //

        if (findingSecondCard) {

            return NO_ERROR;
        }

        return ERROR_DEV_NOT_EXIST;
    }

    //
    // Determine the XGA registers by munging the POS register data. This
    // computes the resource set for the card that was JUST found (which
    // may be the first or second physical card) into local variables;
    // these get committed into either the primary or Card2-prefixed
    // fields of the device extension further down, once we know this
    // card's resources do not conflict with anything already claimed.
    //

    ioAddress = (posData.PosData1 & 0x0E) >> 1;

    foundRomBaseAddress.LowPart =
        (((posData.PosData1 & 0xF0) >> 4) * 0x2000) + 0xC0000;
    foundRomBaseAddress.HighPart = 0x00000000;

    foundIoRegBaseAddress.LowPart = 0x2100 + (ioAddress << 4);
    foundIoRegBaseAddress.HighPart = 0x00000000;

    //
    // Find the physical address of the frame buffer. Get the 4Meg aperture
    // if it is available, otherwise take the 1MEG.
    //

    if (posData.PosData3 & 0x01) {

        VideoDebugPrint((1, "Xga: using the 4 MEG Aperture\n"));

        foundFrameBufferLength = 0x00100000;
        foundVideoMemoryLength = 0x00400000;

        foundVideoMemoryAddress.HighPart = 0x00000000;
        foundVideoMemoryAddress.LowPart =
            ((posData.PosData3 & 0xFE) << 24) | (ioAddress << 22);

    } else {

        VideoDebugPrint((1, "Xga: trying the 1 MEG aperture\n"));

        foundFrameBufferLength = 0x00100000;
        foundVideoMemoryLength = 0x00100000;

        foundVideoMemoryAddress.HighPart = 0x00000000;
        foundVideoMemoryAddress.LowPart =
            (posData.PosData4 & 0x0F) << 20;
    }

    //
    // If none of these apertures are open, fail. On genuine MCA XGA/XGA-2
    // hardware a 1MEG or 4MEG linear aperture is always expected; there is
    // no ISA-style 64K banked fallback in this build.
    //

    if (!foundVideoMemoryAddress.LowPart) {

        VideoDebugPrint((1, "Xga: no linear aperture available - failing\n"));

        if (findingSecondCard) {

            //
            // Treat a bad second card exactly like "no second card found" -
            // fall back to single-card operation rather than failing the
            // whole device.
            //

            return NO_ERROR;
        }

        return ERROR_INVALID_PARAMETER;
    }

    //
    // Get the address of the Co processor registers.
    //

    foundCoProcessorAddress.LowPart = 0x80 * ioAddress
        + 0x1C00 + foundRomBaseAddress.LowPart;
    foundCoProcessorAddress.HighPart = 0x00000000;

    //
    // Save the data for access ranges. Both cards' access ranges are
    // verified together so that VideoPortVerifyAccessRanges() will catch
    // any overlap between the two cards' resources (which would indicate
    // we mis-identified something) as well as conflicts with any other
    // device in the system.
    //

    //
    // Io Ports
    //

    accessRange[0].RangeStart = foundIoRegBaseAddress;
    accessRange[0].RangeLength = XGA_IO_REGS_SIZE;
    accessRange[0].RangeInIoSpace = TRUE;
    accessRange[0].RangeVisible = TRUE;
    accessRange[0].RangeShareable = FALSE;

    //
    // Video Memory
    //

    accessRange[1].RangeStart = foundVideoMemoryAddress;
    accessRange[1].RangeLength = foundVideoMemoryLength;
    accessRange[1].RangeInIoSpace = FALSE;
    accessRange[1].RangeVisible = TRUE;
    accessRange[1].RangeShareable = FALSE;

    //
    // ROM Location
    //

    accessRange[2].RangeStart = foundRomBaseAddress;
    accessRange[2].RangeLength = XGA_ROM_SIZE;
    accessRange[2].RangeInIoSpace = FALSE;
    accessRange[2].RangeVisible = TRUE;
    accessRange[2].RangeShareable = FALSE;

    //
    // Co-Processor Location
    //

    accessRange[3].RangeStart = foundCoProcessorAddress;
    accessRange[3].RangeLength = XGA_CO_PROCESSOR_REGS_SIZE;
    accessRange[3].RangeInIoSpace = FALSE;
    accessRange[3].RangeVisible = TRUE;
    accessRange[3].RangeShareable = FALSE;

    //
    // Resources shared with the VGA
    //

    //
    // Io Ports
    //

    accessRange[4].RangeStart = passThroughPort;
    accessRange[4].RangeLength = 0x00000001;
    accessRange[4].RangeInIoSpace = TRUE;
    accessRange[4].RangeVisible = FALSE;
    accessRange[4].RangeShareable = TRUE;

    //
    // Video Memory
    //

    accessRange[5].RangeStart = A0000PhysicalAddress;
    accessRange[5].RangeLength = 0x00010000;
    accessRange[5].RangeInIoSpace = FALSE;
    accessRange[5].RangeVisible = FALSE;
    accessRange[5].RangeShareable = TRUE;

    //
    // Check to see if there is a hardware resource conflict.
    //

    status = VideoPortVerifyAccessRanges(HwDeviceExtension,
                                         NUM_XGA_ACCESS_RANGES,
                                         accessRange);

    if (status != NO_ERROR) {

        if (findingSecondCard) {

            //
            // A conflicting second card is treated like no second card -
            // fall back to single-card operation.
            //

            return NO_ERROR;
        }

        return status;
    }

    //
    // Clear out the Emulator entries and the state size since this driver
    // does not support them. (Harmless to repeat on the second-card pass;
    // ConfigInfo is shared/already-initialized state from the port driver
    // for this one device.)
    //

    ConfigInfo->NumEmulatorAccessEntries = 0;
    ConfigInfo->EmulatorAccessEntries    = NULL;
    ConfigInfo->EmulatorAccessEntriesContext = 0;

    ConfigInfo->VdmPhysicalVideoMemoryAddress.LowPart = 0x00000000;
    ConfigInfo->VdmPhysicalVideoMemoryAddress.HighPart = 0x00000000;
    ConfigInfo->VdmPhysicalVideoMemoryLength = 0x00000000;

    ConfigInfo->HardwareStateSize = 0;

    //
    // Map in the memory at A0000 so we can test the card for the amount of
    // memory afterwards.
    //

    if ( ( foundA0000MemoryAddress =
              VideoPortGetDeviceBase(hwDeviceExtension,
                                     A0000PhysicalAddress,
                                     0x00010000,
                                     FALSE)) == NULL) {

        VideoDebugPrint((2, "XgaFindAdapter - Fail to get A0000 aperture address\n"));

        if (findingSecondCard) {

            return NO_ERROR;
        }

        return ERROR_INVALID_PARAMETER;
    }

    //
    // Map in the IO registers so we can access them.
    //

    if ( ( foundIoRegBaseAddressMapped = (ULONG)
              VideoPortGetDeviceBase(hwDeviceExtension,
                                     foundIoRegBaseAddress,
                                     XGA_IO_REGS_SIZE,
                                     TRUE)) == 0) {

        VideoDebugPrint((2, "XgaFindAdapter - Fail to get io register addresses\n"));

        if (findingSecondCard) {

            return NO_ERROR;
        }

        return ERROR_INVALID_PARAMETER;
    }

    //
    // Map in the pass-throught port. Only needs to be done once - both
    // cards share the same VGA pass-through port - so skip this on the
    // second-card pass (hwDeviceExtension->PassThroughPort is already
    // set from the first pass).
    //

    if (!findingSecondCard) {

        if ( ( hwDeviceExtension->PassThroughPort =
                  VideoPortGetDeviceBase(hwDeviceExtension,
                                         passThroughPort,
                                         1,
                                         TRUE)) == 0) {

            VideoDebugPrint((2, "XgaFindAdapter - Fail to get io register addresses\n"));

            return ERROR_INVALID_PARAMETER;
        }
    }

    //
    // Probe the card for its true installed VRAM size. This must happen
    // here (rather than later in XgaInitialize) so that, by the time ANY
    // adapter's XgaInitialize() runs, the full system-wide adapter
    // inventory - including every other adapter's memory size - is
    // already known. This is what lets a weaker adapter (e.g. an onboard
    // 512K XGA-1) correctly constrain the mode list offered on a more
    // capable XGA-2 card installed alongside it.
    //

    XgaProbeCardMemory(hwDeviceExtension,
                       foundIoRegBaseAddressMapped,
                       foundA0000MemoryAddress,
                       &foundFrameBufferLength);

    //
    // Determine this card's board type from the POS data captured by
    // XgaGetPosData() into hwDeviceExtension->BoardType (it always writes
    // there regardless of which pass we are on).
    //

    foundBoardType = hwDeviceExtension->BoardType;

    //
    // Commit this card's resources into the device extension - either the
    // primary fields (first card) or the Card2-prefixed fields (second
    // card).
    //

    if (!findingSecondCard) {

        hwDeviceExtension->PhysicalRomBaseAddress = foundRomBaseAddress;
        hwDeviceExtension->PhysicalIoRegBaseAddress = foundIoRegBaseAddress;
        hwDeviceExtension->PhysicalVideoMemoryAddress = foundVideoMemoryAddress;
        hwDeviceExtension->PhysicalVideoMemoryLength = foundVideoMemoryLength;
        hwDeviceExtension->PhysicalCoProcessorAddress = foundCoProcessorAddress;
        hwDeviceExtension->IoRegBaseAddress = foundIoRegBaseAddressMapped;
        hwDeviceExtension->A0000MemoryAddress = foundA0000MemoryAddress;
        hwDeviceExtension->FrameBufferLength = foundFrameBufferLength;
        hwDeviceExtension->BoardType = foundBoardType;

        hwDeviceExtension->NumCards = 1;

        //
        // We have successfully found and configured the first card. Ask
        // to be called again so we have a chance to find a second card
        // and merge it into this same device extension.
        //

        *Again = 1;

    } else {

        hwDeviceExtension->Card2PhysicalRomBaseAddress = foundRomBaseAddress;
        hwDeviceExtension->Card2PhysicalIoRegBaseAddress = foundIoRegBaseAddress;
        hwDeviceExtension->Card2PhysicalVideoMemoryAddress = foundVideoMemoryAddress;
        hwDeviceExtension->Card2PhysicalVideoMemoryLength = foundVideoMemoryLength;
        hwDeviceExtension->Card2PhysicalCoProcessorAddress = foundCoProcessorAddress;
        hwDeviceExtension->Card2IoRegBaseAddress = foundIoRegBaseAddressMapped;
        hwDeviceExtension->Card2A0000MemoryAddress = foundA0000MemoryAddress;
        hwDeviceExtension->Card2FrameBufferLength = foundFrameBufferLength;
        hwDeviceExtension->Card2BoardType = foundBoardType;

        hwDeviceExtension->NumCards = 2;

        //
        // We now have two cards merged into this one device. Do not ask
        // to be called again - a third physical adapter, if present, is
        // intentionally left unused.
        //

        *Again = 0;

        VideoDebugPrint((1, "Xga: second card merged for extended desktop - BoardType=0x%x\n",
                         foundBoardType));
    }

    //
    // Record this adapter's board type and detected memory size in the
    // system-wide inventory, so XgaInitialize() can later compute the
    // modes common to every adapter present in the machine. This is done
    // for both the first and second card.
    //

    if (XgaAdapterInventoryCount < XGA_MAX_ADAPTERS) {

        XgaAdapterInventory[XgaAdapterInventoryCount].BoardType = foundBoardType;
        XgaAdapterInventory[XgaAdapterInventoryCount].FrameBufferLength = foundFrameBufferLength;

        XgaAdapterInventoryCount++;

    } else {

        VideoDebugPrint((0, "Xga: adapter inventory full - more than %d XGA adapters found\n",
                         XGA_MAX_ADAPTERS));
    }

    //
    // Everything is successful. Put some information in the registry.
    // (For the merged dual-card case, this currently still reflects only
    // the primary/first card - that is sufficient for Stage 0, since the
    // registry ChipType string is purely informational.)
    //

    {
        PWSTR pwszChip;
        ULONG cbChip;

        if (hwDeviceExtension->BoardType == XGA_TYPE_1) {

            pwszChip = L"XGA 1";
            cbChip = sizeof(L"XGA 1");

        } else if (hwDeviceExtension->BoardType == XGA_TYPE_2) {

            pwszChip = L"XGA 2";
            cbChip = sizeof(L"XGA 2");

        } else {

            pwszChip = L"bad XGA type";
            cbChip = sizeof(L"bad XGA type");

        }

        VideoPortSetRegistryParameters(hwDeviceExtension,
                                       L"HardwareInformation.ChipType",
                                       pwszChip,
                                       cbChip);

    }


    //
    // Indicate a successful completion status.
    //

    return NO_ERROR;

} // end XgaFindAdapter()


VP_STATUS
XgaGetPosData(
    PVOID HwDeviceExtension,
    PVOID Context,
    VIDEO_DEVICE_DATA_TYPE DeviceDataType,
    PVOID Identifier,
    ULONG IdentifierLength,
    PVOID ConfigurationData,
    ULONG ConfigurationDataLength,
    PVOID ComponentInformation,
    ULONG ComponentInformationLength
    )

/*++

Routine Description:

    Callback for the GetDeviceData function.
    This routine will scan for an XGA adapter, and if it finds one, will
    save the POS information in the device extension so it can be further
    processed in the FindAdapter() routine.

Arguments:

    HwDeviceExtension - Pointer to the miniport drivers device extension.

    Context - Context value passed to the VideoPortGetDeviceData function.

    DeviceDataType - The type of data that was requested in
        VideoPortGetDeviceData.

    Identifier - Pointer to a string that contains the name of the device,
        as setup by the ROM or ntdetect.

    IdentifierLength - Length of the Identifier string.

    ConfigurationData - Pointer to the configuration data for the device or
        BUS.

    ConfigurationDataLength - Length of the data in the configurationData
        field.

    ComponentInformation - Undefined.

    ComponentInformationLength - Undefined.

Return Value:

    Returns NO_ERROR if the function completed properly.
    Returns ERROR_DEV_NOT_EXIST if we did not find the device.
    Returns ERROR_INVALID_PARAMETER otherwise.

--*/

{
    PHW_DEVICE_EXTENSION hwDeviceExtension = HwDeviceExtension;
    USHORT adapterId;
    BOOLEAN found = FALSE;
    PCM_MCA_POS_DATA posData = Context;

    //
    // Find the XGA.
    // Loop through all the POS data in the PosData array
    //

    while ((XgaSlot + 1) * sizeof(CM_MCA_POS_DATA) <= ConfigurationDataLength) {

        adapterId = *((PUSHORT)((PCM_MCA_POS_DATA)ConfigurationData + XgaSlot));

        VideoDebugPrint((3, "PosAdapterId for slot %d is %x \n", XgaSlot,
                         adapterId));

        switch ( adapterId ) {

        case 0x8FDB:

            hwDeviceExtension->BoardType = XGA_TYPE_1;

            found = TRUE;
            break;

        case 0x8FDA:

            hwDeviceExtension->BoardType = XGA_TYPE_2;

            found = TRUE;
            break;

        default:
            break;

        }

        //
        // Go to the next slot.
        //

        XgaSlot++;

        //
        // We have found an XGA adapter. Save the data in the device extension
        // so we can process it further.
        //

        if (found) {

            *posData = *((PCM_MCA_POS_DATA)ConfigurationData + XgaSlot - 1);

            return NO_ERROR;
        }
    }

    return ERROR_DEV_NOT_EXIST;

}


BOOLEAN
XgaInitialize(
    PVOID HwDeviceExtension
    )

/*++

Routine Description:

    This routine does one time initialization of the device.

    VRAM detection has already been performed for this adapter (and every
    other XGA/XGA-2 adapter in the system) back in XgaFindAdapter(), and
    recorded in XgaAdapterInventory[]. This routine uses that system-wide
    inventory to compute the set of modes valid on THIS device: a mode is
    only marked valid if every adapter present in the machine - not just
    this one - has enough memory for it and a register table for it. This
    means a weaker adapter (e.g. an onboard 512K XGA-1) correctly limits
    the modes offered on a more capable XGA-2 card installed alongside it,
    matching the behavior of IBM's MXGA driver for OS/2.

    Note: when two cards have been merged into this device extension
    (hwDeviceExtension->NumCards == 2), the mode list computed here still
    represents what BOTH merged cards can run identically, since the
    inventory-based lowest-common-denominator computation below already
    accounts for every adapter in the system, including both merged ones.

Arguments:

    HwDeviceExtension - Supplies a pointer to the miniport's device extension.

Return Value:


    Always returns TRUE since this routine can never fail.

--*/

{
    PHW_DEVICE_EXTENSION hwDeviceExtension = HwDeviceExtension;
    ULONG i;
    ULONG j;
    ULONG color;
    ULONG modeBytes;
    ULONG systemMinFrameBufferLength;
    UCHAR systemBoardTypeMask;
    ULONG thisDeviceMinFrameBufferLength;

    //
    // hwDeviceExtension->FrameBufferLength contains the amount of memory
    // on the device.  Lets write this information into the registry.
    //
    // This value is in bytes, which is what I believe we want.
    //

    VideoPortSetRegistryParameters(hwDeviceExtension,
                                   L"HardwareInformation.MemorySize",
                                   &hwDeviceExtension->FrameBufferLength,
                                   sizeof(ULONG));
								   
    								
								   
	if (hwDeviceExtension->BoardType == XGA_TYPE_1)
	{
	
		VideoPortSetRegistryParameters(hwDeviceExtension,
									   L"HardwareInformation.ChipType",
									   L"XGA",
									   sizeof(L"XGA"));
									   
		VideoPortSetRegistryParameters(hwDeviceExtension,
                                   L"HardwareInformation.DacType",
                                   L"6-bit Integrated RAMDAC",
                                   sizeof(L"6-bit Integrated RAMDAC"));	
								   
		VideoPortSetRegistryParameters(hwDeviceExtension,
                                   L"HardwareInformation.AdapterString",
                                   L"IBM XGA Based Adapter",
                                   sizeof(L"IBM XGA Based Adapter"));
	}
	else if (hwDeviceExtension->BoardType == XGA_TYPE_2)
	{
	
		VideoPortSetRegistryParameters(hwDeviceExtension,
									   L"HardwareInformation.ChipType",
									   L"XGA-2",
									   sizeof(L"XGA-2"));
		
		VideoPortSetRegistryParameters(hwDeviceExtension,
                                   L"HardwareInformation.DacType",
                                   L"8-bit Integrated RAMDAC",
                                   sizeof(L"8-bit Integrated RAMDAC"));
		
		VideoPortSetRegistryParameters(hwDeviceExtension,
                                   L"HardwareInformation.AdapterString",
                                   L"IBM XGA-2 Based Adapter",
                                   sizeof(L"IBM XGA-2 Based Adapter"));		
	}
	
	VideoPortSetRegistryParameters(hwDeviceExtension,
                            L"HardwareInformation.BiosString",
                            L"VGA Compatible",
                            sizeof(L"VGA Compatible"));	

						
	//
    // For now, we only support color devices
    //

    hwDeviceExtension->Color = TRUE;

    //
    // Compute the lowest-common-denominator constraints across every
    // adapter discovered in the system: the smallest installed VRAM size,
    // and the union of board types present. A mode must fit within the
    // smallest VRAM size and must have a register table for every board
    // type present in order to be usable on all adapters simultaneously.
    //
    // thisDeviceMinFrameBufferLength additionally accounts for a SECOND
    // card merged into THIS SAME device extension (if any) - since both
    // merged cards must run the identical mode (they present one extended
    // desktop), a mode must fit in whichever of the two merged cards has
    // less VRAM, not just the first one.
    //

    thisDeviceMinFrameBufferLength = hwDeviceExtension->FrameBufferLength;

    if ( (hwDeviceExtension->NumCards == 2) &&
         (hwDeviceExtension->Card2FrameBufferLength < thisDeviceMinFrameBufferLength) ) {

        thisDeviceMinFrameBufferLength = hwDeviceExtension->Card2FrameBufferLength;
    }

    systemMinFrameBufferLength = thisDeviceMinFrameBufferLength;
    systemBoardTypeMask = hwDeviceExtension->BoardType;

    if (hwDeviceExtension->NumCards == 2) {

        systemBoardTypeMask |= hwDeviceExtension->Card2BoardType;
    }

    for (j = 0; j < XgaAdapterInventoryCount; j++) {

        if (XgaAdapterInventory[j].FrameBufferLength < systemMinFrameBufferLength) {

            systemMinFrameBufferLength = XgaAdapterInventory[j].FrameBufferLength;

        }

        systemBoardTypeMask |= XgaAdapterInventory[j].BoardType;

    }

    VideoDebugPrint((1, "XgaInitialize: system-wide min VRAM = %d K, board type mask = 0x%x, NumCards on this device = %d\n",
                     systemMinFrameBufferLength >> 10, systemBoardTypeMask, hwDeviceExtension->NumCards));

    //
    // Now compute the number of available modes based on the information
    // we found out - constrained to what every adapter in the system can
    // support (including a second card merged into this same device, if
    // any), not just the primary card on this device.
    //

    hwDeviceExtension->NumAvailableModes = 0;

    for (i = 0; i < XgaNumModes; i++) {

        XgaModes[i].modeInformation.ModeIndex = i;

        modeBytes = XgaModes[i].modeInformation.ScreenStride *
                    XgaModes[i].modeInformation.VisScreenHeight;

        //
        // The mode must fit in this device's own memory (the smaller of
        // its one or two merged cards' VRAM) and in the smallest memory
        // size found anywhere in the system.
        //

        if ( (modeBytes <= thisDeviceMinFrameBufferLength) &&
             (modeBytes <= systemMinFrameBufferLength) ) {

            //
            // Check that this device's card(s) can themselves run the
            // mode ...
            //

            if ( (XgaModes[i].Xga1Mode &&
                     (hwDeviceExtension->BoardType & XGA_TYPE_1)) ||
                 (XgaModes[i].Xga2Mode &&
                     (hwDeviceExtension->BoardType & XGA_TYPE_2)) ) {

                //
                // ... and that every adapter type present anywhere in the
                // system has a register table for this mode. If, say, an
                // XGA-1 is present elsewhere in the machine, a mode that
                // only has an Xga2Mode table is rejected everywhere, even
                // on an XGA-2 card that could otherwise run it.
                //

                if ( ((systemBoardTypeMask & XGA_TYPE_1) ? (XgaModes[i].Xga1Mode != NULL) : TRUE) &&
                     ((systemBoardTypeMask & XGA_TYPE_2) ? (XgaModes[i].Xga2Mode != NULL) : TRUE) ) {

                    color = XgaModes[i].modeInformation.AttributeFlags &
                            VIDEO_MODE_COLOR;

                    //
                    // check that both the mode and what we support is the same
                    // in terms of color.
                    //

                    if ( (color && (ULONG)hwDeviceExtension->Color) ||  // color
                         (color == (ULONG)hwDeviceExtension->Color) ) { // monochrome

                        hwDeviceExtension->Valid[i] = TRUE;
                        hwDeviceExtension->NumAvailableModes++;

                    }
                }
            }
        }
    }

    VideoPortGetRegistryParameters(hwDeviceExtension,
                                   L"Monitor",
                                   TRUE,
                                   XgaGetRegistryParamaterCallback,
                                   NULL);

    //
    // Make sure we leave the hardware in a state where int10 from vga will
    // work. This is necessary for detection where only Initialize will
    // be called, without a setmode\resetdevice being done afterwards.
    //

    XgaResetHW(HwDeviceExtension, 0, 0);

    return TRUE;

} // end XgaInitialize()


VP_STATUS
XgaGetRegistryParamaterCallback(
    PVOID HwDeviceExtension,
    PVOID Context,
    PWSTR ValueName,
    PVOID ValueData,
    ULONG ValueLength
    )

/*++

Routine Description:

    Callback routine to process the information coming back from the registry.

Arguments:

    HwDeviceExtension - Pointer to the miniport driver's device extension.

    Context - Context parameter passed to the callback routine.

    ValueName - Pointer to the name of the requested data field.

    ValueData - Pointer to a buffer containing the information.

    ValueLength - Size of the data.

Return Value:


Environment:

    Can only be called During initialization.

--*/

{

    VideoDebugPrint((3, "In the GetRegistryParameters callback routine\n"));

    return NO_ERROR;

}


BOOLEAN
XgaStartIO(
    PVOID HwDeviceExtension,
    PVIDEO_REQUEST_PACKET RequestPacket
    )

/*++

Routine Description:

    This routine is the main execution routine for the miniport driver. It
    acceptss a Video Request Packet, performs the request, and then returns
    with the appropriate status.

Arguments:

    HwDeviceExtension - Supplies a pointer to the miniport's device extension.

    RequestPacket - Pointer to the video request packet. This structure
        contains all the parameters passed to the VideoIoControl function.

Return Value:


--*/

{
    VP_STATUS status;
    PHW_DEVICE_EXTENSION hwDeviceExtension = HwDeviceExtension;

    PVIDEO_MEMORY_INFORMATION memoryInformation;
    PVIDEO_MODE_INFORMATION modeInformation;
    ULONG inIoSpace;

    PVIDEO_CLUT clutBuffer;
    PVIDEO_CLUTDATA pClutData;

    ULONG i;
    ULONG length;
    PHYSICAL_ADDRESS physicalAddress;
    ULONG offset;

    ULONG requestedCardIndex;
    PHYSICAL_ADDRESS cardVideoMemoryAddress;
    ULONG cardVideoMemoryLength;
    PHYSICAL_ADDRESS cardCoProcessorAddress;
    ULONG cardIoRegBaseAddressPhysical;


    //
    // Switch on the IoContolCode in the RequestPacket. It indicates which
    // function must be performed by the driver.
    //

    switch (RequestPacket->IoControlCode) {


    case IOCTL_VIDEO_MAP_VIDEO_MEMORY:

        VideoDebugPrint((2, "XgaStartIO - MapVideoMemory\n"));

        if ( (RequestPacket->OutputBufferLength <
              (RequestPacket->StatusBlock->Information =
                                     sizeof(VIDEO_MEMORY_INFORMATION))) ||
             (RequestPacket->InputBufferLength < sizeof(VIDEO_MEMORY)) ) {

            status = ERROR_INSUFFICIENT_BUFFER;
            break;
        }

        //
        // Determine which card's video memory is being requested. For
        // backward compatibility, if the caller's input buffer is only
        // large enough for the (older, card-agnostic) VIDEO_MEMORY
        // structure with nothing extra appended, we default to card 0 -
        // this is exactly what the original single-card xgadll does.
        //

        requestedCardIndex = 0;

        if ( (hwDeviceExtension->NumCards == 2) &&
             (RequestPacket->InputBufferLength >=
                 (sizeof(VIDEO_MEMORY) + sizeof(VIDEO_XGA_CARD_REQUEST))) ) {

            requestedCardIndex = ((PVIDEO_XGA_CARD_REQUEST)
                ((PUCHAR)RequestPacket->InputBuffer + sizeof(VIDEO_MEMORY)))->CardIndex;

            if (requestedCardIndex > 1) {

                requestedCardIndex = 0;
            }
        }

        if ( (requestedCardIndex == 1) && (hwDeviceExtension->NumCards == 2) ) {

            cardVideoMemoryAddress = hwDeviceExtension->Card2PhysicalVideoMemoryAddress;
            cardVideoMemoryLength  = hwDeviceExtension->Card2PhysicalVideoMemoryLength;

        } else {

            cardVideoMemoryAddress = hwDeviceExtension->PhysicalVideoMemoryAddress;
            cardVideoMemoryLength  = hwDeviceExtension->PhysicalVideoMemoryLength;
        }

        memoryInformation = RequestPacket->OutputBuffer;

        memoryInformation->VideoRamBase = ((PVIDEO_MEMORY)
                (RequestPacket->InputBuffer))->RequestedVirtualAddress;

        memoryInformation->VideoRamLength = cardVideoMemoryLength;

        inIoSpace = 0;

        status = VideoPortMapMemory(hwDeviceExtension,
                                    cardVideoMemoryAddress,
                                    &(memoryInformation->VideoRamLength),
                                    &inIoSpace,
                                    &(memoryInformation->VideoRamBase));

        //
        // The frame buffer and virtual memory and equivalent in this
        // case.
        //

        memoryInformation->FrameBufferBase =
            memoryInformation->VideoRamBase;

        memoryInformation->FrameBufferLength =
            XgaModes[hwDeviceExtension->CurrentMode].modeInformation.VisScreenHeight *
            XgaModes[hwDeviceExtension->CurrentMode].modeInformation.ScreenStride;

        VideoDebugPrint((3, "Xga IOCLT_MAP_MEMORY (card %d)\n physical = %x\n \
virtual = %x\n length = %x\n framebase = %x\n frameLength = %x\n",
                           requestedCardIndex,
                           cardVideoMemoryAddress.LowPart,
                           memoryInformation->VideoRamBase,
                           memoryInformation->VideoRamLength,
                           memoryInformation->FrameBufferBase,
                           memoryInformation->FrameBufferLength));

        break;


    case IOCTL_VIDEO_UNMAP_VIDEO_MEMORY:

        VideoDebugPrint((2, "XgaStartIO - UnMapVideoMemory\n"));

        if (RequestPacket->InputBufferLength < sizeof(VIDEO_MEMORY)) {

            status = ERROR_INSUFFICIENT_BUFFER;
            break;
        }

        status = VideoPortUnmapMemory(hwDeviceExtension,
                                      ((PVIDEO_MEMORY)
                                       (RequestPacket->InputBuffer))->
                                           RequestedVirtualAddress,
                                      0);
        break;


    case IOCTL_VIDEO_QUERY_NUM_AVAIL_MODES:

        VideoDebugPrint((2, "XgaStartIO - QueryNumberAvaialbleModes\n"));

        if (RequestPacket->OutputBufferLength <
            (RequestPacket->StatusBlock->Information =
                sizeof(VIDEO_NUM_MODES)) ) {

            status = ERROR_INSUFFICIENT_BUFFER;

        } else {

            ((PVIDEO_NUM_MODES)RequestPacket->OutputBuffer)->NumModes =
                hwDeviceExtension->NumAvailableModes;
            ((PVIDEO_NUM_MODES)RequestPacket->OutputBuffer)->ModeInformationLength =
                sizeof(VIDEO_MODE_INFORMATION);

            status = NO_ERROR;
        }

        break;


    case IOCTL_VIDEO_QUERY_AVAIL_MODES:

        VideoDebugPrint((2, "XgaStartIO - QueryAvailableModes\n"));

        if (RequestPacket->OutputBufferLength <
            (RequestPacket->StatusBlock->Information =
                 hwDeviceExtension->NumAvailableModes *
                 sizeof(VIDEO_MODE_INFORMATION)) ) {

            status = ERROR_INSUFFICIENT_BUFFER;

        } else {

            modeInformation = RequestPacket->OutputBuffer;

            for (i = 0; i < XgaNumModes; i++) {

                if (hwDeviceExtension->Valid[i]) {

                    *modeInformation = XgaModes[i].modeInformation;
                    modeInformation++;

                }
            }

            status = NO_ERROR;
        }

        break;


    case IOCTL_VIDEO_QUERY_CURRENT_MODE:

        VideoDebugPrint((2, "XgaStartIO - QueryCurrentMode\n"));

        if (RequestPacket->OutputBufferLength <
            (RequestPacket->StatusBlock->Information =
            sizeof(VIDEO_MODE_INFORMATION)) ) {

            status = ERROR_INSUFFICIENT_BUFFER;

        } else {

            *((PVIDEO_MODE_INFORMATION)RequestPacket->OutputBuffer) =
                XgaModes[hwDeviceExtension->CurrentMode].modeInformation;

            status = NO_ERROR;

        }

        break;


    case IOCTL_VIDEO_SET_CURRENT_MODE:

        VideoDebugPrint((2, "XgaStartIO - SetCurrentMode\n"));

        if (RequestPacket->InputBufferLength < sizeof(VIDEO_MODE)) {

            status = ERROR_INSUFFICIENT_BUFFER;

        } else {

            status = XgaSetMode(hwDeviceExtension,
                                ((PVIDEO_MODE)RequestPacket->InputBuffer)->
                                RequestedMode);

        }

        break;


    case IOCTL_VIDEO_SET_COLOR_REGISTERS:

        VideoDebugPrint((2, "XgaStartIO - SetColorRegs\n"));

        clutBuffer = RequestPacket->InputBuffer;

        /*   Load simple 16 entry palette */

        XGAIDXOUT(INDEX_REG, 0x66, 0x0);

        /* Start at beginning of palette */

        XGAIDXOUT(INDEX_REG, 0x60, clutBuffer->FirstEntry);
        XGAIDXOUT(INDEX_REG, 0x61, 0x0);


        for ( i = 0; i < clutBuffer->NumEntries; i++ ) {

           pClutData = &clutBuffer->LookupTable[i].RgbArray;

           XGAOUT(INDEX_REG, 0x65);
           XGAOUT(DATA_IN_REG, pClutData->Red);

           XGAOUT(INDEX_REG, 0x65);
           XGAOUT(DATA_IN_REG, pClutData->Green);

           XGAOUT(INDEX_REG, 0x65);
           XGAOUT(DATA_IN_REG, pClutData->Blue);
        }

        status = NO_ERROR;

        break;

    case IOCTL_VIDEO_XGA_MAP_COPROCESSOR:

        if (RequestPacket->OutputBufferLength <
            (RequestPacket->StatusBlock->Information =
            sizeof(VIDEO_XGA_COPROCESSOR_INFORMATION)) ) {

            status = ERROR_INSUFFICIENT_BUFFER;
            break;

        }

        //
        // Determine which card's coprocessor is being requested. Default
        // to card 0 if no (or too small an) input buffer is supplied -
        // this preserves exact backward compatibility with the original
        // single-card xgadll, which never supplies an input buffer here.
        //

        requestedCardIndex = 0;

        if ( (hwDeviceExtension->NumCards == 2) &&
             (RequestPacket->InputBufferLength >= sizeof(VIDEO_XGA_CARD_REQUEST)) ) {

            requestedCardIndex =
                ((PVIDEO_XGA_CARD_REQUEST)RequestPacket->InputBuffer)->CardIndex;

            if (requestedCardIndex > 1) {

                requestedCardIndex = 0;
            }
        }

        if ( (requestedCardIndex == 1) && (hwDeviceExtension->NumCards == 2) ) {

            cardCoProcessorAddress = hwDeviceExtension->Card2PhysicalCoProcessorAddress;
            cardVideoMemoryAddress = hwDeviceExtension->Card2PhysicalVideoMemoryAddress;
            cardIoRegBaseAddressPhysical = hwDeviceExtension->Card2PhysicalIoRegBaseAddress.LowPart;

        } else {

            cardCoProcessorAddress = hwDeviceExtension->PhysicalCoProcessorAddress;
            cardVideoMemoryAddress = hwDeviceExtension->PhysicalVideoMemoryAddress;
            cardIoRegBaseAddressPhysical = hwDeviceExtension->PhysicalIoRegBaseAddress.LowPart;
        }

        length = XGA_CO_PROCESSOR_REGS_SIZE;

        // Map in at the page ganularity, then add in the page offset
        // to the linear address.

        offset = cardCoProcessorAddress.LowPart & 0x00000FFF;

        physicalAddress.LowPart = cardCoProcessorAddress.LowPart & 0xFFFFF000;

        physicalAddress.HighPart = 0x00000000;

        ((PVIDEO_XGA_COPROCESSOR_INFORMATION)RequestPacket->OutputBuffer)->
            CoProcessorVirtualAddress = 0;

        inIoSpace = 0;

        status = VideoPortMapMemory(hwDeviceExtension,
                                    physicalAddress,
                                    &length,
                                    &inIoSpace,
                                    &((PVIDEO_XGA_COPROCESSOR_INFORMATION)RequestPacket->
                                        OutputBuffer)->CoProcessorVirtualAddress);

        (PCHAR) ((PVIDEO_XGA_COPROCESSOR_INFORMATION)
             RequestPacket->OutputBuffer)->CoProcessorVirtualAddress += offset;

        //
        // Also return the physical address of the video memory so the
        // co-processor can access it.
        //


        (ULONG) ((PVIDEO_XGA_COPROCESSOR_INFORMATION)
            RequestPacket->OutputBuffer)->PhysicalVideoMemoryAddress =
                cardVideoMemoryAddress.LowPart;

        //
        // Also return the XGA IO Register Base Address.
        //

        (ULONG) ((PVIDEO_XGA_COPROCESSOR_INFORMATION)
            RequestPacket->OutputBuffer)->XgaIoRegisterBaseAddress =
                cardIoRegBaseAddressPhysical;


        VideoDebugPrint((3, "Xga IOCLT_MAP_CO_PROCESSOR (card %d)\n physicalco = %x\n virtualco = %x\n physicalmem = %x\n",
                         requestedCardIndex,
                         cardCoProcessorAddress.LowPart,
                         ((PVIDEO_XGA_COPROCESSOR_INFORMATION)
                             RequestPacket->OutputBuffer)->CoProcessorVirtualAddress,
                         cardVideoMemoryAddress.LowPart));

        break;


    case IOCTL_VIDEO_GET_BANK_SELECT_CODE:

        VideoDebugPrint((2, "XgaStartIO - GetBankSelectCode\n"));

        status = XgaGetBankSelectCode(HwDeviceExtension,
                                        (PVIDEO_BANK_SELECT) RequestPacket->OutputBuffer,
                                        RequestPacket->OutputBufferLength,
                                        &RequestPacket->StatusBlock->Information);

        break;


    case IOCTL_VIDEO_RESET_DEVICE:

        XgaResetHW(HwDeviceExtension, 0, 0);

        status = NO_ERROR;

        break;

    //
    // if we get here, an invalid IoControlCode was specified.
    //

    default:

        VideoDebugPrint((1, "Fell through Xga startIO routine - invalid command\n"));

        status = ERROR_INVALID_FUNCTION;

        break;

    }

    RequestPacket->StatusBlock->Status = status;

    return TRUE;

} // end XgaStartIO()


VOID
XgaSetModeOnCard(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG ModeNum,
    UCHAR BoardType,
    ULONG IoRegBaseAddress
    )

/*++

Routine Description:

    Programs the mode-set register table for ModeNum into a single card's
    registers, using the supplied BoardType (to select the Xga1Mode or
    Xga2Mode table) and IoRegBaseAddress (to target either the primary
    card or a second, merged-in card). This is the same register
    programming sequence that was previously inlined directly in
    XgaSetMode() for the single-card case; it is factored out here purely
    so it can be invoked twice (once per card) when two cards have been
    merged into one device extension.

Arguments:

    HwDeviceExtension - Supplies a pointer to the miniport's device
        extension (only used here to satisfy the XGAOUT macro's
        reference to a local hwDeviceExtension - see below).

    ModeNum - Index of the mode in which the card must be programmed.

    BoardType - XGA_TYPE_1 or XGA_TYPE_2, selecting which register table
        to use for this card.

    IoRegBaseAddress - I/O base address of the XGA registers for the card
        being programmed.

--*/

{
    HW_DEVICE_EXTENSION localExtension;
    PHW_DEVICE_EXTENSION hwDeviceExtension = &localExtension;
    PUCHAR indexRegister;
    PMODE_REGISTER_DATA_TABLE modeTable;
    UCHAR input;

    hwDeviceExtension->IoRegBaseAddress = IoRegBaseAddress;
    indexRegister = (PUCHAR) (IoRegBaseAddress + INDEX_REG);

    if (BoardType & XGA_TYPE_1) {

        modeTable = XgaModes[ModeNum].Xga1Mode;

    } else if (BoardType & XGA_TYPE_2) {

        modeTable = XgaModes[ModeNum].Xga2Mode;

    } else {

        VideoDebugPrint((0, "XGA: Internal mode flags are invalid (XgaSetModeOnCard)"));
        return;
    }

    //
    // Program the registers.
    //

    while (modeTable->Port != END_OF_SWITCH) {

        switch (modeTable->Port) {

        case INDEX_REG:

            VideoPortWritePortUchar(indexRegister,
                                    modeTable->IndexPort);

            VideoPortWritePortUchar(indexRegister + 1,
                                    modeTable->Data);

            break;

        case INDEX_OR_REG:

            VideoPortWritePortUchar(indexRegister,
                                    modeTable->IndexPort);

            input = VideoPortReadPortUchar((PUCHAR) IoRegBaseAddress +
                                           DATA_IN_REG);

            input |= modeTable->Data;

            VideoPortWritePortUchar(indexRegister + 1, input);

            break;

        default:

            VideoPortWritePortUchar((PUCHAR) IoRegBaseAddress +
                                        modeTable->Port,
                                    modeTable->Data);

        }

        modeTable++;

    }

} // end XgaSetModeOnCard()


VP_STATUS
XgaSetMode(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG ModeNum
    )

/*++

Routine Description:

    This routine is the main execution routine for the miniport driver. It
    acceptss a Video Request Packet, performs the request, and then returns
    with the appropriate status.

    Dual-card support: if two cards have been merged into this device
    extension (HwDeviceExtension->NumCards == 2), BOTH cards are
    programmed into the SAME mode here, one after the other, via
    XgaSetModeOnCard(). Since XgaInitialize()'s mode-validity computation
    already guarantees that any mode marked Valid[] for this device is
    runnable identically on both merged cards (same resolution/depth),
    this keeps both monitors synchronized to one consistent mode - which
    is required for the extended-desktop image built by xgadll to line up
    correctly across the seam between the two cards.

Arguments:

    HwDeviceExtension - Supplies a pointer to the miniport's device extension.

    ModeNum - Index of the mode in which the adapter must be programmed.

Return Value:

    NO_ERROR if the function completed successfully
    ERROR_INVALID_PARAMETER if the requested mode is invalid for the device.

--*/

{
    PHW_DEVICE_EXTENSION hwDeviceExtension = HwDeviceExtension;

    VideoDebugPrint((1, "XgaSetMode: the mode being set is %d\n", ModeNum));

    //
    // Validate that this device's own board type has a register table
    // for this mode before doing anything - matches the original
    // single-card behavior, which failed the call in this situation.
    //

    if ( !( ((hwDeviceExtension->BoardType & XGA_TYPE_1) && XgaModes[ModeNum].Xga1Mode) ||
            ((hwDeviceExtension->BoardType & XGA_TYPE_2) && XgaModes[ModeNum].Xga2Mode) ) ) {

        VideoDebugPrint((0, "XGA: Internal mode flags are invalid"));
        return ERROR_INVALID_PARAMETER;
    }

    //
    // Program the primary card.
    //

    XgaSetModeOnCard(hwDeviceExtension,
                     ModeNum,
                     hwDeviceExtension->BoardType,
                     hwDeviceExtension->IoRegBaseAddress);

    //
    // If a second card has been merged into this device, program it into
    // the identical mode as well.
    //

    if (hwDeviceExtension->NumCards == 2) {

        if ( !( ((hwDeviceExtension->Card2BoardType & XGA_TYPE_1) && XgaModes[ModeNum].Xga1Mode) ||
                ((hwDeviceExtension->Card2BoardType & XGA_TYPE_2) && XgaModes[ModeNum].Xga2Mode) ) ) {

            VideoDebugPrint((0, "XGA: Internal mode flags are invalid (card 2)"));
            return ERROR_INVALID_PARAMETER;
        }

        XgaSetModeOnCard(hwDeviceExtension,
                         ModeNum,
                         hwDeviceExtension->Card2BoardType,
                         hwDeviceExtension->Card2IoRegBaseAddress);
    }

    //
    // Save the current mode for future reference.
    //

    hwDeviceExtension->CurrentMode = (UCHAR) ModeNum;

    return NO_ERROR;

} // end XgaSetMode();


VP_STATUS
XgaGetBankSelectCode(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    PVIDEO_BANK_SELECT BankSelect,
    ULONG BankSelectSize,
    PULONG OutputSize
    )

/*++

Routine Description:

    Returns information needed in order for caller to implement bank
         management.

Arguments:

    HwDeviceExtension - Pointer to the miniport driver's device extension.

    BankSelect - Pointer to a VIDEO_BANK_SELECT structure in which the bank
             select data will be returned (output buffer).

    BankSelectSize - Length of the output buffer supplied by the user.

    OutputSize - Pointer to a variable in which to return the actual size of
        the data returned in the output buffer.

Return Value:

    NO_ERROR - information returned successfully

    ERROR_MORE_DATA - output buffer not large enough to hold all info (but
        Size is returned, so caller can tell how large a buffer to allocate)

    ERROR_INSUFFICIENT_BUFFER - output buffer not large enough to return
        any useful data

    ERROR_INVALID_PARAMETER - invalid video mode selection

--*/

{
    ULONG codeSize;
    PUCHAR pCodeDest;
    PUCHAR pCodeBank;

    //
    // The minimum passed buffer size is a VIDEO_BANK_SELECT
    // structure, so that we can return the required size; we can't do
    // anything if we don't have at least that much buffer.
    //

    if (BankSelectSize < sizeof(VIDEO_BANK_SELECT)) {

        return ERROR_INSUFFICIENT_BUFFER;

    }

    //
    // This serves an a ID for the version of the structure we're using.
    //

    BankSelect->Length = sizeof(VIDEO_BANK_SELECT);

    //
    // Determine the banking type, and set whether any banking is actually
    // supported in this mode.
    //

    BankSelect->BankingFlags = 0;
    BankSelect->BankingType = VideoBanked1RW;
    BankSelect->Granularity = 0x10000;

    BankSelect->BitmapWidthInBytes =
        XgaModes[HwDeviceExtension->CurrentMode].modeInformation.ScreenStride;

    BankSelect->BitmapSize = HwDeviceExtension->FrameBufferLength;

    pCodeBank = &BankSwitchStart;
    codeSize = ((ULONG)&BankSwitchEnd) - ((ULONG)&BankSwitchStart);

    //
    // Size of complete banking info structure.
    //

    BankSelect->Size = sizeof(VIDEO_BANK_SELECT) + codeSize;

    //
    // If the buffer isn't big enough to hold all info, just return
    // ERROR_MORE_DATA; Size is already set.
    //

    if (BankSelectSize < BankSelect->Size ) {

        //
        // We're returning only the VIDEO_BANK_SELECT structure.
        //

        *OutputSize = sizeof(VIDEO_BANK_SELECT);
        return ERROR_MORE_DATA;
    }

    //
    // Copy all banking code into the output buffer.
    //
    // Adjust the banking code first so we have the right IO port for
    // the Aperture index register
    //
    //

    *ApertureIndexRegister = HwDeviceExtension->PhysicalIoRegBaseAddress.LowPart
                             + APP_INDEX_REG;

    pCodeDest = (PUCHAR)BankSelect + sizeof(VIDEO_BANK_SELECT);

    BankSelect->CodeOffset = sizeof(VIDEO_BANK_SELECT);

    VideoPortMoveMemory(pCodeDest, pCodeBank, codeSize);

    //
    // Number of bytes we're returning is the full banking info size.
    //

    *OutputSize = BankSelect->Size;

    return NO_ERROR;

} // end XgaGetBankSelectCode()


//
// Set of registers needed to reset the XGA in standart VGA mode.
//

INDEX_REGISTER_DATA_TABLE XgaResetToVga[] = {
    {0x64, 0xff},
    {0x50, 0x15},
    {0x50, 0x14},
    {0x51, 0x00},
    {0x54, 0x04},
    {0x70, 0x00},
    {0x2A, 0x20},
    {0xFF, 0x00}
};

VOID
XgaResetHWCard(
    PHW_DEVICE_EXTENSION HwDeviceExtension,
    ULONG IoRegBaseAddress,
    PVOID PassThroughPort
    )

/*++

Routine Description:

    Resets a single card's XGA extended registers back to VGA. This is the
    same register programming sequence previously inlined directly in
    XgaResetHW() for the single-card case, factored out so it can be
    invoked once per merged card.

Arguments:

    HwDeviceExtension - Supplies a pointer to the miniport's device
        extension (only used here to satisfy the XGAOUT macro's
        reference to a local hwDeviceExtension - see below).

    IoRegBaseAddress - I/O base address of the XGA registers for the card
        being reset.

    PassThroughPort - VGA pass-through port address (shared by both
        cards).

--*/

{
    HW_DEVICE_EXTENSION localExtension;
    PHW_DEVICE_EXTENSION hwDeviceExtension = &localExtension;
    USHORT i;

    hwDeviceExtension->IoRegBaseAddress = IoRegBaseAddress;

    XGAOUT(APP_CTL_REG, 0x00);

    XGAOUT(INT_ENABLE_REG, 0x00);

    XGAOUT(INT_STATUS_REG, 0xff);

    // Now init all the index registers.

    for (i = 0 ; XgaResetToVga[i].PortIndex != 0xff ; i++) {

        XGAIDXOUT(0x0A,
                  XgaResetToVga[i].PortIndex,
                  XgaResetToVga[i].Data);
    }

    XGAOUT(OP_MODE_REG, 0x1);

    VideoPortWritePortUchar(PassThroughPort, 0x01);

} // end XgaResetHWCard()


BOOLEAN
XgaResetHW(
    IN PVOID HwDeviceExtension,
    IN ULONG Columns,
    IN ULONG Rows
    )

/*++

routine description:

    This function is called to reset the XGA extended registers back to
    VGA so we can do an int 10 back to VGA mode.

    Dual-card support: if a second card has been merged into this device
    extension, it is reset immediately after the primary card, using the
    same (shared) VGA pass-through port.

Arguments:

    hwdeviceextension - pointer to the miniport driver's device extension.
    Columns - Number of columns for text mode (not used).
    Rows - Number of rows for text mode (not used).

Return value:

    Always returns FALSE so that the Video Port driver will call Int 10 to
    set the desired video mode.

--*/

{
    PHW_DEVICE_EXTENSION hwDeviceExtension = HwDeviceExtension;

    XgaResetHWCard(hwDeviceExtension,
                   hwDeviceExtension->IoRegBaseAddress,
                   hwDeviceExtension->PassThroughPort);

    if (hwDeviceExtension->NumCards == 2) {

        XgaResetHWCard(hwDeviceExtension,
                       hwDeviceExtension->Card2IoRegBaseAddress,
                       hwDeviceExtension->PassThroughPort);
    }

    return FALSE;
}