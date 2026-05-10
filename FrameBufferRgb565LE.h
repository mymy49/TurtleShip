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
	FrameBufferRgb565LE(void);

	virtual ~FrameBufferRgb565LE(void);

	virtual void blendDot(int16_t x, int16_t y, uint8_t alpha);
	
	virtual void setBrushColor(Color color);

	virtual Color getBrushColor(void);

	virtual void setBackgroundColor(Color color);

	virtual Color getBackgroundColor(void);

protected :

private :
	ColorRgb565LE mBrushColor, mBgColor, mFontColor;
};

#endif

