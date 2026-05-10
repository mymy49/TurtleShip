/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

 #if USE_GUI
/*
#include <gui/FrameBufferArgb1555LE.h>
#include <gui/ColorRgb565LE.h>
#include <gui/Size.h>
#include <cmsis/cmsis_compiler.h>
#include <std_ext/malloc.h>
#include <std_ext/string.h>
#include <__debug_stdio.h>
#include <gui/BitmapDrawingCalculator.h>
#include <stdlib.h>

FrameBufferArgb1555LE::FrameBufferArgb1555LE(void)
{
}

FrameBufferArgb1555LE::~FrameBufferArgb1555LE(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

void FrameBufferArgb1555LE::blendDot(int16_t x, int16_t y, uint8_t alpha)
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
	
	des[width * y + x] = ((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | ((b >> 3) & 0x001F);
}

void FrameBufferArgb1555LE::setBrushColor(Color color)
{
	mBrushColor.Color::setColor(color);
	mBrushColorCode = mBrushColor.getCode();
}

Color FrameBufferArgb1555LE::getBrushColor(void)
{
	return mBrushColor;
}

void FrameBufferArgb1555LE::setBackgroundColor(Color color)
{
	mBgColor.Color::setColor(color);
}

Color FrameBufferArgb1555LE::getBackgroundColor(void)
{
	return mBgColor;
}
*/
#endif

