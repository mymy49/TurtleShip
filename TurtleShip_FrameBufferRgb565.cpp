/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_FrameBufferRgb565.cpp
 * @brief Implementation of the FrameBufferRgb565 class for 16-bit RGB565 frame buffers.
 */

#include <config.h>

#include "FrameBufferRgb565.h"
#include <yss/Size.h>
#include <std_ext/string.h>
#include "BitmapDrawingCalculator.h"
#include <yss/lmalloc.h>
#include <yss/hmalloc.h>

/**
 * @details Default constructor for `FrameBufferRgb565`.
 */
FrameBufferRgb565::FrameBufferRgb565(void)
{
}

/**
 * @details Frees `mFrameBuffer` if `mMallocFlag` is set.
 * @warning `FrameBuffer::~FrameBuffer()` also checks `mMallocFlag` and frees `mFrameBuffer`;
 *          ensure `mMallocFlag` is cleared or managed carefully if subclass destructors also free it.
 */
FrameBufferRgb565::~FrameBufferRgb565(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

/**
 * @details Returns `2` bytes per pixel for the 16-bit RGB565 format.
 */
uint8_t FrameBufferRgb565::getPixelCapacity(void)
{
	return 2;
}

/**
 * @details Bounds-checks `(x, y)` against `[0, width - 1]` and `[0, height - 1]`, then writes
 * `mBrushColorCode` into `((uint16_t*)mFrameBuffer)[width * y + x]`.
 */
void FrameBufferRgb565::drawDot(int16_t x, int16_t y)
{
	uint16_t width = mSize.getWidth();
	uint16_t height = mSize.getHeight();
	uint16_t *des = (uint16_t*)mFrameBuffer;
	
	// Ignore pixels outside the logical canvas bounds
	if(x < 0 || y < 0 || x >= width || y >= height)
		return;

	des[width * y + x] = mBrushColorCode;
}

/**
 * @details Fills `count` 16-bit half-words (`count * 2` bytes) starting at `&mFrameBuffer[offset]`
 * with `color.getCode()` using the optimized `memsethw()` routine.
 */
void FrameBufferRgb565::fillDotArray(uint32_t offset, uint32_t count, Color color)
{
	memsethw(&mFrameBuffer[offset], color.getCode(), count * 2);
}

/**
 * @details Returns the current logical canvas dimensions `mSize`.
 */
Size FrameBufferRgb565::getCanvasSize(void)
{
	return mSize;
}

/**
 * @details Dispatches the clipped bitmap drawing operation to `drawBitmapRgb565()` or
 * `drawBitmapArgb1555()` according to `bitmap.type`.
 * @see FrameBufferRgb565::drawBitmapRgb565(), FrameBufferRgb565::drawBitmapArgb1555()
 */
void FrameBufferRgb565::drawBitmapBase(Size canvasSize, Area canvasRect, Position bitmapPos, const bitmap_t bitmap)
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

/**
 * @details Empty implementation of `drawBitmapBase(Position, const bitmap_t)`. Use the clipped overload
 * or draw onto the target LCD directly.
 */
void FrameBufferRgb565::drawBitmapBase(Position pos, const bitmap_t bitmap)
{

}

/**
 * @details Clips the source RGB565 `bitmap` against `canvasDesArea` and `canvasSize` using
 * `BitmapDrawingCalculator`, then copies each visible scanline (`bwidth * 2` bytes) from `bitmap.data`
 * into `mFrameBuffer` via `memcpy()`.
 * @see BitmapDrawingCalculator::calculate()
 */
void FrameBufferRgb565::drawBitmapRgb565(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap)
{
	if(bitmap.type != BITMAP_TYPE_RGB565)
		return;

	Area bitmapArea = {bitmapPos, {bitmap.width, bitmap.height}};
	BitmapDrawingCalculator bdc(canvasSize, canvasDesArea, bitmapArea);
	
	if(bdc.calculate() == false)
		return;

	uint16_t cwidth = getCanvasSize().getWidth(), *des = (uint16_t*)mFrameBuffer, *src = (uint16_t*)bitmap.data;
	uint16_t bwidth = bdc.getTrimedBitmapWidth();
	uint16_t bheight = bdc.getTrimedBitmapHeight();
	des = &des[bdc.getCanvasDesFrameBufferOffset()];
	src = &src[bdc.getBitmapSrcFrameBufferOffset()];

	// Copy each clipped scanline from the RGB565 bitmap into the frame buffer
	for(uint16_t i = 0; i < bheight; i++)
	{
		memcpy(des, src, bwidth * 2);
		des += cwidth;
		src += bitmap.width;
	}
}

/**
 * @details Clips the source ARGB1555 `bitmap` against `canvasDesArea` and `canvasSize` using
 * `BitmapDrawingCalculator`, then iterates over each visible pixel:
 * - If the alpha bit (bit 15, `0x8000`) is `0`, the destination pixel is left untouched (transparent).
 * - If the alpha bit is `1`, converts the 15-bit RGB555 value into 16-bit RGB565 (replicating the
 *   MSB of green into the LSB of the 6-bit green field) and writes it to `*des`.
 * @see BitmapDrawingCalculator::calculate()
 */
void FrameBufferRgb565::drawBitmapArgb1555(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap)
{
	if(bitmap.type != BITMAP_TYPE_ARGB1555)
		return;
	
	Area bitmapArea = {bitmapPos, {bitmap.width, bitmap.height}};
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
			// Draw pixel only when the 1-bit alpha flag (bit 15) is set
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

/**
 * @details Constructs and returns a `bitmap_t` structure initialized with `mSize.getWidth()`,
 * `mSize.getHeight()`, `BITMAP_TYPE_RGB565`, and `mFrameBuffer`.
 */
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
