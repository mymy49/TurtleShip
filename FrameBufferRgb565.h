/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file FrameBufferRgb565.h
 * @brief Base 16-bit RGB565 off-screen frame buffer class of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_FRAME_BUFFER_RGB565__H_
#define YSS_GUI_FRAME_BUFFER_RGB565__H_

#include "FrameBuffer.h"

/**
 * @class FrameBufferRgb565
 * @brief Intermediate `FrameBuffer` class implementing common 16-bit RGB565 pixel operations and bitmap blitting.
 *
 * @details
 * `FrameBufferRgb565` implements the byte-order-independent operations of a 16-bit RGB565 frame buffer:
 * - `getPixelCapacity()` returning `2` bytes per pixel,
 * - `drawDot()` writing the cached 16-bit `mBrushColorCode` to `(x, y)`,
 * - `fillDotArray()` filling 16-bit half-words using `memsethw()`,
 * - `drawBitmapBase()` supporting both `BITMAP_TYPE_RGB565` and `BITMAP_TYPE_ARGB1555` (1-bit transparency),
 * - `getBitmap()` exporting a `BITMAP_TYPE_RGB565` descriptor pointing to `mFrameBuffer`.
 *
 * Endian-specific subclasses (`FrameBufferRgb565LE` and `FrameBufferRgb565BE`) extend this class to
 * implement color encoding and per-channel alpha blending (`blendDot()`).
 */
class FrameBufferRgb565 : public FrameBuffer
{
public :
	/**
	 * @brief Constructs a `FrameBufferRgb565` instance.
	 * @author Yoon-Ki Hong
	 */
	FrameBufferRgb565(void);

	/**
	 * @brief Destructor. Frees the pixel buffer if allocated.
	 * @author Yoon-Ki Hong
	 */
	~FrameBufferRgb565(void) override;

	/**
	 * @brief Returns the number of bytes per pixel for RGB565 (`2`).
	 * @return `2`.
	 * @author Yoon-Ki Hong
	 */
	uint8_t getPixelCapacity(void) override;

	/**
	 * @brief Draws a single 16-bit RGB565 pixel at `(x, y)` using `mBrushColorCode`.
	 * @param[in] x X coordinate of the pixel (`0` ~ `width - 1`).
	 * @param[in] y Y coordinate of the pixel (`0` ~ `height - 1`).
	 * @author Yoon-Ki Hong
	 */
	void drawDot(int16_t x, int16_t y) override;

	/**
	 * @brief Exports a `bitmap_t` descriptor of type `BITMAP_TYPE_RGB565` pointing to `mFrameBuffer`.
	 * @return `bitmap_t` descriptor with current `mSize` dimensions and `mFrameBuffer` data pointer.
	 * @author Yoon-Ki Hong
	 */
	bitmap_t getBitmap(void) override;

protected :
	uint16_t mBrushColorCode; ///< Cached 16-bit RGB565 color code for the active foreground brush color.

	/**
	 * @brief Fills `count` consecutive 16-bit pixels starting at byte `offset` in `mFrameBuffer` with `color`.
	 * @param[in] offset Starting byte offset in `mFrameBuffer` (`pixelIndex * 2`).
	 * @param[in] count  Number of 16-bit pixels to fill.
	 * @param[in] color  Fill color whose 16-bit code (`color.getCode()`) is written.
	 * @author Yoon-Ki Hong
	 */
	void fillDotArray(uint32_t offset, uint32_t count, Color color) override;

	/**
	 * @brief Draws a bitmap clipped to `canvasDesArea` at `bitmapPos` onto the RGB565 frame buffer.
	 * @param[in] canvasSize    Dimensions of the frame buffer canvas.
	 * @param[in] canvasDesArea Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos     Top-left placement coordinate of the bitmap on the canvas.
	 * @param[in] bitmap        Bitmap descriptor (`BITMAP_TYPE_RGB565` or `BITMAP_TYPE_ARGB1555`).
	 * @author Yoon-Ki Hong
	 */
	void drawBitmapBase(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap) override;

	/**
	 * @brief Draws a bitmap at `pos` onto the RGB565 frame buffer (placeholder override).
	 * @param[in] pos    Top-left placement coordinate of the bitmap on the canvas.
	 * @param[in] bitmap Bitmap descriptor (`bitmap_t`).
	 * @author Yoon-Ki Hong
	 */
	void drawBitmapBase(Position pos, const bitmap_t bitmap) override;

	/**
	 * @brief Returns the current logical canvas dimensions (`mSize`) of this frame buffer.
	 * @return Canvas `Size` in pixels.
	 * @author Yoon-Ki Hong
	 */
	Size getCanvasSize(void) override;

private :
	/**
	 * @brief Blits an RGB565 bitmap clipped to `canvasDesArea` into `mFrameBuffer`.
	 * @param[in] canvasSize    Dimensions of the frame buffer canvas.
	 * @param[in] canvasDesArea Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos     Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap        Source `BITMAP_TYPE_RGB565` bitmap.
	 * @author Yoon-Ki Hong
	 */
	void drawBitmapRgb565(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap);

	/**
	 * @brief Blits an ARGB1555 bitmap (with 1-bit MSB transparency) clipped to `canvasDesArea` into `mFrameBuffer`.
	 * @param[in] canvasSize    Dimensions of the frame buffer canvas.
	 * @param[in] canvasDesArea Destination clipping rectangle on the canvas.
	 * @param[in] bitmapPos     Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap        Source `BITMAP_TYPE_ARGB1555` bitmap.
	 * @author Yoon-Ki Hong
	 */
	void drawBitmapArgb1555(Size canvasSize, Area canvasDesArea, Position bitmapPos, const bitmap_t bitmap);
};

#endif
