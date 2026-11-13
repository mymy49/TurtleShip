/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Color.h
 * @brief Generic RGBA color class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_COLOR__H_
#define YSS_GUI_COLOR__H_

#include <stdint.h>

/**
 * @class Color
 * @brief RGBA color container with a cached pixel-format-specific hardware color code.
 *
 * @details
 * `Color` stores 8-bit red, green, blue, and alpha components (`0` ~ `255`) along with a
 * pre-computed 32-bit color code (`mCode`).
 *
 * The base `Color` class encodes `mCode` in 32-bit ARGB8888 format (`0xAARRGGBB`).
 * Derived classes (`ColorRgb565LE`, `ColorRgb565BE`, `ColorBgr565LE`, `ColorBgr565BE`) override
 * `setColorBase()` to compute 16-bit packed color codes tailored to the byte order and channel
 * order of specific framebuffers and TFT LCD controllers.
 *
 * ### Example Usage in User Application
 * @code
 * #include <bsp.h>
 *
 * static Color gBasicBorderColor = {0x80, 0x80, 0x80};
 * static Color gSelectedBorderColor = {0xFF, 0x00, 0x00};
 *
 * void drawPaletteBox(Area area, Color bdColor, Color fillColor)
 * {
 *     brush.setBackgroundColor(bdColor);
 *     brush.setBrushColor(fillColor);
 *     brush.clear();
 *     brush.fillRectangular({2, 2, 26, 14});
 *     lcd.drawBitmap(area.getPosition(), brush.getBitmap());
 * }
 * @endcode
 */
class Color
{
public:
	/**
	 * @brief Constructs a black, fully opaque color (`R=0, G=0, B=0, A=0xFF`).
	 * @author Yoon-Ki Hong
	 * @note `setColorBase()` is invoked from the base constructor, so the base ARGB8888 encoding
	 *       is executed during base construction.
	 */
	Color(void);

	/**
	 * @brief Constructs a `Color` from individual 8-bit RGBA components.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`: opaque).
	 * @author Yoon-Ki Hong
	 */
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

	/**
	 * @brief Copies the RGBA components from another `Color` object and re-encodes `mCode`.
	 * @param[in] obj Source `Color` object whose 8-bit RGBA channels are copied.
	 * @author Yoon-Ki Hong
	 * @note Only the RGBA components of `obj` are copied; `mCode` is regenerated in this instance's
	 *       own pixel format via `setColorBase()`.
	 */
	void setColor(Color obj);

	/**
	 * @brief Sets the color from individual 8-bit RGBA components and re-encodes `mCode`.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`: opaque).
	 * @author Yoon-Ki Hong
	 */
	void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

	/**
	 * @brief Assignment operator that copies RGBA components from `obj` and re-encodes `mCode`.
	 * @param[in] obj Source `Color` reference.
	 * @author Yoon-Ki Hong
	 */
	void operator=(Color &obj);

	/**
	 * @brief Returns the pre-computed hardware color code in this object's pixel format.
	 * @return Encoded color code (`0xAARRGGBB` for the base `Color` class, or 16-bit packed RGB565/BGR565
	 *         values for derived classes).
	 * @author Yoon-Ki Hong
	 */
	virtual uint32_t getCode(void);

		
protected:
	uint8_t mRed,   ///< Red component (`0` ~ `255`).
	        mGreen, ///< Green component (`0` ~ `255`).
	        mBlue,  ///< Blue component (`0` ~ `255`).
	        mAlpha; ///< Alpha component (`0` ~ `255`, `0xFF` = opaque).

	uint32_t mCode; ///< Cached color code encoded in the pixel format of this class.

	/**
	 * @brief Stores the 8-bit RGBA components and encodes them into `mCode`.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`).
	 * @author Yoon-Ki Hong
	 * @note Overridden by derived classes to produce pixel-format-specific color codes.
	 */
	virtual void setColorBase(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
};

#endif
