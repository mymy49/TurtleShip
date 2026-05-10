/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_MOD_BRUSH_TFT_LCD_RGB_565__H_
#define YSS_MOD_BRUSH_TFT_LCD_RGB_565__H_

class Size;

#include "Brush.h"
#include "ColorRgb565LE.h"

class BrushTftLcdRgb565 : public Brush
{
public :
	BrushTftLcdRgb565(void);

	virtual ~BrushTftLcdRgb565(void);

protected :
	uint16_t mBrushColorCode;

	virtual void fillDotArray(uint32_t offset, uint32_t count, Color color);

	virtual Size getCanvasSize(void);

	virtual uint8_t getPixelCapacity(void);
};

#endif

