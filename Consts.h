#pragma once
#ifndef GCONSTS_H
#define GCONSTS_H


static bool USE_SRGB = false;
static bool ENABLE_MIP = false;
static bool PIXELATED = true;
static bool VSYNC = true;

const int MAX_TEXTURES_SUPPORTED = 200;//match in shader
const bool PALETTE_SWAP = false;
//lighter to darker
const unsigned char COLOUR_SWAP_1[3] = { 0xFF, 0xFF, 0xFF }; 
const unsigned char COLOUR_SWAP_2[3] = { 0xAA, 0xAA, 0xAA };
const unsigned char COLOUR_SWAP_3[3] = { 0x88, 0x88, 0x88 };
const unsigned char COLOUR_SWAP_4[3] = { 0x11, 0x11, 0x11 };


static int TARGET_WIDTH = 160;
static int TARGET_HEIGHT = 144;
static bool FIXED_RATIO = true;


#endif