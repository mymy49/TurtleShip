/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "Color.h"

Color::Color(void)
{
	setColorBase(0x00, 0x00, 0x00);
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	setColorBase(red, green, blue, alpha);
}

void Color::setColor(Color obj)
{
	setColorBase(obj.mRed, obj.mGreen, obj.mBlue, obj.mAlapha);
}

void Color::operator=(Color &obj)
{
	setColorBase(obj.mRed, obj.mGreen, obj.mBlue, obj.mAlapha);
}

void Color::setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mRed = red;
	mGreen = green;
	mBlue = blue;
	mAlapha = alpha;

	mCode = (uint32_t)mBlue | (uint32_t)mGreen << 8 | (uint32_t)mRed << 16 | (uint32_t)mAlapha << 24;
}

uint32_t Color::getCode(void)
{
	return mCode;
}
