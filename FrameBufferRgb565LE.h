/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565LE__H_
#define YSS_GUI_FRAME_BUFFER_RGB565LE__H_

#include "FrameBufferRgb565.h"
#include "ColorRgb565LE.h"

class FrameBufferRgb565LE : public FrameBufferRgb565
{
public :
	FrameBufferRgb565LE(void) __attribute__((optimize("-O1")));

	~FrameBufferRgb565LE(void) override __attribute__((optimize("-O1")));

	void blendDot(int16_t x, int16_t y, uint8_t alpha) override __attribute__((optimize("-O1")));
	
	void setBrushColor(Color color) override __attribute__((optimize("-O1")));

	void setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override __attribute__((optimize("-O1")));

	Color getBrushColor(void) override __attribute__((optimize("-O1")));

	void setBackgroundColor(Color color) override __attribute__((optimize("-O1")));

	void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override __attribute__((optimize("-O1")));

	Color getBackgroundColor(void) override __attribute__((optimize("-O1")));

protected :

private :
	ColorRgb565LE mBrushColor, mBgColor, mFontColor;
};

#endif

