/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565__H_
#define YSS_GUI_FRAME_BUFFER_RGB565__H_

#include "FrameBuffer.h"

class FrameBufferRgb565 : public FrameBuffer
{
public :
	FrameBufferRgb565(void);

	~FrameBufferRgb565(void) override;

	uint8_t getPixelCapacity(void) override;

	void drawDot(int16_t x, int16_t y) override;

	bitmap_t getBitmap(void) override;

protected :
	uint16_t mBrushColorCode;

	void fillDotArray(uint32_t offset, uint32_t count, Color color) override;

	void drawBitmapBase(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap) override;

	void drawBitmapBase(Position pos, const bitmap_t bitmap) override;

	Size getCanvasSize(void) override;

private :
	void drawBitmapRgb565(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap);

	void drawBitmapArgb1555(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap);
};

#endif

