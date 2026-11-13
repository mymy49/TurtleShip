/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_ColorRgb565BE.cpp
 * @brief Implementation of the ColorRgb565BE class for 16-bit Big-Endian 5-6-5 color encoding.
 */

#include "ColorRgb565BE.h"

/**
 * @details Default constructor. Base class `Color::Color()` initializes the RGBA fields.
 */
ColorRgb565BE::ColorRgb565BE(void)
{

}

/**
 * @details Stores the 8-bit `red`, `green`, `blue`, and `alpha` values and packs them into a
 * 2-byte array `code[2]` in Big-Endian byte order:
 * - `code[0]` holds `(mBlue & 0xF8) | (mGreen >> 5)`
 * - `code[1]` holds `((mGreen & 0xFC) << 3) | (mRed >> 3)`
 *
 * The resulting 16-bit word is stored in `mCode`.
 * @see Color::getCode()
 */
void  ColorRgb565BE::setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint8_t code[2];

	mRed = red;
	mGreen = green;
	mBlue = blue;
	mAlpha = alpha;

	// Pack low byte into byte index 1 for big-endian layout
	code [1] = (mGreen & 0xFC) << 3;
	code [1] |= mRed >> 3;
	// Pack high byte into byte index 0 for big-endian layout
	code [0] = mBlue & 0xF8;
	code [0] |= mGreen >> 5;

	mCode = *(uint16_t*)code;
}
