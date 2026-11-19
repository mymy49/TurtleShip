/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_BrushTftLcdRgb565BE.cpp
 * @brief Implementation of the BrushTftLcdRgb565BE class for Big-Endian RGB565 TFT LCD brushes.
 * @author Yoon-Ki Hong
 * @date 2025
 */

#include "BrushTftLcdRgb565BE.h"
#include "ColorRgb565BE.h"
#include <config.h>
#include <yss/hmalloc.h>
#include <std_ext/string.h>

/**
 * @details Initializes the brush with no working frame buffer attached (`mFb = nullptr`).
 * @see BrushTftLcdRgb565BE
 * @todo None.
 * @bug None.
 */
BrushTftLcdRgb565BE::BrushTftLcdRgb565BE(void)
{
	mFb = nullptr;
}

/**
 * @details Virtual destructor for `BrushTftLcdRgb565BE`.
 * @see BrushTftLcdRgb565BE
 * @todo None.
 * @bug None.
 */
BrushTftLcdRgb565BE::~BrushTftLcdRgb565BE(void)
{

}

/**
 * @details Stores the address of `obj` into `mFb` so the LCD driver can use its allocated memory
 * for block transfers.
 * @see BrushTftLcdRgb565BE
 * @todo None.
 * @bug None.
 */
void BrushTftLcdRgb565BE::setFrameBuffer(FrameBufferRgb565BE &obj)
{
	mFb = &obj;
}

/**
 * @details Updates `mBrushColor` from `color` (which invokes `ColorRgb565BE::setColorBase()`)
 * and caches the resulting 16-bit Big-Endian color code in `mBrushColorCode`.
 * @see ColorRgb565BE::setColorBase()
 * @todo None.
 * @bug None.
 */
void BrushTftLcdRgb565BE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

/**
 * @details Returns a copy of `mBrushColor` as a base `Color` object.
 * @see BrushTftLcdRgb565BE
 * @todo None.
 * @bug None.
 */
Color BrushTftLcdRgb565BE::getBrushColor(void)
{
	return mBrushColor;
}

/**
 * @details Updates `mBgColor` from `color` using `ColorRgb565BE` encoding.
 * @see ColorRgb565BE::setColorBase()
 * @todo None.
 * @bug None.
 */
void BrushTftLcdRgb565BE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

/**
 * @details Returns a copy of `mBgColor` as a base `Color` object.
 * @see BrushTftLcdRgb565BE
 * @todo None.
 * @bug None.
 */
Color BrushTftLcdRgb565BE::getBackgroundColor(void)
{
	return mBgColor;
}
