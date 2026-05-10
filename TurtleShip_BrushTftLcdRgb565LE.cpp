/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "BrushTftLcdRgb565LE.h"
#include "ColorRgb565LE.h"
#include <config.h>
#include <std_ext/malloc.h>
#include <std_ext/string.h>

BrushTftLcdRgb565LE::BrushTftLcdRgb565LE(void)
{
	mFb = nullptr;
}

BrushTftLcdRgb565LE::~BrushTftLcdRgb565LE(void)
{

}

void BrushTftLcdRgb565LE::setFrameBuffer(FrameBufferRgb565LE &obj)
{
	mFb = &obj;
}

void BrushTftLcdRgb565LE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

Color BrushTftLcdRgb565LE::getBrushColor(void)
{
	return mBrushColor;
}

void BrushTftLcdRgb565LE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

Color BrushTftLcdRgb565LE::getBackgroundColor(void)
{
	return mBgColor;
}

