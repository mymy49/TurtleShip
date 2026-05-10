/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

#if USE_GUI

#include <gui/FrameBufferRgb565.h>
#include <gui/ColorRgb565BE.h>
#include <gui/Size.h>
#include <cmsis/cmsis_compiler.h>
#include <std_ext/malloc.h>
#include <std_ext/string.h>
#include <stdlib.h>
#include <gui/BitmapDrawingCalculator.h>

FrameBufferRgb565::FrameBufferRgb565(void)
{
}

FrameBufferRgb565::~FrameBufferRgb565(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

uint8_t FrameBufferRgb565::getPixelCapacity(void)
{
	return 2;
}

void FrameBufferRgb565::drawDot(int16_t x, int16_t y)
{
	uint16_t width = mSize.getWidth();
	uint16_t height = mSize.getHeight();
	uint16_t *des = (uint16_t*)mFrameBuffer;
	
	if(x < 0 || y < 0 || x >= width || y >= height)
		return;

	des[width * y + x] = mBrushColorCode;
}

void FrameBufferRgb565::fillDotArray(uint32_t offset, uint32_t count, Color color)
{
	memsethw(&mFrameBuffer[offset], color.getCode(), count * 2);
}

Size FrameBufferRgb565::getCanvasSize(void)
{
	return mSize;
}

void FrameBufferRgb565::drawBitmapBase(Size canvasSize, Rectangular canvasRect, Position bitmapPos, const bitmap_t bitmap)
{
	switch(bitmap.type)
	{
	case BITMAP_TYPE_RGB565 :
		drawBitmapRgb565(getCanvasSize(), canvasRect, bitmapPos, bitmap);
		break;
	
	case BITMAP_TYPE_ARGB1555 :
		drawBitmapArgb1555(getCanvasSize(),canvasRect, bitmapPos, bitmap);
		break;

	default :
		return;
	}
}

void FrameBufferRgb565::drawBitmapRgb565(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap)
{
	if(bitmap.type != BITMAP_TYPE_RGB565)
		return;

	Rectangular bitmapArea = {bitmapPos, {bitmap.width, bitmap.height}};
	BitmapDrawingCalculator bdc(canvasSize, canvasDesArea, bitmapArea);
	
	if(bdc.calculate() == false)
		return;

	uint16_t cwidth = getCanvasSize().getWidth(), *des = (uint16_t*)mFrameBuffer, *src = (uint16_t*)bitmap.data;
	uint16_t bwidth = bdc.getTrimedBitmapWidth();
	uint16_t bheight = bdc.getTrimedBitmapHeight();
	des = &des[bdc.getCanvasDesFrameBufferOffset()];
	src = &src[bdc.getBitmapSrcFrameBufferOffset()];

	for(uint16_t i = 0; i < bheight; i++)
	{
		memcpy(des, src, bwidth * 2);
		des += cwidth;
		src += bitmap.width;
	}
}

void FrameBufferRgb565::drawBitmapArgb1555(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap)
{
	if(bitmap.type != BITMAP_TYPE_ARGB1555)
		return;
	
	Rectangular bitmapArea = {bitmapPos, {bitmap.width, bitmap.height}};
	BitmapDrawingCalculator bdc(canvasSize, canvasDesArea, bitmapArea);
	
	if(bdc.calculate() == false)
		return;

	uint16_t cwidth = getCanvasSize().getWidth(), *des = (uint16_t*)mFrameBuffer, *src = (uint16_t*)bitmap.data;
	uint16_t bwidth = bdc.getTrimedBitmapWidth();
	uint16_t bheight = bdc.getTrimedBitmapHeight();
	uint16_t buf, offset, trans;
	des = &des[bdc.getCanvasDesFrameBufferOffset()];
	src = &src[bdc.getBitmapSrcFrameBufferOffset()];
	offset = bitmap.width - bwidth;

	for(uint16_t i = 0; i < bheight; i++)
	{
		for(uint16_t j = 0; j < bwidth; j++)
		{
			trans = *src++;
			if(trans & 0x8000)
			{
				buf = trans;
				trans <<= 1;
				trans &= ~0x003F;
				trans |= buf & 0x1F;
				if(trans & 0x0400)
					trans |= 0x0020;
				*des = trans;
			}
			des++;
		}

		des += cwidth - bwidth;
		src += offset;
	}
}

bitmap_t FrameBufferRgb565::getBitmap(void)
{
	bitmap_t bitmap = 
	{
		mSize.getWidth(),	//uint16_t width;
		mSize.getHeight(),	//uint16_t height;
		BITMAP_TYPE_RGB565,	//bitmapType_t type; // 0 : RGB565, 1 : RGB888, 2 : ARGB1555
		mFrameBuffer		//uint8_t *data;
	};

	return bitmap;
}

#endif

