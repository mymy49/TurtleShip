/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file FrameBufferArgb1555.h
 * @brief Base 16-bit ARGB1555 off-screen frame buffer class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#ifndef YSS_GUI_FRAME_BUFFER_ARGB1555__H_
#define YSS_GUI_FRAME_BUFFER_ARGB1555__H_

#include "FrameBuffer.h"
#include "Brush.h"

/**
 * @class FrameBufferArgb1555
 * @brief Off-screen frame buffer class for 16-bit ARGB1555 (1-bit Alpha, 5-bit Red, 5-bit Green, 5-bit Blue) bitmaps.
 *
 * @details
 * Provides a 2-bytes-per-pixel off-screen canvas that exports `BITMAP_TYPE_ARGB1555` bitmaps via `getBitmap()`.
 * @note Currently retained as a legacy/experimental class; `FrameBufferRgb565LE` is the primary frame buffer
 *       used in active builds.
 */
class FrameBufferArgb1555 : public FrameBuffer, public Brush
{
public :
	/**
	 * @brief Constructs a `FrameBufferArgb1555` instance.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	FrameBufferArgb1555(void);

	/**
	 * @brief Virtual destructor. Frees the pixel buffer if allocated.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual ~FrameBufferArgb1555(void);

	/**
	 * @brief Returns the number of bytes per pixel for ARGB1555 (`2`).
	 * @param[in] none
	 * @return `2`.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual uint8_t getPixelCapacity(void) override;

	/**
	 * @brief Draws a single 16-bit ARGB1555 pixel at `(x, y)` using `mBrushColorCode`.
	 * @param[in] x X coordinate of the pixel.
	 * @param[in] y Y coordinate of the pixel.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void drawDot(int16_t x, int16_t y) override;

	/**
	 * @brief Returns a `bitmap_t` descriptor of type `BITMAP_TYPE_ARGB1555` pointing to `mFrameBuffer`.
	 * @param[in] none
	 * @return `bitmap_t` descriptor.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual bitmap_t getBitmap(void) override;

protected :
	uint16_t mBrushColorCode; ///< Cached 16-bit ARGB1555 color code for the active foreground brush color.

	/**
	 * @brief Fills `count` consecutive 16-bit pixels starting at byte `offset` in `mFrameBuffer` with `color`.
	 * @param[in] offset Starting byte offset in `mFrameBuffer`.
	 * @param[in] count  Number of pixels to fill.
	 * @param[in] color  Fill color.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void fillDotArray(uint32_t offset, uint32_t count, Color color) override;

	/**
	 * @brief Draws a bitmap clipped to `canvasDesArea` at `bitmapPos` onto the ARGB1555 frame buffer.
	 * @param[in] canvasSize    Dimensions of the frame buffer canvas.
	 * @param[in] canvasDesArea Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos     Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap        Bitmap descriptor (`bitmap_t`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void drawBitmapBase(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap);

	/**
	 * @brief Returns the current logical canvas dimensions (`mSize`) of this frame buffer.
	 * @param[in] none
	 * @return Canvas `Size` in pixels.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual Size getCanvasSize(void) override;

private :
	/**
	 * @brief Blits an RGB565 bitmap clipped to `canvasDesArea` into `mFrameBuffer`.
	 * @param[in] canvasSize    Dimensions of the frame buffer canvas.
	 * @param[in] canvasDesArea Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos     Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap        Source `BITMAP_TYPE_RGB565` bitmap.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawBitmapRgb565(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap);

	/**
	 * @brief Blits an ARGB1555 bitmap clipped to `canvasDesArea` into `mFrameBuffer`.
	 * @param[in] canvasSize    Dimensions of the frame buffer canvas.
	 * @param[in] canvasDesArea Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos     Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap        Source `BITMAP_TYPE_ARGB1555` bitmap.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawBitmapArgb1555(Size canvasSize, Rectangular canvasDesArea, Position bitmapPos, const bitmap_t bitmap);
};

#endif
