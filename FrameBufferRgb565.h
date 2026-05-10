/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565__H_
#define YSS_GUI_FRAME_BUFFER_RGB565__H_

#include "FrameBuffer.h"
#include "Brush.h"

class FrameBufferRgb565 : public FrameBuffer, public Brush
{
public :
	FrameBufferRgb565(void);

	virtual ~FrameBufferRgb565(void);

	virtual uint8_t getPixelCapacity(void);

	virtual void drawDot(int16_t x, int16_t y);

	virtual bitmap_t getBitmap(void);

protected :
	uint16_t mBrushColorCode;

	virtual void fillDotArray(uint32_t offset, uint32_t count, Color color);

	virtual void drawBitmapBase(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap);

	virtual Size getCanvasSize(void);

private :
	void drawBitmapRgb565(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap);

	void drawBitmapArgb1555(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap);
};

#endif

