/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file ColorRgb565BE.h
 * @brief 16-bit RGB565 Big-Endian color encoding class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 * @date 2025
 */

#ifndef YSS_GUI_COLOR_RGB565BE__H_
#define YSS_GUI_COLOR_RGB565BE__H_

#include "Color.h"

/**
 * @class ColorRgb565BE
 * @brief Derived `Color` class that encodes RGBA values into a 16-bit Big-Endian 5-6-5 color code.
 *
 * @details
 * `ColorRgb565BE` is used by `FrameBufferRgb565BE` and `BrushTftLcdRgb565BE` to pre-calculate
 * 16-bit byte-swapped (Big-Endian) color codes in `mCode`.
 */
class ColorRgb565BE : public Color
{
public:
	/**
	 * @brief Constructs a default `ColorRgb565BE` object.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2025
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	ColorRgb565BE(void);

	/**
	 * @brief Stores the RGBA components and encodes them as a 16-bit Big-Endian 5-6-5 color code in `mCode`.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2025
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override;

protected:
};

#endif
