/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_ColorBgr565BE.cpp
 * @brief Implementation of the ColorBgr565BE class for 16-bit Big-Endian color encoding.
 * @author Yoon-Ki Hong
 * @date 2025
 */

#include "ColorBgr565BE.h"

/**
 * @details Default constructor. Base class `Color::Color()` initializes the RGBA fields.
 * @see ColorBgr565BE
 * @todo None.
 * @bug None.
 */
ColorBgr565BE::ColorBgr565BE(void)
{

}

/**
 * @details Stores the 8-bit `red`, `green`, `blue`, and `alpha` values and packs the top 5 bits of red,
 * top 6 bits of green, and top 5 bits of blue into a 2-byte array `code[2]` in Big-Endian byte order:
 * - `code[0]` holds `(mRed & 0xF8) | (mGreen >> 5)`
 * - `code[1]` holds `((mGreen & 0xFC) << 3) | (mBlue >> 3)`
 *
 * The resulting 16-bit word is stored in `mCode`.
 * @see Color::getCode()
 * @todo None.
 * @bug None.
 */
void  ColorBgr565BE::setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint8_t code[2];

	mRed = red;
	mGreen = green;
	mBlue = blue;
	mAlpha = alpha;

	// Pack low byte (green[4:2] and blue[7:3]) into byte index 1 for big-endian layout
	code [1] = (mGreen & 0xFC) << 3;
	code [1] |= mBlue >> 3;
	// Pack high byte (red[7:3] and green[7:5]) into byte index 0 for big-endian layout
	code [0] = mRed & 0xF8;
	code [0] |= mGreen >> 5;

	mCode = *(uint16_t*)code;
}
