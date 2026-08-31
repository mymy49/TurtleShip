/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_MOD_BRUSH_TFT_LCD_RGB_565LE__H_
#define YSS_MOD_BRUSH_TFT_LCD_RGB_565LE__H_

#include "BrushTftLcdRgb565.h"
#include "ColorRgb565LE.h"
#include "FrameBufferRgb565LE.h"

class BrushTftLcdRgb565LE : public BrushTftLcdRgb565
{
public :
	BrushTftLcdRgb565LE(void) __attribute__((optimize("-O1")));

	virtual ~BrushTftLcdRgb565LE(void) __attribute__((optimize("-O1")));

	void setFrameBuffer(FrameBufferRgb565LE &obj) __attribute__((optimize("-O1")));

	void setBrushColor(Color color) override __attribute__((optimize("-O1")));

	void setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override __attribute__((optimize("-O1")));

	Color getBrushColor(void) override __attribute__((optimize("-O1")));

	void setBackgroundColor(Color color) override __attribute__((optimize("-O1")));

	void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override __attribute__((optimize("-O1")));

	Color getBackgroundColor(void) override __attribute__((optimize("-O1")));

protected :
	ColorRgb565LE mBrushColor, mBgColor, mFontColor;
	uint16_t mBrushColorCode;
	FrameBufferRgb565LE *mFb;
};

#endif

