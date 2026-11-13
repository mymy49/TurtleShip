/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file BrushTftLcdRgb565LE.h
 * @brief Little-Endian RGB565 TFT LCD brush class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_MOD_BRUSH_TFT_LCD_RGB_565LE__H_
#define YSS_MOD_BRUSH_TFT_LCD_RGB_565LE__H_

#include "BrushTftLcdRgb565.h"
#include "ColorRgb565LE.h"
#include "FrameBufferRgb565LE.h"

/**
 * @class BrushTftLcdRgb565LE
 * @brief `BrushTftLcdRgb565` specialization using Little-Endian 16-bit RGB565 colors (`ColorRgb565LE`).
 *
 * @details
 * `BrushTftLcdRgb565LE` manages foreground (`mBrushColor`), background (`mBgColor`), and font
 * (`mFontColor`) colors in Little-Endian RGB565 format. It also holds a pointer (`mFb`) to an
 * external `FrameBufferRgb565LE` instance registered via `setFrameBuffer()`, allowing SPI/parallel
 * TFT LCD drivers (such as `ST7789V_spi_with_Brush_RGB565` and `Touch_LCD_Shield_for_Arduino_2_8_inch`)
 * to use the shared buffer for fast block fills (`fillRectBase()`, `clear()`).
 *
 * ### Example Usage in User Application
 * @code
 * #include <bsp.h>
 *
 * Touch_LCD_Shield_for_Arduino_2_8_inch lcd; // Inherits BrushTftLcdRgb565LE
 * FrameBufferRgb565LE brush;
 *
 * void initLcd(void)
 * {
 *     brush.malloc(5000);
 *
 *     lcd.initialize(lcdConfig);
 *     lcd.setFrameBuffer(brush);
 *     lcd.setBackgroundColor({0xFF, 0xFF, 0xFF});
 *     lcd.clear();
 * }
 * @endcode
 */
class BrushTftLcdRgb565LE : public BrushTftLcdRgb565
{
public :
	/**
	 * @brief Constructs a `BrushTftLcdRgb565LE` instance with `mFb` initialized to `nullptr`.
	 * @author Yoon-Ki Hong
	 */
	BrushTftLcdRgb565LE(void) __attribute__((optimize("-O1")));

	/**
	 * @brief Virtual destructor.
	 * @author Yoon-Ki Hong
	 */
	virtual ~BrushTftLcdRgb565LE(void) __attribute__((optimize("-O1")));

	/**
	 * @brief Associates a working `FrameBufferRgb565LE` used by the LCD driver to accelerate block fills.
	 * @param[in] obj Reference to the `FrameBufferRgb565LE` object.
	 * @author Yoon-Ki Hong
	 * @note Only the address of `obj` is stored; `obj` must have memory allocated via `malloc()`.
	 */
	void setFrameBuffer(FrameBufferRgb565LE &obj) __attribute__((optimize("-O1")));

	/**
	 * @brief Sets the foreground brush color from a `Color` object and updates the cached 16-bit LE code.
	 * @param[in] color New foreground brush color.
	 * @author Yoon-Ki Hong
	 */
	void setBrushColor(Color color) override __attribute__((optimize("-O1")));

	/**
	 * @brief Sets the foreground brush color from individual RGBA components.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`).
	 * @author Yoon-Ki Hong
	 */
	void setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override __attribute__((optimize("-O1")));

	/**
	 * @brief Gets the current foreground brush color.
	 * @return Current foreground `Color` object.
	 * @author Yoon-Ki Hong
	 */
	Color getBrushColor(void) override __attribute__((optimize("-O1")));

	/**
	 * @brief Sets the background color from a `Color` object.
	 * @param[in] color New background color.
	 * @author Yoon-Ki Hong
	 */
	void setBackgroundColor(Color color) override __attribute__((optimize("-O1")));

	/**
	 * @brief Sets the background color from individual RGBA components.
	 * @param[in] red   Red component (`0` ~ `255`).
	 * @param[in] green Green component (`0` ~ `255`).
	 * @param[in] blue  Blue component (`0` ~ `255`).
	 * @param[in] alpha Alpha component (`0` ~ `255`, default `0xFF`).
	 * @author Yoon-Ki Hong
	 */
	void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) override __attribute__((optimize("-O1")));

	/**
	 * @brief Gets the current background color.
	 * @return Current background `Color` object.
	 * @author Yoon-Ki Hong
	 */
	Color getBackgroundColor(void) override __attribute__((optimize("-O1")));

protected :
	ColorRgb565LE mBrushColor; ///< Active foreground color in Little-Endian RGB565 format.
	ColorRgb565LE mBgColor;    ///< Active background color in Little-Endian RGB565 format.
	ColorRgb565LE mFontColor;  ///< Active font color in Little-Endian RGB565 format.
	uint16_t mBrushColorCode;  ///< Cached 16-bit Little-Endian RGB565 code for `mBrushColor`.
	FrameBufferRgb565LE *mFb;  ///< Pointer to an optional working `FrameBufferRgb565LE` buffer.
};

#endif
