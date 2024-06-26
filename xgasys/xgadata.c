/*++

Copyright (c) 1991-1992  Microsoft Corporation

Module Name:

    xga.h

Abstract:

    This module contains the global structures and definitions used
    by the xga driver.

Environment:

    Kernel mode

Revision History:


--*/

#include "devioctl.h"
#include "miniport.h"

#include "ntddvdeo.h"
#include "video.h"
#include "xga.h"

#if defined(ALLOC_PRAGMA)
#pragma data_seg("PAGE")
#endif

//
// Last slot at which an adapter was found.
//

ULONG XgaSlot = 0;
LONG BusNumber = -1;

//
// This variable determines if we are going to run the XGA with the
// coprocessor (FALSE) or jsut as a banked frame buffer (TRUE);
//

BOOLEAN framebufMode = FALSE;


//
// Mode set tables
//
MODE_REGISTER_DATA_TABLE XGA2_MODE_640x480x65536x73Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x67,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x67,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x5A,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x07,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x07,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xE8,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEB,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xA0,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x58, 0x3D,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_MODE_640x480x65536x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x54,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x60,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x0C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x0C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xEA,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEC,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xA0,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x58, 0x24,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_MODE_800x600x65536x72Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x82,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x82,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x6A,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x79,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x6C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x6C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x57,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x5A,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xC8,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x5D,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_MODE_800x600x65536x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x83,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x83,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x6B,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x7B,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x73,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x73,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x58,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x5C,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xC8,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x4F,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};




MODE_REGISTER_DATA_TABLE XGA2_MODE_800x600x65536x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x82,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x82,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x6A,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x79,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x6C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x6C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x57,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x5A,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xC8,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x59,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1024x768x65536x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA5,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA5,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x99,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x00,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x80,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1024x768x65536x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA5,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA5,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x99,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x00,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x8A,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};
//stop
MODE_REGISTER_DATA_TABLE XGA2_1104x828x65536x72Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xB2,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x89,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x89,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xB2,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x90,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAB,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x58,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x3B,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x3B,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x58,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x3B,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x42,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x14,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x99,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1104x828x65536x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xB2,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x89,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x89,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xB2,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x90,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAB,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x58,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x3B,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x3B,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x58,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x3B,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x42,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x14,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x9C,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1104x828x65536x78Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xB2,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x89,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x89,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xB2,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x90,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAB,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x58,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x3B,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x3B,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x58,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x3B,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x42,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x14,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x9C,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1120x840x65536x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xB4,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x8B,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x89,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xB4,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x92,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAD,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x65,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x47,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x47,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x65,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x47,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x4E,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x18,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x9D,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1160x870x65536x65Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xBA,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x90,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x90,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xBA,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x97,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xB3,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x83,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x65,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x65,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x83,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x65,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x6C,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x22,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x99,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1160x870x65536x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xBA,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x90,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x90,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xBA,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x97,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xB3,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x83,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x65,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x65,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x83,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x65,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x6C,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x22,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x9D,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA2_1280x800x65536x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xCC,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x9F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x9F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xCC,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0xA6,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xC3,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x39,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x1F,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x1F,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x39,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x1F,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x26,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x40,
INDEX_REG,        0x44, 0x01,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0xA5,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};
//done

MODE_REGISTER_DATA_TABLE XGA2_MODE_800x600x65536x77Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x82,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x82,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x6A,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x79,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x6C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x6C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x57,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x5A,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xC8,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x58, 0x63,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};




// End of 65536 XGA2


//
//  XGA 1 mode tables
//

