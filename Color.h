/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_COLOR__H_
#define YSS_GUI_COLOR__H_

#include <stdint.h>

class Color
{
public:
	Color(void);

	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

	void setColor(Color obj);

	void operator=(Color &obj);

	virtual uint32_t getCode(void);

		
protected:
	uint8_t mRed, mGreen, mBlue, mAlapha;

	uint32_t mCode;

	virtual void setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
};

#endif

