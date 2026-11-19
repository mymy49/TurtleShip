/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_FrameBufferRgb565BE.cpp
 * @brief Implementation of the FrameBufferRgb565BE class for Big-Endian 16-bit RGB565 frame buffers.
 * @author Yoon-Ki Hong
 * @date 2025
 */

#include <config.h>

#if USE_GUI

#include <gui/FrameBufferRgb565BE.h>
#include <gui/ColorRgb565BE.h>
#include <gui/Size.h>
#include <cmsis/cmsis_compiler.h>
#include <std_ext/malloc.h>
#include <std_ext/string.h>
#include <stdlib.h>
#include <gui/BitmapDrawingCalculator.h>

/**
 * @details Default constructor for `FrameBufferRgb565BE`.
 * @see FrameBufferRgb565BE
 * @todo None.
 * @bug None.
 */
FrameBufferRgb565BE::FrameBufferRgb565BE(void)
{
}

/**
 * @details Frees `mFrameBuffer` if `mMallocFlag` is set.
 * @see FrameBufferRgb565BE
 * @todo None.
 * @bug None.
 */
FrameBufferRgb565BE::~FrameBufferRgb565BE(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

/**
 * @details Blends the foreground brush color with the existing Big-Endian RGB565 pixel at `(x, y)`.
 *
 * -# Returns early if `alpha < 16` or if `(x, y)` is outside the canvas bounds.
 * -# Reads the 16-bit Big-Endian destination pixel and byte-swaps it to native Little-Endian order.
 * -# Extracts the 8-bit expanded Red, Green, and Blue channels of both the destination pixel and
 *    the brush color, and linearly interpolates each channel:
 *    @f$ C_{\text{out}} = \lfloor((C_{\text{brush}} - C_{\text{des}}) \times \text{alpha}) / 256\rfloor + C_{\text{des}} @f$.
 * -# Repacks the blended channels into 16-bit RGB565, byte-swaps back to Big-Endian, and writes to `des[width * y + x]`.
 * @see Brush::drawChar()
 * @todo None.
 * @bug None.
 */
void FrameBufferRgb565BE::blendDot(int16_t x, int16_t y, uint8_t alpha)
{
	if(alpha < 16)
		return;

	uint16_t width = mSize.getWidth();
	uint16_t height = mSize.getHeight();

	if(x < 0 || y < 0 || x >= width || y >= height)
		return;

	uint16_t *des = (uint16_t*)mFrameBuffer;
	uint16_t blend, code;
	int32_t r, g, b, buf;
	
	// Read big-endian pixel and swap bytes for channel extraction
	buf = des[width * y + x];
	blend = buf >> 8;
	blend |= buf << 8;
	code = mBrushColorCode >> 8;
	code |= mBrushColorCode << 8;

	// Interpolate Red channel
	r = blend >> 8 & 0xF8 | 0x07;
	buf = mBrushColorCode >> 8 & 0xF8 | 0x07;
	r = (((buf - r) * (int32_t)alpha) >> 8) + r;
	
	// Interpolate Green channel
	g = blend >> 3 & 0xFC | 0x03;
	buf = mBrushColorCode >> 3 & 0xFC | 0x03;
	g = (((buf - g) * (int32_t)alpha) >> 8) + g;

	// Interpolate Blue channel
	b = (blend << 3) & 0xF8 | 0x07;
	buf = (mBrushColorCode << 3) & 0xF8;// | 0x07;
	b = (((buf - b) * (int32_t)alpha) >> 8) + b;

	// Repack into RGB565 and byte-swap back to big-endian
	blend = ((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | ((b >> 3) & 0x001F);
	code = blend >> 8;
	code |= blend << 8;
	
	des[width * y + x] = code;
}

/**
 * @details Updates `mBrushColor` from `color` and caches the Big-Endian 16-bit code in `mBrushColorCode`.
 * @see FrameBufferRgb565BE
 * @todo None.
 * @bug None.
 */
void FrameBufferRgb565BE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

/**
 * @details Returns the current foreground brush color (`mBrushColor`).
 * @see FrameBufferRgb565BE
 * @todo None.
 * @bug None.
 */
Color FrameBufferRgb565BE::getBrushColor(void)
{
	return mBrushColor;
}

/**
 * @details Updates the background color (`mBgColor`) from `color`.
 * @see FrameBufferRgb565BE
 * @todo None.
 * @bug None.
 */
void FrameBufferRgb565BE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

/**
 * @details Returns the current background color (`mBgColor`).
 * @see FrameBufferRgb565BE
 * @todo None.
 * @bug None.
 */
Color FrameBufferRgb565BE::getBackgroundColor(void)
{
	return mBgColor;
}

#endif
