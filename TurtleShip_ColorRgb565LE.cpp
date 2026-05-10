/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "ColorRgb565LE.h"

ColorRgb565LE::ColorRgb565LE(void)
{

}

void  ColorRgb565LE::setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint8_t code[2];

	mRed = red;
	mGreen = green;
	mBlue = blue;
	mAlapha = alpha;

	code [0] = (mGreen & 0xFC) << 3;
	code [0] |= mBlue >> 3;
	code [1] = mRed & 0xF8;
	code [1] |= mGreen >> 5;

	mCode = *(uint16_t*)code;
}
