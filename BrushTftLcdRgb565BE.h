/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file BrushTftLcdRgb565BE.h
 * @brief Big-Endian RGB565 TFT LCD brush class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_MOD_BRUSH_TFT_LCD_RGB_565BE__H_
#define YSS_MOD_BRUSH_TFT_LCD_RGB_565BE__H_

#include "BrushTftLcdRgb565.h"
#include "ColorRgb565BE.h"

class FrameBufferRgb565BE;

/**
 * @class BrushTftLcdRgb565BE
 * @brief `BrushTftLcdRgb565` specialization using Big-Endian 16-bit RGB565 colors (`ColorRgb565BE`).
 *
 * @details
 * Manages foreground (`mBrushColor`), background (`mBgColor`), and font (`mFontColor`) colors in
 * Big-Endian RGB565 format and optionally holds a pointer to an auxiliary `FrameBufferRgb565BE`
 * (`mFb`) used by LCD drivers to batch pixel transfers (such as solid fills) over SPI/bus interfaces.
 */
class BrushTftLcdRgb565BE : public BrushTftLcdRgb565
{
public :
	/**
	 * @brief Constructs a `BrushTftLcdRgb565BE` instance with `mFb` initialized to `nullptr`.
	 * @author Yoon-Ki Hong
	 */
	BrushTftLcdRgb565BE(void);

	/**
	 * @brief Virtual destructor.
	 * @author Yoon-Ki Hong
	 */
	virtual ~BrushTftLcdRgb565BE(void);

	/**
	 * @brief Attaches a working `FrameBufferRgb565BE` used as a transfer/scratch buffer by the LCD driver.
	 * @param[in] obj Reference to the `FrameBufferRgb565BE` instance.
	 * @author Yoon-Ki Hong
	 */
	void setFrameBuffer(FrameBufferRgb565BE &obj);

	/**
	 * @brief Sets the foreground brush color and updates the cached Big-Endian 16-bit color code.
	 * @param[in] color New foreground brush color.
	 * @author Yoon-Ki Hong
	 */
	void setBrushColor(Color color) override;

	/**
	 * @brief Gets the current foreground brush color.
	 * @return Current `Color` object.
	 * @author Yoon-Ki Hong
	 */
	Color getBrushColor(void) override;

	/**
	 * @brief Sets the background color used by `clear()` and `clearRectangular()`.
	 * @param[in] color New background color.
	 * @author Yoon-Ki Hong
	 */
	void setBackgroundColor(Color color) override;

	/**
	 * @brief Gets the current background color.
	 * @return Current background `Color` object.
	 * @author Yoon-Ki Hong
	 */
	Color getBackgroundColor(void) override;

protected :
	ColorRgb565BE mBrushColor; ///< Active foreground color in Big-Endian RGB565 format.
	ColorRgb565BE mBgColor;    ///< Active background color in Big-Endian RGB565 format.
	ColorRgb565BE mFontColor;  ///< Active font color in Big-Endian RGB565 format.
	uint16_t mBrushColorCode;  ///< Cached 16-bit Big-Endian RGB565 code for `mBrushColor`.
	FrameBufferRgb565BE *mFb;  ///< Pointer to an optional working `FrameBufferRgb565BE` buffer.
};

#endif
