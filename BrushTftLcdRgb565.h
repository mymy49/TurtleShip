/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file BrushTftLcdRgb565.h
 * @brief Intermediate `Brush` base class for 16-bit RGB565 TFT LCD drivers.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_MOD_BRUSH_TFT_LCD_RGB_565__H_
#define YSS_MOD_BRUSH_TFT_LCD_RGB_565__H_

class Size;

#include "Brush.h"

/**
 * @class BrushTftLcdRgb565
 * @brief Base `Brush` adapter for TFT LCD drivers operating in 16-bit RGB565 pixel format.
 *
 * @details
 * `BrushTftLcdRgb565` specializes `Brush` for 2-bytes-per-pixel (16-bit RGB565) TFT LCD devices.
 * Endian-specific subclasses (`BrushTftLcdRgb565LE` and `BrushTftLcdRgb565BE`) extend this class
 * to manage foreground/background `Color` encoding and optional working `FrameBuffer` association,
 * while concrete LCD controller drivers (such as `ST7789V_spi_with_Brush_RGB565` and
 * `Touch_LCD_Shield_for_Arduino_2_8_inch`) implement the hardware pixel/window transfer methods.
 */
class BrushTftLcdRgb565 : public Brush
{
public :
	/**
	 * @brief Constructs a `BrushTftLcdRgb565` object.
	 * @author Yoon-Ki Hong
	 */
	BrushTftLcdRgb565(void);

	/**
	 * @brief Virtual destructor.
	 * @author Yoon-Ki Hong
	 */
	virtual ~BrushTftLcdRgb565(void);

protected :
	uint16_t mBrushColorCode; ///< Cached 16-bit RGB565 color code for the active brush color.

	/**
	 * @brief Placeholder implementation for filling consecutive pixels in a buffer.
	 * @param[in] offset Byte offset in the buffer.
	 * @param[in] count  Number of pixels to fill.
	 * @param[in] color  Fill color.
	 * @author Yoon-Ki Hong
	 * @note Concrete TFT LCD drivers typically override `fillRectBase()` directly to stream pixels over SPI/8080 bus.
	 */
	void fillDotArray(uint32_t offset, uint32_t count, Color color) override;

	/**
	 * @brief Returns the default canvas size (`{0, 0}` unless overridden by the concrete LCD driver).
	 * @return Canvas `Size` in pixels.
	 * @author Yoon-Ki Hong
	 */
	Size getCanvasSize(void) override;

	/**
	 * @brief Returns the pixel size in bytes for RGB565 (`2` bytes per pixel).
	 * @return `2`.
	 * @author Yoon-Ki Hong
	 */
	uint8_t getPixelCapacity(void) override;
};

#endif
