/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file FrameBufferArgb1555LE.h
 * @brief Little-Endian 16-bit ARGB1555 off-screen frame buffer class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565LE__H_
#define YSS_GUI_FRAME_BUFFER_RGB565LE__H_

#include "FrameBufferRgb565.h"
#include "ColorRgb565LE.h"

/**
 * @class FrameBufferArgb1555LE
 * @brief Off-screen frame buffer class for Little-Endian 16-bit ARGB1555 pixels.
 *
 * @details
 * Provides Little-Endian color management and alpha blending for 16-bit ARGB1555 buffers.
 * @note Currently retained as a legacy/experimental class; note that the header guard macro
 *       `YSS_GUI_FRAME_BUFFER_RGB565LE__H_` is shared with `FrameBufferRgb565LE.h`.
 */
class FrameBufferArgb1555LE : public FrameBufferRgb565
{
public :
	/**
	 * @brief Constructs a `FrameBufferArgb1555LE` instance.
	 * @author Yoon-Ki Hong
	 */
	FrameBufferArgb1555LE(void);

	/**
	 * @brief Virtual destructor. Frees the pixel buffer if allocated.
	 * @author Yoon-Ki Hong
	 */
	virtual ~FrameBufferArgb1555LE(void);

	/**
	 * @brief Alpha-blends the current brush color onto the pixel at `(x, y)`.
	 * @param[in] x     X coordinate of the pixel.
	 * @param[in] y     Y coordinate of the pixel.
	 * @param[in] alpha Blending weight (`0` ~ `255`).
	 * @author Yoon-Ki Hong
	 */
	virtual void blendDot(int16_t x, int16_t y, uint8_t alpha) override;
	
	/**
	 * @brief Sets the foreground brush color and caches its 16-bit color code.
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
	 * @brief Sets the background color.
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
	ColorRgb565LE mBrushColor; ///< Active foreground color.
	ColorRgb565LE mBgColor;    ///< Active background color.
	ColorRgb565LE mFontColor;  ///< Active font color.
};

#endif
