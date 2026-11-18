
/******************************************************************************
 *
 *  XGA IOCTLs -
 *
 *****************************************************************************/

#include <devioctl.h>

#define IOCTL_VIDEO_XGA_MAP_COPROCESSOR \
	CTL_CODE (FILE_DEVICE_VIDEO, 2048, METHOD_BUFFERED, FILE_ANY_ACCESS)


typedef struct _VIDEO_XGA_COPROCESSOR_INFORMATION {
    PVOID CoProcessorVirtualAddress;
    PVOID PhysicalVideoMemoryAddress;
    ULONG XgaIoRegisterBaseAddress ;
} VIDEO_XGA_COPROCESSOR_INFORMATION, *PVIDEO_XGA_COPROCESSOR_INFORMATION;

//
// Dual-card (side by side extended desktop) support.
//
// When the miniport has merged two physical MCA XGA/XGA-2 adapters into a
// single logical device, xgadll addresses the second card's coprocessor
// and video memory by passing a VIDEO_XGA_CARD_REQUEST input buffer
// specifying which card (0 or 1) it wants. If no input buffer is supplied
// the miniport defaults to CardIndex 0 - this keeps single-card systems
// and any caller that does not yet know about dual-card support working
// exactly as before.
//

typedef struct _VIDEO_XGA_CARD_REQUEST {
    ULONG CardIndex;    // 0 = primary (left) card, 1 = secondary (right) card
} VIDEO_XGA_CARD_REQUEST, *PVIDEO_XGA_CARD_REQUEST;

