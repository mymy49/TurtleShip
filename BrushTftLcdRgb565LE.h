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
	BrushTftLcdRgb565LE(void);

	virtual ~BrushTftLcdRgb565LE(void);

	void setFrameBuffer(FrameBufferRgb565LE &obj);

	virtual void setBrushColor(Color color);

	virtual Color getBrushColor(void);

	virtual void setBackgroundColor(Color color);

	virtual Color getBackgroundColor(void);

protected :
	ColorRgb565LE mBrushColor, mBgColor, mFontColor;
	uint16_t mBrushColorCode;
	FrameBufferRgb565LE *mFb;
};

#endif

