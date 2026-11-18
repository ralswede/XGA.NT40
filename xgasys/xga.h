
/*++

Copyright (c) 1991-1992  Microsoft Corporation

Module Name:

    xga.h

Abstract:

    This module contains the global structures and definitions used
    by the xga driver.

    MCA-only build: ISA/EISA (POS-emulation / banked framebuffer) support
    has been removed. Genuine Micro Channel XGA/XGA-2 adapters only.

    Dual-card (side by side extended desktop) support: when exactly two
    MCA XGA/XGA-2 adapters are present in the system, XgaFindAdapter merges
    both of them into a SINGLE logical device (one HW_DEVICE_EXTENSION,
    one \Device\VideoN instance) instead of registering two independent
    devices. This is required because the NT4 GDI/DDI model only ever
    binds ONE display driver instance to ONE miniport device - there is no
    way for xgadll to be handed a second miniport handle by the OS. All of
    the second card's physical resources (I/O base, coprocessor, VRAM,
    POS data) are therefore tracked alongside the first card's resources
    in this same device extension, under the "Card2" prefixed fields
    below. If only one adapter is found, NumCards stays 1 and every
    Card2-prefixed field is simply unused - single card behavior is
    unchanged.

Environment:

    Kernel mode

Revision History:


--*/

//
// bit field of types of board recognized by the device
//

#define XGA_TYPE_1        1  // IBM XGA 1 board
#define XGA_TYPE_2        2  // IBM XGA 2 board

//
// Resource Sizes
//

#define XGA_IO_REGS_SIZE           0x0010
#define XGA_CO_PROCESSOR_REGS_SIZE 0x0080
#define XGA_ROM_SIZE               0x1C00

//
// XGA I/O register definitions
//

#define OP_MODE_REG      0x0
#define APP_CTL_REG      0x1
#define INT_ENABLE_REG   0x4
#define INT_STATUS_REG   0x5
#define VMEM_CONTROL_REG 0x6
#define APP_INDEX_REG    0x8
#define MEMACC_MODE_REG  0x9
#define INDEX_REG        0xA
#define DATA_IN_REG      0xB
#define DATA_OUT_REG     0xC
#define INDEX_OR_REG     0xFE
#define END_OF_SWITCH    0xFF

//
// INDEX Register definitions
//

#define DISPLAY_PIXEL_MAP_WIDTH_LOW  0x43

//
// Pos register masks
//

#define ROM_MASK    0xF0
#define INST_MASK   0x0E
#define EN_MASK     0x01

#define VIDEO_MEM_MASK            0xFE
#define VIDEO_ENABLE_MASK   0x01

#define XGAOUT(reg, val)    \
  VideoPortWritePortUchar((PUCHAR)(hwDeviceExtension->IoRegBaseAddress+reg), val)

// Added by Christian Holzapfel | 11-04-2024
// *****************************************
#define XGAIN(reg)    \
  VideoPortReadPortUchar((PUCHAR)(hwDeviceExtension->IoRegBaseAddress+reg))
// *****************************************

#define XGAIDXOUT(reg, index, val)  \
  VideoPortWritePortUshort((PUSHORT)(hwDeviceExtension->IoRegBaseAddress+reg), (USHORT)((val << 8) + index))

//
// Mode set tables
//

//
// Table entries on which the mode switch routine is based on.
//

typedef struct _MODE_REGISTER_DATA_TABLE {
    UCHAR Port;
    UCHAR IndexPort;
    UCHAR Data;
} MODE_REGISTER_DATA_TABLE, *PMODE_REGISTER_DATA_TABLE;

//
// Characteristics of each mode
//

typedef struct _XGA_VIDEO_MODE {
    PMODE_REGISTER_DATA_TABLE Xga1Mode;
    PMODE_REGISTER_DATA_TABLE Xga2Mode;
    VIDEO_MODE_INFORMATION modeInformation;
} XGA_VIDEO_MODE, *PXGA_VIDEO_MODE;

//
// Structure for VGA reseting
//

