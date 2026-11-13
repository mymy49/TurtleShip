/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file ColorRgb888.h
 * @brief 24/32-bit RGB888 color encoding class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_COLOR_RGB888__H_
#define YSS_GUI_COLOR_RGB888__H_

#include "Color.h"

/**
 * @class ColorRgb888
 * @brief Derived `Color` class representing 24-bit RGB888 / 32-bit ARGB8888 colors.
 *
 * @details
 * Provides a specialized `Color` interface for 24-bit RGB888 surfaces. Note that the base `Color`
 * class already encodes 32-bit ARGB8888 (`0xAARRGGBB`) by default.
 */
class ColorRgb888 : public Color
{
public:
	/**
	 * @brief Constructs a default `ColorRgb888` object.
	 * @author Yoon-Ki Hong
	 */
	ColorRgb888(void);

	/**
	 * @brief Constructs a `ColorRgb888` object from 8-bit RGBA components.
	 * @param[in] r Red component (`0` ~ `255`).
	 * @param[in] g Green component (`0` ~ `255`).
	 * @param[in] b Blue component (`0` ~ `255`).
	 * @param[in] a Alpha component (`0` ~ `255`, default `0xFF`).
	 * @author Yoon-Ki Hong
	 */
	ColorRgb888(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

	/**
	 * @brief Sets the RGB888 color from 8-bit RGBA components.
	 * @param[in] r Red component (`0` ~ `255`).
	 * @param[in] g Green component (`0` ~ `255`).
	 * @param[in] b Blue component (`0` ~ `255`).
	 * @param[in] a Alpha component (`0` ~ `255`, default `0xFF`).
	 * @author Yoon-Ki Hong
	 */
	virtual void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

	/**
	 * @brief Gets the encoded RGB888 color code.
	 * @return 32-bit encoded color code.
	 * @author Yoon-Ki Hong
	 */
	uint32_t getCode(void);
		
protected:
	uint32_t mCode; ///< Cached RGB888 color code.
};

#endif
