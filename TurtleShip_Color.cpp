/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Color.cpp
 * @brief Implementation of the base Color class methods.
 */

#include "Color.h"

/**
 * @details Initializes the color to opaque black (`R=0x00, G=0x00, B=0x00, A=0xFF`) by calling `setColorBase()`.
 * @see Color::setColorBase()
 */
Color::Color(void)
{
	setColorBase(0x00, 0x00, 0x00);
}

/**
 * @details Initializes the color with the specified `red`, `green`, `blue`, and `alpha` values
 * and encodes `mCode` via `setColorBase()`.
 * @see Color::setColorBase()
 */
Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	setColorBase(red, green, blue, alpha);
}

/**
 * @details Extracts `mRed`, `mGreen`, `mBlue`, and `mAlpha` from `obj` and passes them to
 * `setColorBase()` so `mCode` is updated using the dynamic type's pixel format encoding.
 * @see Color::setColorBase()
 */
void Color::setColor(Color obj)
{
	setColorBase(obj.mRed, obj.mGreen, obj.mBlue, obj.mAlpha);
}

/**
 * @details Forwards the `red`, `green`, `blue`, and `alpha` channel values to `setColorBase()`
 * to update both the stored channels and the cached `mCode`.
 * @see Color::setColorBase()
 */
void Color::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	setColorBase(red, green, blue, alpha);
}

/**
 * @details Copies the RGBA channel values from `obj` and recomputes `mCode` via `setColorBase()`.
 * @see Color::setColorBase()
 */
void Color::operator=(Color &obj)
{
	setColorBase(obj.mRed, obj.mGreen, obj.mBlue, obj.mAlpha);
}

/**
 * @details Saves the 8-bit `red`, `green`, `blue`, and `alpha` components into member variables
 * and packs them into `mCode` in 32-bit ARGB8888 layout:
 * `(alpha << 24) | (red << 16) | (green << 8) | blue`.
 */
void Color::setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mRed = red;
	mGreen = green;
	mBlue = blue;
	mAlpha = alpha;

	// Pack 8-bit channels into 32-bit ARGB8888 format (0xAARRGGBB)
	mCode = (uint32_t)mBlue | (uint32_t)mGreen << 8 | (uint32_t)mRed << 16 | (uint32_t)mAlpha << 24;
}

/**
 * @details Returns the cached hardware color code (`mCode`) computed by the most recent call to `setColorBase()`.
 */
uint32_t Color::getCode(void)
{
	return mCode;
}
