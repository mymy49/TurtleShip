/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file ColorBgr565LE.h
 * @brief 16-bit BGR565 Little-Endian color encoding class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_COLOR_BGR565LE__H_
#define YSS_GUI_COLOR_BGR565LE__H_

#include "Color.h"

/**
 * @class ColorBgr565LE
 * @brief Derived `Color` class that encodes RGBA values into a 16-bit BGR565 Little-Endian color code.
 *
 * @details
 * `ColorBgr565LE` overrides `setColorBase()` to convert 8-bit RGB channels into a 16-bit
 * Little-Endian BGR565 code (`[15:11]` Blue, `[10:5]` Green, `[4:0]` Red) stored in `mCode`.
 */
class ColorBgr565LE : public Color
{
public:
	/**
	 * @brief Constructs a default `ColorBgr565LE` object.
	 * @author Yoon-Ki Hong
	 */
	ColorBgr565LE(void);

	/**
	 * @brief Stores the RGBA components and encodes them as a 16-bit Little-Endian BGR565 color code in `mCode`.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`).
	 * @author Yoon-Ki Hong
	 */
	virtual void setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override;

protected:
};

#endif
