/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file FrameBufferRgb565BE.h
 * @brief Big-Endian 16-bit RGB565 off-screen frame buffer class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565BE__H_
#define YSS_GUI_FRAME_BUFFER_RGB565BE__H_

#include "FrameBufferRgb565.h"
#include "ColorRgb565BE.h"

/**
 * @class FrameBufferRgb565BE
 * @brief Concrete 16-bit RGB565 `FrameBuffer` storing pixels in Big-Endian byte order.
 *
 * @details
 * `FrameBufferRgb565BE` extends `FrameBufferRgb565` with Big-Endian color management (`ColorRgb565BE`)
 * and implements `blendDot()` by byte-swapping the stored 16-bit pixel, blending the Red, Green, and
 * Blue channels according to the 8-bit `alpha` weight, and byte-swapping the result back to Big-Endian.
 */
class FrameBufferRgb565BE : public FrameBufferRgb565
{
public :
	/**
	 * @brief Constructs a `FrameBufferRgb565BE` object.
	 * @author Yoon-Ki Hong
	 */
	FrameBufferRgb565BE(void);

	/**
	 * @brief Virtual destructor. Frees the frame buffer if allocated.
	 * @author Yoon-Ki Hong
	 */
	virtual ~FrameBufferRgb565BE(void);

	/**
	 * @brief Alpha-blends the current brush color onto the Big-Endian RGB565 pixel at `(x, y)`.
	 * @param[in] x     X coordinate of the pixel (`0` ~ `width - 1`).
	 * @param[in] y     Y coordinate of the pixel (`0` ~ `height - 1`).
	 * @param[in] alpha Blending weight (`0` ~ `255`; values `< 16` are ignored as transparent).
	 * @author Yoon-Ki Hong
	 */
	virtual void blendDot(int16_t x, int16_t y, uint8_t alpha) override;
	
	/**
	 * @brief Sets the foreground brush color and caches its Big-Endian 16-bit color code.
	 * @param[in] color New foreground brush color.
	 * @author Yoon-Ki Hong
	 */
	virtual void setBrushColor(Color color) override;

	/**
	 * @brief Gets the current foreground brush color.
	 * @return Current foreground `Color` object.
	 * @author Yoon-Ki Hong
	 */
	virtual Color getBrushColor(void) override;

	/**
	 * @brief Sets the background color used by `clear()` and `clearRectangular()`.
	 * @param[in] color New background color.
	 * @author Yoon-Ki Hong
	 */
	virtual void setBackgroundColor(Color color) override;

	/**
	 * @brief Gets the current background color.
	 * @return Current background `Color` object.
	 * @author Yoon-Ki Hong
	 */
	virtual Color getBackgroundColor(void) override;

protected :

private :
	ColorRgb565BE mBrushColor; ///< Active foreground color in Big-Endian RGB565 format.
	ColorRgb565BE mBgColor;    ///< Active background color in Big-Endian RGB565 format.
	ColorRgb565BE mFontColor;  ///< Active font color in Big-Endian RGB565 format.
};

#endif
