/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_COLOR_RGB888__H_
#define YSS_GUI_COLOR_RGB888__H_

#include "Color.h"

class ColorRgb888 : public Color
{
public:
	ColorRgb888(void);

	ColorRgb888(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

	virtual void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

	uint32_t getCode(void);
		
protected:
	uint32_t mCode;
};

#endif

