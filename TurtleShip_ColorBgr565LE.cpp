/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_ColorBgr565LE.cpp
 * @brief Implementation of the ColorBgr565LE class for 16-bit Little-Endian BGR565 color encoding.
 */

#include "ColorBgr565LE.h"

/**
 * @details Default constructor. Base class `Color::Color()` initializes the RGBA fields.
 */
ColorBgr565LE::ColorBgr565LE(void)
{

}

/**
 * @details Stores the 8-bit `red`, `green`, `blue`, and `alpha` values and packs the top 5 bits of blue,
 * top 6 bits of green, and top 5 bits of red into a 2-byte array `code[2]` in Little-Endian byte order:
 * - `code[0]` holds `((mGreen & 0xFC) << 3) | (mRed >> 3)`
 * - `code[1]` holds `(mBlue & 0xF8) | (mGreen >> 5)`
 *
 * The resulting 16-bit word is stored in `mCode`.
 * @see Color::getCode()
 */
void  ColorBgr565LE::setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint8_t code[2];

	mRed = red;
	mGreen = green;
	mBlue = blue;
	mAlpha = alpha;

	// Pack low byte (green[4:2] and red[7:3]) into byte index 0 for little-endian BGR565 layout
	code [0] = (mGreen & 0xFC) << 3;
	code [0] |= mRed >> 3;
	// Pack high byte (blue[7:3] and green[7:5]) into byte index 1 for little-endian BGR565 layout
	code [1] = mBlue & 0xF8;
	code [1] |= mGreen >> 5;

	mCode = *(uint16_t*)code;
}