typedef struct _INDEX_REGISTER_DATA_TABLE {
    UCHAR PortIndex;
    UCHAR Data;
} INDEX_REGISTER_DATA_TABLE, *PINDEX_REGISTER_DATA_TABLE;


//
// externs
//

extern ULONG XgaSlot;
extern LONG BusNumber;

extern XGA_VIDEO_MODE XgaModes[];

extern UCHAR colour_default_palette[];

extern ULONG XgaNumModes;

#define XGA_MAX_MODES 100


extern UCHAR BankSwitchStart;
extern UCHAR BankSwitchEnd;
extern PULONG ApertureIndexRegister;

//
// Inventory of every XGA/XGA-2 adapter discovered in the system during
// VideoPortInitialize()'s enumeration pass (XgaFindAdapter is called once
// per MCA slot that holds a recognized adapter, via *Again). We record
// each one's board type and detected VRAM size here so that, once all
// adapters have been found, XgaInitialize() can compute the set of modes
// that *every* adapter in the system is capable of - i.e. the weakest
// adapter (for example an onboard 512K XGA-1 on a model 8590) limits the
// modes offered on a more capable XGA-2 add-in card, matching the
// behavior of IBM's MXGA driver for OS/2.
//
// NOTE: as of the dual-card (extended desktop) support, this inventory is
// still populated exactly as before and is still used the same way by
// XgaInitialize() for its system-wide lowest-common-denominator mode
// computation. It is independent of (and still fully compatible with)
// the Card2 merge logic described below.
//

#define XGA_MAX_ADAPTERS 8

typedef struct _XGA_ADAPTER_INVENTORY_ENTRY {
    UCHAR BoardType;
    ULONG FrameBufferLength;
} XGA_ADAPTER_INVENTORY_ENTRY, *PXGA_ADAPTER_INVENTORY_ENTRY;

extern XGA_ADAPTER_INVENTORY_ENTRY XgaAdapterInventory[XGA_MAX_ADAPTERS];
extern ULONG XgaAdapterInventoryCount;

//
// the device's private Device Extension
//

typedef struct _HW_DEVICE_EXTENSION {
    PHYSICAL_ADDRESS PhysicalRomBaseAddress;
    PHYSICAL_ADDRESS PhysicalCoProcessorAddress;
    PHYSICAL_ADDRESS PhysicalVideoMemoryAddress;
    PHYSICAL_ADDRESS PhysicalIoRegBaseAddress;

    UCHAR BoardType;
    UCHAR Color;
    UCHAR NumAvailableModes;
    UCHAR CurrentMode;

    ULONG IoRegBaseAddress;

    ULONG FrameBufferLength;
    ULONG PhysicalVideoMemoryLength;

    PVOID A0000MemoryAddress;
    PVOID PassThroughPort;

    UCHAR Valid[XGA_MAX_MODES];

    //
    // Dual-card (extended desktop) support.
    //
    // NumCards is 1 for a normal single-adapter system (all of the
    // Card2-prefixed fields below are then unused/zero). It becomes 2
    // only when XgaFindAdapter has located exactly two MCA XGA/XGA-2
    // adapters in the system and merged them into this single device
    // extension so they can be presented to xgadll as one extended
    // desktop (card 0 = left/primary monitor, card 1 = right/secondary
    // monitor). All resources for the second card are tracked
    // independently from the first card's (already existing, unrenamed)
    // fields above, so single-card code paths are entirely unaffected.
    //

    ULONG NumCards;

    PHYSICAL_ADDRESS Card2PhysicalRomBaseAddress;
    PHYSICAL_ADDRESS Card2PhysicalCoProcessorAddress;
    PHYSICAL_ADDRESS Card2PhysicalVideoMemoryAddress;
    PHYSICAL_ADDRESS Card2PhysicalIoRegBaseAddress;

    UCHAR Card2BoardType;

    ULONG Card2IoRegBaseAddress;

    ULONG Card2FrameBufferLength;
    ULONG Card2PhysicalVideoMemoryLength;

    PVOID Card2A0000MemoryAddress;

} HW_DEVICE_EXTENSION, *PHW_DEVICE_EXTENSION;