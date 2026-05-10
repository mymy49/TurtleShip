/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_COLOR_BGR565BE__H_
#define YSS_GUI_COLOR_BGR565BE__H_

#include "Color.h"

class ColorBgr565BE : public Color
{
public:
	ColorBgr565BE(void);

	virtual void setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

protected:
};

#endif