MODE_REGISTER_DATA_TABLE XGA1_MODE_640_480_64_GR[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0x00,
OP_MODE_REG,      0x00, 0x00,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x00,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x00,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x00,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x00,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x00,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x00,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x00,
INDEX_REG,        0x21, 0x00,
INDEX_REG,        0x22, 0x00,
INDEX_REG,        0x23, 0x00,
INDEX_REG,        0x24, 0x00,
INDEX_REG,        0x25, 0x00,
INDEX_REG,        0x26, 0x00,
INDEX_REG,        0x27, 0x00,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x00,
INDEX_REG,        0x2A, 0x00,
INDEX_REG,        0x2C, 0x00,
INDEX_REG,        0x2D, 0x00,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x00,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0x00,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA1_MODE_640_480_256_CO[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x61,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x0C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x0C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xEA,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEC,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA1_MODE_640_480_65536_CO[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x04,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x61,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x0C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x0C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xEA,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEC,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xA0,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x04,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA1_MODE_1024_768_16_GR[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0x00,
OP_MODE_REG,      0x00, 0x00,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x00,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x00,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x00,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x00,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x00,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x00,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x00,
INDEX_REG,        0x21, 0x00,
INDEX_REG,        0x22, 0x00,
INDEX_REG,        0x23, 0x00,
INDEX_REG,        0x24, 0x00,
INDEX_REG,        0x25, 0x00,
INDEX_REG,        0x26, 0x00,
INDEX_REG,        0x27, 0x00,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x00,
INDEX_REG,        0x2A, 0x00,
INDEX_REG,        0x2C, 0x00,
INDEX_REG,        0x2D, 0x00,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x00,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0x00,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA1_MODE_1024_768_16_CO[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x02,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x9D,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x9D,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x87,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9C,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x40,
INDEX_REG,        0x1E, 0x04,
INDEX_REG,        0x20, 0x30,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x30,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x40,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x0D,
INDEX_REG,        0x51, 0x02,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0x0F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA1_MODE_1024_768_64_GR[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0x00,
OP_MODE_REG,      0x00, 0x00,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x00,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x00,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x00,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x00,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x00,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x00,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x00,
INDEX_REG,        0x21, 0x00,
INDEX_REG,        0x22, 0x00,
INDEX_REG,        0x23, 0x00,
INDEX_REG,        0x24, 0x00,
INDEX_REG,        0x25, 0x00,
INDEX_REG,        0x26, 0x00,
INDEX_REG,        0x27, 0x00,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x00,
INDEX_REG,        0x2A, 0x00,
INDEX_REG,        0x2C, 0x00,
INDEX_REG,        0x2D, 0x00,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x00,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0x00,
END_OF_SWITCH
};

MODE_REGISTER_DATA_TABLE XGA1_MODE_1024_768_256_CO[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x9D,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x9D,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x87,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9C,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x40,
INDEX_REG,        0x1E, 0x04,
INDEX_REG,        0x20, 0x30,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x30,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x0D,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x50, 0x0F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

//
// XGA 2 mode tables
//

MODE_REGISTER_DATA_TABLE XGA2_MODE_1024_768_256_CO[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA7,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA7,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x99,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x58, 0x80,
INDEX_REG,     0x5B, 0x02,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

//
// New IBM modes
//

//
// XGA 1 modes
//

MODE_REGISTER_DATA_TABLE XGA1_640x480x8x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x61,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x0C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x0C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xEA,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEC,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA1_640x400x8x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x61,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0xC0,
INDEX_REG,        0x21, 0x01,
INDEX_REG,        0x22, 0x8F,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0x8F,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0xC0,
INDEX_REG,        0x27, 0x01,
INDEX_REG,        0x28, 0x9C,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0x9E,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x54, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x50, 0x47,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA1_1024x768x8x43Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x9D,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x9D,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x86,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9C,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x40,
INDEX_REG,        0x1E, 0x04,
INDEX_REG,        0x20, 0x30,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x30,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x54, 0x0D,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x50, 0x0F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA1_1040x768x8x43Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x9D,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x9D,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x87,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9D,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x30,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x30,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x54, 0x0D,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x50, 0x0F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};

//
// XGA 2 mode tables
//

MODE_REGISTER_DATA_TABLE XGA2_640x480x8x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x54,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x60,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x0C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x0C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xEA,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEC,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x24,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_640x480x8x72Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x67,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x67,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x5A,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x07,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x07,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xE8,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEB,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x3D,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_640x480x8x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x54,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x60,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x0C,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0xDF,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0xDF,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0x0C,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0xEA,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0xEC,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x3D,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_640x400x8x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x54,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x60,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0xC0,
INDEX_REG,        0x21, 0x01,
INDEX_REG,        0x22, 0x8F,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0x8F,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0xC0,
INDEX_REG,        0x27, 0x01,
INDEX_REG,        0x28, 0x9C,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0x9E,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x24,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0x47,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_640x400x8x84Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x67,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x67,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x55,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x5A,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0xC0,
INDEX_REG,        0x21, 0x01,
INDEX_REG,        0x22, 0x8F,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0x8F,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0xC0,
INDEX_REG,        0x27, 0x01,
INDEX_REG,        0x28, 0x9C,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0x9E,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x3D,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0x47,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_640x400x8x88Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x63,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x4F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x4F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x63,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x54,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x60,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0xC0,
INDEX_REG,        0x21, 0x01,
INDEX_REG,        0x22, 0x8F,
INDEX_REG,        0x23, 0x01,
INDEX_REG,        0x24, 0x8F,
INDEX_REG,        0x25, 0x01,
INDEX_REG,        0x26, 0xC0,
INDEX_REG,        0x27, 0x01,
INDEX_REG,        0x28, 0x9C,
INDEX_REG,        0x29, 0x01,
INDEX_REG,        0x2A, 0x9E,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x50,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x3D,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0x47,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_800x600x8x56Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x7F,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x7F,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x69,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x72,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x70,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x70,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x58,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x5A,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x64,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x47,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_800x600x8x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x83,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x83,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x6B,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x7B,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x73,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x73,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x58,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x5C,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x64,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x4F,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_800x600x8x72Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x81,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x81,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x70,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x7F,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x99,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x99,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x7C,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x82,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x64,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x63,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_800x600x8x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x81,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x63,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x63,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x81,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x70,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x7F,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x99,
INDEX_REG,        0x21, 0x02,
INDEX_REG,        0x22, 0x57,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0x57,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x99,
INDEX_REG,        0x27, 0x02,
INDEX_REG,        0x28, 0x7C,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x82,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x64,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x67,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1024x768x8x43Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x9D,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x9D,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x83,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x99,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x20,
INDEX_REG,        0x1E, 0x02,
INDEX_REG,        0x20, 0x30,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x30,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x59,
INDEX_REG,        0x54, 0x80,
INDEX_REG,        0x50, 0x0F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1024x768x8x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA7,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA7,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x99,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x80,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1024x768x8x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA5,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA5,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x99,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x8A,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1024x768x8x70HzxIBM[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xaa,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xaa,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x86,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xa9,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x2e,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x2e,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0xff,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x07,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x8D,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1024x768x8x72Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA9,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA9,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x86,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xA9,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x26,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x26,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0xFF,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x07,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x8E,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1024x768x8x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xAF,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x7F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x7F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xAF,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x86,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAE,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0xFF,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x07,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x80,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x95,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1040x768x8x43Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0x9F,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0x9F,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9E,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x30,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x30,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x00,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x5A,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x0F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1040x768x8x60Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xAA,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xAA,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x8A,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9B,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x81,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1040x768x8x70Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xA8,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xA8,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x8A,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0x9B,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x08,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x8B,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0xC7,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1040x768x8x70HzDBCS[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xAD,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xAD,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAB,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x2E,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x2E,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0xFF,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x07,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x8E,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1040x768x8x72Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xAE,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xAE,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x89,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAD,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x26,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x26,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0xFF,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x07,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x90,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1040x768x8x75Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xB2,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x81,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x81,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xB2,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x88,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xB0,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x25,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x02,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x02,
INDEX_REG,        0x26, 0x25,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0xFF,
INDEX_REG,        0x29, 0x02,
INDEX_REG,        0x2A, 0x07,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x82,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x96,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x07,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1104x828x8x69Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xBD,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x89,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x89,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xBD,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0x94,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xAD,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x5B,
INDEX_REG,        0x21, 0x03,
INDEX_REG,        0x22, 0x3B,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0x3B,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x5B,
INDEX_REG,        0x27, 0x03,
INDEX_REG,        0x28, 0x3E,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0x41,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0x8A,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x99,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x87,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1280x1024x8x45Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xD1,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x9F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x9F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xD1,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0xA7,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xCF,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x44,
INDEX_REG,        0x21, 0x04,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x44,
INDEX_REG,        0x27, 0x04,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x04,
INDEX_REG,        0x2A, 0x0A,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xA0,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x91,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x4F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1280x960x8x45Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xD1,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0x9F,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0x9F,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xD1,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0xA7,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xCF,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x04,
INDEX_REG,        0x21, 0x04,
INDEX_REG,        0x22, 0xBF,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0xBF,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x04,
INDEX_REG,        0x27, 0x04,
INDEX_REG,        0x28, 0xC2,
INDEX_REG,        0x29, 0x03,
INDEX_REG,        0x2A, 0xCA,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xA0,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x8C,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x4F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


MODE_REGISTER_DATA_TABLE XGA2_1360x1024x8x45Hz[] = {

INT_ENABLE_REG,   0x00, 0x00,
INT_STATUS_REG,   0x00, 0xFF,
OP_MODE_REG,      0x00, 0x04,
INDEX_REG,        0x64, 0x00,
APP_CTL_REG,      0x00, 0x00,
APP_INDEX_REG,    0x00, 0x00,
VMEM_CONTROL_REG, 0x00, 0x00,
MEMACC_MODE_REG,  0x00, 0x03,
INDEX_REG,        0x50, 0x01,
INDEX_REG,        0x50, 0x00,
INDEX_REG,        0x10, 0xDB,
INDEX_REG,        0x11, 0x00,
INDEX_REG,        0x12, 0xA9,
INDEX_REG,        0x13, 0x00,
INDEX_REG,        0x14, 0xA9,
INDEX_REG,        0x15, 0x00,
INDEX_REG,        0x16, 0xDB,
INDEX_REG,        0x17, 0x00,
INDEX_REG,        0x18, 0xB1,
INDEX_REG,        0x19, 0x00,
INDEX_REG,        0x1A, 0xD9,
INDEX_REG,        0x1B, 0x00,
INDEX_REG,        0x1C, 0x00,
INDEX_REG,        0x1E, 0x00,
INDEX_REG,        0x20, 0x40,
INDEX_REG,        0x21, 0x04,
INDEX_REG,        0x22, 0xFF,
INDEX_REG,        0x23, 0x03,
INDEX_REG,        0x24, 0xFF,
INDEX_REG,        0x25, 0x03,
INDEX_REG,        0x26, 0x40,
INDEX_REG,        0x27, 0x04,
INDEX_REG,        0x28, 0x02,
INDEX_REG,        0x29, 0x04,
INDEX_REG,        0x2A, 0x0A,
INDEX_REG,        0x2C, 0xFF,
INDEX_REG,        0x2D, 0xFF,
INDEX_REG,        0x36, 0x00,
INDEX_REG,        0x40, 0x00,
INDEX_REG,        0x41, 0x00,
INDEX_REG,        0x42, 0x00,
INDEX_REG,        0x43, 0xAA,
INDEX_REG,        0x44, 0x00,
INDEX_REG,        0x54, 0x01,
INDEX_REG,        0x70, 0x00,
INDEX_REG,        0x51, 0x03,
INDEX_REG,        0x58, 0x95,
INDEX_REG,        0x54, 0x81,
INDEX_REG,        0x50, 0x4F,
INDEX_REG,        0x55, 0x00,
INDEX_REG,        0x60, 0x00,
INDEX_REG,        0x61, 0x00,
INDEX_REG,        0x62, 0x00,
INDEX_REG,        0x63, 0x00,
INDEX_REG,        0x64, 0xFF,
END_OF_SWITCH
};


//
// Video mode table - Lists the information about each individual mode
//

XGA_VIDEO_MODE XgaModes[] = {
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_MODE_640x480x65536x60Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      640,                         // X Resolution, in pixels
      480,                         // Y Resolution, in pixels
      1280,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      60,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,                           // Number Blue pixels in DAC
      0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_MODE_640x480x65536x73Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      640,                         // X Resolution, in pixels
      480,                         // Y Resolution, in pixels
      1280,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      73,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,                           // Number Blue pixels in DAC
      0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_MODE_800x600x65536x72Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      800,                         // X Resolution, in pixels
      600,                         // Y Resolution, in pixels
      1600,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      72,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  
{
	
  NULL,         					// Mode setting table for XGA 1
  XGA2_MODE_800x600x65536x70Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      800,                         // X Resolution, in pixels
      600,                         // Y Resolution, in pixels
      1600,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      70,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  
{
	
  NULL,         					// Mode setting table for XGA 1
  XGA2_MODE_800x600x65536x60Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      800,                         // X Resolution, in pixels
      600,                         // Y Resolution, in pixels
      1600,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      60,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
}, 
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1024x768x65536x60Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1024,                         // X Resolution, in pixels
      768,                         // Y Resolution, in pixels
      2048,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      60,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   						 // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1024x768x65536x70Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1024,                         // X Resolution, in pixels
      768,                         // Y Resolution, in pixels
      2048,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      70,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   						 // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1104x828x65536x72Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1104,                         // X Resolution, in pixels
      828,                         // Y Resolution, in pixels
      2208,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      72,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   						 // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1104x828x65536x78Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1104,                         // X Resolution, in pixels
      828,                         // Y Resolution, in pixels
      2208,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      78,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,       					 // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1120x840x65536x75Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1120,                         // X Resolution, in pixels
      840,                         // Y Resolution, in pixels
      2240,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      75,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   						 // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1160x870x65536x65Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1160,                         // X Resolution, in pixels
      870,                         // Y Resolution, in pixels
      2320,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      65,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,                      		 // Mask for Blue Pixels in non-palette modes           
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
  XGA2_1160x870x65536x70Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1160,                         // X Resolution, in pixels
      870,                         // Y Resolution, in pixels
      2320,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      70,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,							 // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	//Added by Ryan Alswede
  NULL,         					// Mode setting table for XGA 1
   XGA2_1280x800x65536x75Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      1280,                         // X Resolution, in pixels
      800,                         // Y Resolution, in pixels
      2560,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      75,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,                          // Mask for Blue Pixels in non-palette modes
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
	
  NULL,         					// Mode setting table for XGA 1
  XGA2_MODE_800x600x65536x77Hz,     // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      800,                         // X Resolution, in pixels
      600,                         // Y Resolution, in pixels
      1600,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      16,                           // Number of bits per plane
      77,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      5,                           // Number Red pixels in DAC
      6,                           // Number Green pixels in DAC
      5,   
	  0xF800,                  // Mask for Red Pixels in non-palette modes
      0x07E0,                  // Mask for Green Pixels in non-palette modes
      0x001F,                  // Mask for Blue Pixels in non-palette modes                 
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS // Mode description flags.
    }
},  //end of 65536 for XGA2
{
  XGA1_640x480x8x60Hz,
  XGA2_640x480x8x60Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      480,
      640,
      1,
      8,
      60,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  XGA1_MODE_640_480_64_GR,         // Mode setting table for XGA 1
  NULL,                            // Mode setting table for XGA 2
    {
      sizeof(VIDEO_MODE_INFORMATION), // Size of the mode informtion structure
      0,                           // Mode index used in setting the mode
      640,                         // X Resolution, in pixels
      480,                         // Y Resolution, in pixels
      480,                         // Screen stride, in bytes (distance
                                   // between the start point of two
                                   // consecutive scan lines, in bytes)
      1,                           // Number of video memory planes
      6,                           // Number of bits per plane
      0,                           // Screen Frequency, in Hertz
      320,                         // Horizontal size of screen in millimeters
      240,                         // Vertical size of screen in millimeters
      8,                           // Number Red pixels in DAC
      8,                           // Number Green pixels in DAC
      8,                           // Number Blue pixels in DAC
      0x00000000,                  // Mask for Red Pixels in non-palette modes
      0x00000000,                  // Mask for Green Pixels in non-palette modes
      0x00000000,                  // Mask for Blue Pixels in non-palette modes
      VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE // Mode description flags.
    }
},
{
  XGA1_MODE_1024_768_16_GR,
  NULL,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      512,
      1,
      4,
      0,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  XGA1_MODE_1024_768_64_GR,
  NULL,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      768,
      1,
      6,
      0,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  XGA1_MODE_1024_768_16_CO,
  NULL,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      512,
      1,
      4,
      0,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  XGA1_640x400x8x70Hz,
  XGA2_640x400x8x70Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      400,
      640,
      1,
      8,
      70,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_640x400x8x84Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      400,
      640,
      1,
      8,
      84,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_640x400x8x88Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      400,
      640,
      1,
      8,
      88,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_640x480x8x72Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      480,
      640,
      1,
      8,
      72,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_640x480x8x75Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      480,
      640,
      1,
      8,
      75,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_800x600x8x56Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      800,
      600,
      800,
      1,
      8,
      56,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | 
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_800x600x8x60Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      800,
      600,
      800,
      1,
      8,
      60,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_800x600x8x72Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      800,
      600,
      800,
      1,
      8,
      72,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_800x600x8x75Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      800,
      600,
      800,
      1,
      8,
      75,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  XGA1_1024x768x8x43Hz,
  XGA2_1024x768x8x43Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      1024,
      1,
      8,
      43,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  NULL,
  XGA2_1024x768x8x60Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      1024,
      1,
      8,
      60,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1024x768x8x70Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      1024,
      1,
      8,
      70,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1024x768x8x72Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      1024,
      1,
      8,
      72,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1024x768x8x75Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1024,
      768,
      1024,
      1,
      8,
      75,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  XGA1_1040x768x8x43Hz,
  XGA2_1040x768x8x43Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1040,
      768,
      1040,
      1,
      8,
      43,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  NULL,
  XGA2_1040x768x8x60Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1040,
      768,
      1040,
      1,
      8,
      60,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1040x768x8x70Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1040,
      768,
      1040,
      1,
      8,
      70,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1040x768x8x72Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1040,
      768,
      1040,
      1,
      8,
      72,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1040x768x8x75Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1040,
      768,
      1040,
      1,
      8,
      75,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE                        
    }
},
{
  NULL,
  XGA2_1104x828x8x69Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1104,
      828,
      1104,
      1,
      8,
      69,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
},
{
  NULL,
  XGA2_1280x960x8x45Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1280,
      960,
      1280,
      1,
      8,
      45,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  NULL,
  XGA2_1280x1024x8x45Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1280,
      1024,
      1280,
      1,
      8,
      45,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  NULL,
  XGA2_1360x1024x8x45Hz,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      1360,
      1024,
      1360,
      1,
      8,
      45,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE | VIDEO_MODE_INTERLACED
    }
},
{
  XGA1_MODE_640_480_65536_CO,
  NULL,
    {
      sizeof(VIDEO_MODE_INFORMATION),
      0,
      640,
      480,
      1280,
      1,
      16,
      0,
      320,
      240,
      8,
      8,
      8,
      0x00000000,
      0x00000000,
      0x00000000,
      VIDEO_MODE_COLOR | VIDEO_MODE_GRAPHICS | VIDEO_MODE_PALETTE_DRIVEN |
        VIDEO_MODE_MANAGED_PALETTE
    }
}

};

ULONG XgaNumModes = (sizeof(XgaModes) / sizeof (XGA_VIDEO_MODE));


//
// Settings for the default color palette.
//

UCHAR colour_default_palette[] = {
    0x00, 0x00, 0x00,
    0x00, 0x00, 0xa8,
    0x00, 0xa8, 0x00,
    0x00, 0xA8, 0xA8,
    0xA8, 0x00, 0x00,
    0xA8, 0x00, 0xa8,
    0xA8, 0x54, 0x00,
    0xA8, 0xA8, 0xA8,
    0x54, 0x54, 0x54,
    0x54, 0x54, 0xfc,
    0x54, 0xfc, 0x54,
    0x54, 0xFC, 0xFC,
    0xFC, 0x54, 0x54,
    0xFC, 0x54, 0xFC,
    0xFC, 0xFC, 0x54,
    0xFC, 0xFC, 0xFC
};


#if defined(ALLOC_PRAGMA)
#pragma data_seg()
#endif
