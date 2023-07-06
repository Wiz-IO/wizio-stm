#ifndef VIDEO_ROM_8M_H
#define VIDEO_ROM_8M_H

// tools/VIDEO_ROM_8M.bin

// reversed
#define CHARSET_8M_SIZE 2048

const unsigned char CHARSET_8M[CHARSET_8M_SIZE] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0x00, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x00, 0x00, 0x78, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 
0x44, 0x44, 0x7C, 0x44, 0x44, 0x28, 0x10, 0x00, 0x78, 0x40, 0x40, 0x70, 0x40, 0x40, 0x78, 0x00, 
0x00, 0x10, 0x08, 0x7C, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60, 0x30, 0x10, 0x10, 
0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x10, 0x10, 0x10, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x30, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x10, 0x10, 0x54, 0x38, 0x10, 0x7C, 0x00, 
0x00, 0x04, 0x48, 0x50, 0x60, 0x78, 0x00, 0x00, 0x28, 0x00, 0x38, 0x44, 0x7C, 0x04, 0x78, 0x00, 
0x00, 0x00, 0x00, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10, 
0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0x10, 0x10, 0x10, 
0x10, 0x10, 0x10, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x20, 0x7F, 0x20, 0x10, 0x00, 0x00, 
0x00, 0x78, 0x60, 0x50, 0x48, 0x04, 0x00, 0x00, 0x00, 0x40, 0x24, 0x14, 0x0C, 0x3C, 0x00, 0x00, 
0x00, 0x3C, 0x0C, 0x14, 0x24, 0x40, 0x00, 0x00, 0x10, 0x38, 0x54, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x10, 0x10, 0x10, 0x10, 0x54, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x10, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x10, 0x10, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x58, 0x24, 0x24, 0x58, 0x00, 0x00, 
0x00, 0x1C, 0x24, 0x34, 0x44, 0x34, 0x04, 0x04, 0x00, 0x44, 0x28, 0x10, 0x28, 0x28, 0x28, 0x10, 
0x00, 0x30, 0x48, 0x10, 0x48, 0x30, 0x00, 0x00, 0x04, 0x08, 0x10, 0x28, 0x44, 0x44, 0x00, 0x00, 
0x00, 0x24, 0x24, 0x24, 0x2C, 0x54, 0x04, 0x04, 0x00, 0x7C, 0x28, 0x28, 0x28, 0x24, 0x00, 0x00, 
0x00, 0x30, 0x48, 0x48, 0x38, 0x08, 0x08, 0x04, 0x00, 0x78, 0x14, 0x10, 0x10, 0x20, 0x00, 0x00, 
0x00, 0x24, 0x54, 0x54, 0x54, 0x38, 0x10, 0x10, 0x00, 0x44, 0x54, 0x54, 0x54, 0x38, 0x10, 0x10, 
0x00, 0x28, 0x44, 0x44, 0x54, 0x28, 0x00, 0x00, 0x7C, 0x04, 0x08, 0x10, 0x08, 0x04, 0x7C, 0x00, 
0x00, 0x40, 0x60, 0x50, 0x48, 0x44, 0x7C, 0x00, 0x00, 0x10, 0x00, 0x7C, 0x00, 0x10, 0x00, 0x00, 
0x18, 0x24, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x08, 0x0C, 0x08, 0x08, 0x1C, 0x00, 0x00, 0x00, 
0x18, 0x24, 0x10, 0x08, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x24, 0x10, 0x24, 0x18, 0x00, 0x00, 0x00, 
0x30, 0x28, 0x24, 0x3C, 0x20, 0x00, 0x00, 0x00, 0x3C, 0x04, 0x1C, 0x20, 0x1C, 0x00, 0x00, 0x00, 
0x38, 0x04, 0x1C, 0x24, 0x18, 0x00, 0x00, 0x00, 0x3C, 0x20, 0x10, 0x08, 0x08, 0x00, 0x00, 0x00, 
0x18, 0x24, 0x18, 0x24, 0x18, 0x00, 0x00, 0x00, 0x18, 0x24, 0x38, 0x20, 0x1C, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x00, 
0x20, 0x10, 0x08, 0x10, 0x20, 0x08, 0x10, 0x20, 0x00, 0x20, 0x7C, 0x10, 0x7C, 0x08, 0x00, 0x00, 
0x08, 0x10, 0x20, 0x10, 0x08, 0x20, 0x10, 0x08, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x7C, 0x00, 
0x78, 0x08, 0x7C, 0x44, 0x7C, 0x20, 0x3C, 0x00, 0x00, 0x00, 0x38, 0x24, 0x24, 0x24, 0x58, 0x00, 
0x04, 0x04, 0x34, 0x4C, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x78, 0x44, 0x04, 0x04, 0x78, 0x00, 
0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x78, 0x00, 0x00, 0x00, 0x30, 0x48, 0x28, 0x18, 0x70, 0x00, 
0x30, 0x48, 0x08, 0x3C, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x38, 
0x04, 0x04, 0x3C, 0x44, 0x44, 0x44, 0x44, 0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 
0x20, 0x00, 0x30, 0x20, 0x20, 0x20, 0x24, 0x18, 0x04, 0x04, 0x44, 0x24, 0x1C, 0x24, 0x44, 0x00, 
0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x2C, 0x54, 0x54, 0x54, 0x54, 0x00, 
0x00, 0x00, 0x34, 0x4C, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x00, 0x00, 0x3C, 0x44, 0x44, 0x4C, 0x34, 0x04, 0x00, 0x00, 0x78, 0x44, 0x44, 0x64, 0x58, 0x40, 
0x00, 0x00, 0x34, 0x4C, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x78, 0x04, 0x38, 0x40, 0x3C, 0x00, 
0x08, 0x08, 0x3C, 0x08, 0x08, 0x48, 0x30, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x64, 0x58, 0x00, 
0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 0x00, 0x00, 0x44, 0x54, 0x54, 0x54, 0x28, 0x00, 
0x00, 0x00, 0x6C, 0x10, 0x10, 0x54, 0x28, 0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x10, 0x08, 0x04, 
0x00, 0x00, 0x7C, 0x20, 0x10, 0x08, 0x7C, 0x00, 0x70, 0x18, 0x18, 0x0C, 0x18, 0x18, 0x70, 0x00, 
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1C, 0x30, 0x30, 0x60, 0x30, 0x30, 0x1C, 0x00, 
0x00, 0x08, 0x54, 0x20, 0x00, 0x7C, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0x00, 0x00, 0x64, 0x94, 0x9C, 0x94, 0x64, 0x00, 0x00, 0x00, 0x18, 0x20, 0x38, 0x24, 0x58, 0x00, 
0x40, 0x38, 0x04, 0x34, 0x4C, 0x44, 0x38, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0xFC, 0x80, 
0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x38, 0x44, 0x7C, 0x04, 0x78, 0x00, 
0x00, 0x10, 0x38, 0x54, 0x54, 0x54, 0x38, 0x10, 0x00, 0x00, 0x3C, 0x24, 0x04, 0x04, 0x04, 0x00, 
0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 
0x00, 0x10, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, 0x00, 0x44, 0x24, 0x1C, 0x24, 0x44, 0x00, 
0x00, 0x00, 0x60, 0x50, 0x48, 0x48, 0x44, 0x00, 0x00, 0x00, 0x44, 0x6C, 0x54, 0x44, 0x44, 0x00, 
0x00, 0x00, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x24, 0x18, 0x00, 
0x00, 0x00, 0x7C, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x78, 0x44, 0x78, 0x50, 0x4C, 0x00, 
0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x00, 0x00, 0x78, 0x04, 0x04, 0x04, 0x78, 0x00, 
0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x78, 0x40, 0x38, 
0x00, 0x00, 0x54, 0x54, 0x38, 0x54, 0x54, 0x00, 0x08, 0x14, 0x14, 0x34, 0x4C, 0x44, 0x3C, 0x00, 
0x00, 0x00, 0x04, 0x34, 0x4C, 0x44, 0x38, 0x00, 0x00, 0x00, 0x84, 0x94, 0xAC, 0xA4, 0x98, 0x00, 
0x00, 0x00, 0x38, 0x44, 0x30, 0x40, 0x3C, 0x00, 0x00, 0x00, 0x54, 0x54, 0x54, 0x54, 0x7C, 0x00, 
0x00, 0x00, 0x3C, 0x40, 0x78, 0x40, 0x3C, 0x00, 0x00, 0x00, 0x54, 0x54, 0x54, 0x54, 0xFC, 0x80, 
0x00, 0x00, 0x44, 0x44, 0x7C, 0x40, 0x40, 0x00, 0x00, 0x00, 0x0C, 0x28, 0x58, 0x48, 0x30, 0x00, 
0x38, 0x44, 0x54, 0x74, 0x34, 0x04, 0x78, 0x00, 0xE0, 0x50, 0x48, 0x48, 0x78, 0x48, 0xCC, 0x00, 
0x3C, 0x48, 0x48, 0x38, 0x48, 0x48, 0x3C, 0x00, 0x78, 0x44, 0x04, 0x04, 0x04, 0x44, 0x78, 0x00, 
0x3C, 0x48, 0x48, 0x48, 0x48, 0x48, 0x3C, 0x00, 0x7C, 0x08, 0x08, 0x38, 0x08, 0x08, 0x7C, 0x00, 
0x7C, 0x48, 0x08, 0x18, 0x08, 0x08, 0x1C, 0x00, 0x38, 0x44, 0x04, 0x04, 0x74, 0x44, 0x38, 0x00, 
0xCC, 0x48, 0x48, 0x78, 0x48, 0x48, 0xCC, 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x70, 0x20, 0x20, 0x20, 0x20, 0x24, 0x18, 0x00, 0x8C, 0x48, 0x28, 0x18, 0x28, 0x48, 0x8C, 0x00, 
0x1C, 0x08, 0x08, 0x08, 0x08, 0x48, 0x7C, 0x00, 0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x8C, 0x00, 
0x44, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x44, 0x00, 0x30, 0x48, 0x84, 0x84, 0x84, 0x48, 0x30, 0x00, 
0x7C, 0x88, 0x88, 0x78, 0x08, 0x08, 0x1C, 0x00, 0x38, 0x44, 0x44, 0x44, 0x54, 0x24, 0x58, 0x00, 
0x7C, 0x88, 0x88, 0x78, 0x28, 0x48, 0x8C, 0x00, 0x38, 0x44, 0x04, 0x38, 0x40, 0x44, 0x38, 0x00, 
0x7C, 0x54, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 0x44, 0x44, 0x44, 0x54, 0x54, 0x6C, 0x44, 0x00, 
0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x7C, 0x44, 0x20, 0x10, 0x08, 0x44, 0x7C, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 
0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 
0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x28, 0x00, 0x7C, 0x04, 0x3C, 0x04, 0x7C, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 
0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x7C, 0x28, 0x7C, 0x28, 0x28, 0x00, 
0x44, 0x38, 0x44, 0x44, 0x44, 0x38, 0x44, 0x00, 0x0C, 0x4C, 0x20, 0x10, 0x08, 0x64, 0x60, 0x00, 
0x08, 0x14, 0x14, 0x08, 0x54, 0x24, 0x58, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x08, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00, 0x10, 0x20, 0x40, 0x40, 0x40, 0x20, 0x10, 0x00, 
0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x10, 0x08, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 
0x38, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x38, 0x00, 0x10, 0x18, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x38, 0x44, 0x40, 0x30, 0x08, 0x04, 0x7C, 0x00, 0x7C, 0x40, 0x20, 0x30, 0x40, 0x44, 0x38, 0x00, 
0x20, 0x30, 0x28, 0x24, 0x7C, 0x20, 0x20, 0x00, 0x7C, 0x04, 0x3C, 0x40, 0x40, 0x44, 0x38, 0x00, 
0x70, 0x08, 0x04, 0x3C, 0x44, 0x44, 0x38, 0x00, 0x7C, 0x40, 0x20, 0x10, 0x08, 0x08, 0x08, 0x00, 
0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00, 0x38, 0x44, 0x44, 0x78, 0x40, 0x20, 0x1C, 0x00, 
0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x10, 0x08, 
0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x00, 0x00, 
0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x00, 0x38, 0x44, 0x20, 0x10, 0x10, 0x00, 0x10, 0x00, 
0x38, 0x44, 0x54, 0x74, 0x34, 0x04, 0x78, 0x00, 0xE0, 0x50, 0x48, 0x48, 0x78, 0x48, 0xCC, 0x00, 
0x3C, 0x48, 0x48, 0x38, 0x48, 0x48, 0x3C, 0x00, 0x78, 0x44, 0x04, 0x04, 0x04, 0x44, 0x78, 0x00, 
0x3C, 0x48, 0x48, 0x48, 0x48, 0x48, 0x3C, 0x00, 0x7C, 0x08, 0x08, 0x38, 0x08, 0x08, 0x7C, 0x00, 
0x7C, 0x48, 0x08, 0x18, 0x08, 0x08, 0x1C, 0x00, 0x38, 0x44, 0x04, 0x04, 0x74, 0x44, 0x38, 0x00, 
0xCC, 0x48, 0x48, 0x78, 0x48, 0x48, 0xCC, 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 
0x70, 0x20, 0x20, 0x20, 0x20, 0x24, 0x18, 0x00, 0x8C, 0x48, 0x28, 0x18, 0x28, 0x48, 0x8C, 0x00, 
0x1C, 0x08, 0x08, 0x08, 0x08, 0x48, 0x7C, 0x00, 0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x8C, 0x00, 
0x44, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x44, 0x00, 0x30, 0x48, 0x84, 0x84, 0x84, 0x48, 0x30, 0x00, 
0x7C, 0x88, 0x88, 0x78, 0x08, 0x08, 0x1C, 0x00, 0x38, 0x44, 0x44, 0x44, 0x54, 0x24, 0x58, 0x00, 
0x7C, 0x88, 0x88, 0x78, 0x28, 0x48, 0x8C, 0x00, 0x38, 0x44, 0x04, 0x38, 0x40, 0x44, 0x38, 0x00, 
0x7C, 0x54, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 0x44, 0x44, 0x44, 0x54, 0x54, 0x6C, 0x44, 0x00, 
0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 
0x7C, 0x44, 0x20, 0x10, 0x08, 0x44, 0x7C, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 
0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 
0x00, 0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0x64, 0x94, 0x94, 0x9C, 0x94, 0x94, 0x64, 0x00, 0x10, 0x28, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x00, 
0x7C, 0x04, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xFC, 0x80, 
0x70, 0x48, 0x48, 0x48, 0x48, 0x48, 0xFC, 0x84, 0x7C, 0x04, 0x04, 0x3C, 0x04, 0x04, 0x7C, 0x00, 
0x38, 0x54, 0x54, 0x54, 0x54, 0x38, 0x10, 0x00, 0x7C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 
0x44, 0x6C, 0x38, 0x10, 0x38, 0x6C, 0x44, 0x00, 0x44, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x44, 0x00, 
0xB4, 0x84, 0xC4, 0xA4, 0x94, 0x8C, 0x84, 0x00, 0x44, 0x24, 0x14, 0x0C, 0x14, 0x24, 0x44, 0x00, 
0x70, 0x48, 0x48, 0x48, 0x48, 0x48, 0x44, 0x00, 0x44, 0x6C, 0x54, 0x54, 0x44, 0x44, 0x44, 0x00, 
0x44, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x44, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 
0x7C, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44, 0x00, 
0x3C, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x00, 0x38, 0x44, 0x04, 0x04, 0x04, 0x44, 0x38, 0x00, 
0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x44, 0x44, 0x44, 0x78, 0x40, 0x44, 0x38, 0x00, 
0x54, 0x54, 0x54, 0x38, 0x54, 0x54, 0x54, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x44, 0x44, 0x3C, 0x00, 
0x04, 0x04, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x84, 0x84, 0x84, 0x9C, 0xA4, 0xA4, 0x9C, 0x00, 
0x38, 0x44, 0x40, 0x30, 0x40, 0x44, 0x38, 0x00, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x7C, 0x00, 
0x3C, 0x40, 0x40, 0x78, 0x40, 0x40, 0x3C, 0x00, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0xFC, 0x80, 
0x44, 0x44, 0x44, 0x44, 0x78, 0x40, 0x40, 0x00, 0x0C, 0x08, 0x08, 0x38, 0x48, 0x48, 0x38, 0x00, 
};

#endif
