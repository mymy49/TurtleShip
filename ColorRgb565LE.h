/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file ColorRgb565LE.h
 * @brief 16-bit RGB565 Little-Endian color encoding class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 * @date 2025
 */

#ifndef YSS_GUI_COLOR_RGB565LE__H_
#define YSS_GUI_COLOR_RGB565LE__H_

#include "Color.h"

/**
 * @class ColorRgb565LE
 * @brief Derived `Color` class that encodes RGBA values into a 16-bit Little-Endian RGB565 color code.
 *
 * @details
 * `ColorRgb565LE` is used by `FrameBufferRgb565LE` and `BrushTftLcdRgb565LE` to pre-calculate
 * standard 16-bit Little-Endian RGB565 color codes (`[15:11]` Red, `[10:5]` Green, `[4:0]` Blue)
 * in `mCode`.
 *
 * ### Example Usage in User Application
 * @code
 * // Internally used by FrameBufferRgb565LE and BrushTftLcdRgb565LE when setting colors:
 * brush.setBackgroundColor(0xFF, 0xFF, 0xFF);
 * brush.setBrushColor({0xFF, 0x30, 0x30});
 * @endcode
 */
class ColorRgb565LE : public Color
{
public:
	/**
	 * @brief Constructs a default `ColorRgb565LE` object.
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
	ColorRgb565LE(void);

	/**
	 * @brief Stores the RGBA components and encodes them as a 16-bit Little-Endian RGB565 color code in `mCode`.
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
