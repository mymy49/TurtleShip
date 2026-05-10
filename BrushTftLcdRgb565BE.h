/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_MOD_BRUSH_TFT_LCD_RGB_565BE__H_
#define YSS_MOD_BRUSH_TFT_LCD_RGB_565BE__H_

#include "BrushTftLcdRgb565.h"
#include "ColorRgb565BE.h"

class FrameBufferRgb565BE;

class BrushTftLcdRgb565BE : public BrushTftLcdRgb565
{
public :
	BrushTftLcdRgb565BE(void);

	virtual ~BrushTftLcdRgb565BE(void);

	void setFrameBuffer(FrameBufferRgb565BE &obj);

	virtual void setBrushColor(Color color);

	virtual Color getBrushColor(void);

	virtual void setBackgroundColor(Color color);

	virtual Color getBackgroundColor(void);

protected :
	ColorRgb565BE mBrushColor, mBgColor, mFontColor;
	uint16_t mBrushColorCode;
	FrameBufferRgb565BE *mFb;
};

#endif

