/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_BITMAP_DRAWING_CALCULATOR__H_
#define YSS_GUI_BITMAP_DRAWING_CALCULATOR__H_

#include <stdint.h>

#include "Rectangular.h"

class BitmapDrawingCalculator
{
public:
	BitmapDrawingCalculator(void);

	BitmapDrawingCalculator(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapSrcArea);
	
	bool calculate(void);

	uint32_t getBitmapSrcFrameBufferOffset(void);

	uint32_t getCanvasDesFrameBufferOffset(void);

	uint16_t getTrimedBitmapWidth(void);

	uint16_t getTrimedBitmapHeight(void);

	Rectangular &getTrimedCanvasDesArea(void);

	uint32_t getTrimedCanvasDesAreaValue(void);

	Rectangular &getTrimedBitmapSrcArea(void);

private :
	Size mCanvasSize;
	Rectangular mCanvasDesArea, mBitmapSrcArea;

	Rectangular mTrimedCanvasDesArea, mTrimedBitmapSrcArea;
	uint32_t mSrcFrameBufferOffset, mDesFrameBufferOffset;
};

#endif

