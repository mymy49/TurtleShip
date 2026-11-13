/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_BrushTftLcdRgb565LE.cpp
 * @brief Implementation of the BrushTftLcdRgb565LE class for Little-Endian RGB565 TFT LCD brushes.
 */

#include "BrushTftLcdRgb565LE.h"
#include "ColorRgb565LE.h"
#include <config.h>
#include <yss/hmalloc.h>
#include <std_ext/string.h>

/**
 * @details Initializes the brush with `mFb` set to `nullptr`.
 */
BrushTftLcdRgb565LE::BrushTftLcdRgb565LE(void)
{
	mFb = nullptr;
}

/**
 * @details Virtual destructor for `BrushTftLcdRgb565LE`.
 */
BrushTftLcdRgb565LE::~BrushTftLcdRgb565LE(void)
{

}

/**
 * @details Stores the address of `obj` into `mFb` so derived LCD drivers can use its pixel buffer
 * for chunked DMA/SPI transfers during rectangular fills.
 */
void BrushTftLcdRgb565LE::setFrameBuffer(FrameBufferRgb565LE &obj)
{
	mFb = &obj;
}

/**
 * @details Copies the RGBA components of `color` into `mBrushColor`, encodes the 16-bit Little-Endian
 * RGB565 value via `ColorRgb565LE::setColorBase()`, and caches the result in `mBrushColorCode`.
 * @see ColorRgb565LE::setColorBase()
 */
void BrushTftLcdRgb565LE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

/**
 * @details Sets `mBrushColor` from the given `red`, `green`, `blue`, and `alpha` values and caches
 * the encoded 16-bit Little-Endian RGB565 value in `mBrushColorCode`.
 * @see ColorRgb565LE::setColorBase()
 */
void BrushTftLcdRgb565LE::setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBrushColor.Color::setColor(red, green, blue, alpha);
	mBrushColorCode = mBrushColor.getCode();
}

/**
 * @details Returns a copy of `mBrushColor` as a base `Color` object.
 */
Color BrushTftLcdRgb565LE::getBrushColor(void)
{
	return mBrushColor;
}

/**
 * @details Copies the RGBA components of `color` into `mBgColor` and encodes its Little-Endian RGB565 code.
 * @see ColorRgb565LE::setColorBase()
 */
void BrushTftLcdRgb565LE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

/**
 * @details Sets `mBgColor` from the given `red`, `green`, `blue`, and `alpha` values and encodes its
 * Little-Endian RGB565 code.
 * @see ColorRgb565LE::setColorBase()
 */
void BrushTftLcdRgb565LE::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBgColor.Color::setColor(red, green, blue, alpha);
}

/**
 * @details Returns a copy of `mBgColor` as a base `Color` object.
 */
Color BrushTftLcdRgb565LE::getBackgroundColor(void)
{
	return mBgColor;
}
