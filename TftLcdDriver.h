/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TftLcdDriver.h
 * @brief Abstract base interface for communication-channel (SPI, I2C, 8080) TFT LCD controller drivers.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_SAC_TFT_LCD_DRIVER__H_
#define YSS_SAC_TFT_LCD_DRIVER__H_

#include <yss/error.h>
#include <stdint.h>
#include <yss/Mutex.h>
#include "TurtleShipTypes.h"

class Area;
class Position;
class Size;

/**
 * @class TftLcdDriver
 * @brief Abstract base class for TFT LCD drivers controllable over a communication bus (SPI, I2C, parallel, etc.).
 *
 * @details
 * Any TFT LCD driver that configures and updates a display panel over a communication channel
 * must inherit from `TftLcdDriver` and implement its pure virtual interface (`getLcdSize()` and
 * `drawBitmapBase()`).
 *
 * Because `TftLcdDriver` inherits from `Mutex`, multi-threaded user applications can safely
 * serialize concurrent screen updates by calling `lcd.lock()` and `lcd.unlock()`.
 *
 * ### Example Usage in User Application
 * @code
 * #include <bsp.h>
 * #include "../bmp/drawing.h"
 *
 * void pageDisplayMain(void*)
 * {
 *     lcd.lock();
 *
 *     lcd.setBackgroundColor({0x00, 0x00, 0x00});
 *     lcd.clear();
 *     lcd.drawBitmap({20, 20}, drawing);
 *
 *     lcd.unlock();
 * }
 * @endcode
 */
class TftLcdDriver : public Mutex
{
public:
	/**
	 * @brief Gets the physical pixel resolution of the TFT LCD panel.
	 * @return Panel `Size` (width and height in pixels).
	 * @author Yoon-Ki Hong
	 */
	virtual Size getLcdSize(void) = 0;

	/**
	 * @brief Transfers a bitmap (clipped to `canvasRect`) onto the TFT LCD at `bitmapPos`.
	 * @param[in] canvasSize Total dimensions of the target canvas.
	 * @param[in] canvasRect Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos  Top-left placement coordinate of the bitmap on the canvas.
	 * @param[in] bitmap     Bitmap descriptor (`bitmap_t`) to transfer.
	 * @author Yoon-Ki Hong
	 */
	virtual void drawBitmapBase(Size canvasSize, Area canvasRect, Position bitmapPos, const bitmap_t bitmap) = 0;
};

#endif
