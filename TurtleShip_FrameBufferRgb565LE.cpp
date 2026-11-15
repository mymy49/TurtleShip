/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_FrameBufferRgb565LE.cpp
 * @brief Implementation of the FrameBufferRgb565LE class for Little-Endian 16-bit RGB565 frame buffers.
 */

#include <config.h>

#include "FrameBufferRgb565LE.h"
#include "ColorRgb565LE.h"
#include <yss/Size.h>
#include <std_ext/string.h>
#include <__debug_stdio.h>
#include <yss/lmalloc.h>
#include <yss/hmalloc.h>

/**
 * @details Default constructor for `FrameBufferRgb565LE`.
 */
FrameBufferRgb565LE::FrameBufferRgb565LE(void)
{
}

/**
 * @details Frees `mFrameBuffer` if `mMallocFlag` is set.
 */
FrameBufferRgb565LE::~FrameBufferRgb565LE(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

/**
 * @details Alpha-blends `mBrushColorCode` onto the existing Little-Endian RGB565 pixel at `(x, y)`.
 *
 * -# Returns early if `alpha < 16` (nearly transparent) or if `(x, y)` lies outside `[0, width - 1]` / `[0, height - 1]`.
 * -# Reads the 16-bit pixel `blend = des[width * y + x]`.
 * -# Unpacks 5-bit Red, 6-bit Green, and 5-bit Blue from both `blend` and `mBrushColorCode` into
 *    8-bit fixed-point values and linearly interpolates each channel by `alpha`:
 *    @f$ C_{\text{out}} = \lfloor((C_{\text{brush}} - C_{\text{des}}) \times \text{alpha}) / 256\rfloor + C_{\text{des}} @f$.
 * -# Packs the blended channels back into 16-bit RGB565 (`((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | ((b >> 3) & 0x001F)`)
 *    and stores the result into `des[width * y + x]`.
 * @see Brush::drawChar()
 * @todo None.
 * @bug None.
 */
void FrameBufferRgb565LE::blendDot(int16_t x, int16_t y, uint8_t alpha)
{
	if(alpha < 16)
		return;

	uint16_t width = mSize.getWidth();
	uint16_t height = mSize.getHeight();

	if(x < 0 || y < 0 || x >= width || y >= height)
		return;

	uint16_t *des = (uint16_t*)mFrameBuffer;
	uint16_t blend;
	int32_t r, g, b, buf;
	
	blend = des[width * y + x];
	
	// Interpolate Red channel (bits [15:11])
	r = blend >> 8 & 0xF8 | 0x07;
	buf = mBrushColorCode >> 8 & 0xF8 | 0x07;
	r = (((buf - r) * (int32_t)alpha) >> 8) + r;
	
	// Interpolate Green channel (bits [10:5])
	g = blend >> 3 & 0xFC | 0x03;
	buf = mBrushColorCode >> 3 & 0xFC | 0x03;
	g = (((buf - g) * (int32_t)alpha) >> 8) + g;

	// Interpolate Blue channel (bits [4:0])
	b = (blend << 3) & 0xF8 | 0x07;
	buf = (mBrushColorCode << 3) & 0xF8;// | 0x07;
	b = (((buf - b) * (int32_t)alpha) >> 8) + b;
	
	// Pack interpolated channels back into 16-bit Little-Endian RGB565
	des[width * y + x] = ((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | ((b >> 3) & 0x001F);
}

/**
 * @details Copies the RGBA components of `color` into `mBrushColor` and caches the resulting
 * 16-bit Little-Endian RGB565 code in `mBrushColorCode`.
 * @see ColorRgb565LE::setColorBase()
 */
void FrameBufferRgb565LE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

/**
 * @details Sets `mBrushColor` from the given `red`, `green`, `blue`, and `alpha` values and caches
 * the resulting 16-bit Little-Endian RGB565 code in `mBrushColorCode`.
 * @see ColorRgb565LE::setColorBase()
 */
void FrameBufferRgb565LE::setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBrushColor.Color::setColor(red, green, blue, alpha);
	mBrushColorCode = mBrushColor.getCode();
}

/**
 * @details Returns a copy of `mBrushColor` as a base `Color` object.
 */
Color FrameBufferRgb565LE::getBrushColor(void)
{
	return mBrushColor;
}

/**
 * @details Copies the RGBA components of `color` into `mBgColor` and encodes its 16-bit Little-Endian RGB565 code.
 * @see ColorRgb565LE::setColorBase()
 */
void FrameBufferRgb565LE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

/**
 * @details Sets `mBgColor` from the given `red`, `green`, `blue`, and `alpha` values and encodes its
 * 16-bit Little-Endian RGB565 code.
 * @see ColorRgb565LE::setColorBase()
 */
void FrameBufferRgb565LE::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBgColor.Color::setColor(red, green, blue, alpha);
}

/**
 * @details Returns a copy of `mBgColor` as a base `Color` object.
 */
Color FrameBufferRgb565LE::getBackgroundColor(void)
{
	return mBgColor;
}
