/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file FrameBufferRgb565LE.h
 * @brief Little-Endian 16-bit RGB565 off-screen frame buffer class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565LE__H_
#define YSS_GUI_FRAME_BUFFER_RGB565LE__H_

#include "FrameBufferRgb565.h"
#include "ColorRgb565LE.h"

/**
 * @class FrameBufferRgb565LE
 * @brief Concrete 16-bit RGB565 `FrameBuffer` storing pixels in Little-Endian byte order.
 *
 * @details
 * `FrameBufferRgb565LE` is the standard off-screen drawing canvas used across yss OS applications
 * and display widgets (`ValueDisplay`, `ProgressDisplay`, `RpmBarDisplay`, `Button`, `Label`, etc.)
 * on Little-Endian ARM Cortex-M microcontrollers.
 *
 * It encodes foreground and background colors via `ColorRgb565LE` and implements fast 16-bit
 * RGB565 alpha blending in `blendDot()` for anti-aliased font rendering (`Brush::drawString()`)
 * and custom graphics.
 *
 * ### Example Usage in User Application
 * @code
 * #include <bsp.h>
 * #include <font/Ubuntu_14_B.h>
 * #include <font/Ubuntu_20_B.h>
 *
 * FrameBufferRgb565LE brush;
 *
 * void drawCalibrationPoint(void)
 * {
 *     lcd.lock();
 *
 *     // Resize off-screen canvas to 35x35 pixels, clear, and draw concentric circles
 *     brush.setSize(35, 35);
 *     brush.setBackgroundColor({0x00, 0x00, 0x00});
 *     brush.clear();
 *     brush.setBrushColor(0x80, 0x80, 0x80);
 *     brush.fillCircle({5, 5}, 16);
 *     brush.setBrushColor(0xFF, 0x30, 0x30);
 *     brush.fillCircle({5, 5}, 5);
 *
 *     // Transfer the off-screen bitmap to the top-left corner of the LCD
 *     lcd.drawBitmap({0, 0}, brush.getBitmap());
 *
 *     lcd.unlock();
 * }
 * @endcode
 */
class FrameBufferRgb565LE : public FrameBufferRgb565
{
public :
	/**
	 * @brief Constructs a `FrameBufferRgb565LE` instance.
	 * @author Yoon-Ki Hong
	 */
	FrameBufferRgb565LE(void) __attribute__((optimize("-O1")));

	/**
	 * @brief Destructor. Frees the pixel buffer if allocated.
	 * @author Yoon-Ki Hong
	 */
	~FrameBufferRgb565LE(void) override __attribute__((optimize("-O1")));

	/**
	 * @brief Alpha-blends the current brush color onto the Little-Endian RGB565 pixel at `(x, y)`.
	 * @param[in] x     X coordinate of the pixel (`0` ~ `width - 1`).
	 * @param[in] y     Y coordinate of the pixel (`0` ~ `height - 1`).
	 * @param[in] alpha Blending weight (`0` ~ `255`; values `< 16` are ignored as transparent).
	 * @author Yoon-Ki Hong
	 */
	void blendDot(int16_t x, int16_t y, uint8_t alpha) override __attribute__((optimize("-O1")));
	
	/**
	 * @brief Sets the foreground brush color from a `Color` object and caches its Little-Endian 16-bit code.
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

private :
	ColorRgb565LE mBrushColor; ///< Active foreground color in Little-Endian RGB565 format.
	ColorRgb565LE mBgColor;    ///< Active background color in Little-Endian RGB565 format.
	ColorRgb565LE mFontColor;  ///< Active font color in Little-Endian RGB565 format.
};

#endif
