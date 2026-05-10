/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
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

FrameBufferRgb565BE::FrameBufferRgb565BE(void)
{
}

FrameBufferRgb565BE::~FrameBufferRgb565BE(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

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
	
	buf = des[width * y + x];
	blend = buf >> 8;
	blend |= buf << 8;
	code = mBrushColorCode >> 8;
	code |= mBrushColorCode << 8;

	// Red
	r = blend >> 8 & 0xF8 | 0x07;
	buf = mBrushColorCode >> 8 & 0xF8 | 0x07;
	r = (((buf - r) * (int32_t)alpha) >> 8) + r;
	
	// Green
	g = blend >> 3 & 0xFC | 0x03;
	buf = mBrushColorCode >> 3 & 0xFC | 0x03;
	g = (((buf - g) * (int32_t)alpha) >> 8) + g;

	// Blue
	b = (blend << 3) & 0xF8 | 0x07;
	buf = (mBrushColorCode << 3) & 0xF8;// | 0x07;
	b = (((buf - b) * (int32_t)alpha) >> 8) + b;

	blend = ((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | ((b >> 3) & 0x001F);
	code = blend >> 8;
	code |= blend << 8;
	
	des[width * y + x] = code;
}

void FrameBufferRgb565BE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

Color FrameBufferRgb565BE::getBrushColor(void)
{
	return mBrushColor;
}

void FrameBufferRgb565BE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

Color FrameBufferRgb565BE::getBackgroundColor(void)
{
	return mBgColor;
}

#endif

