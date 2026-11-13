/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file BitmapDrawingCalculator.h
 * @brief Clipping and buffer offset calculator for blitting bitmaps onto a target canvas region.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_BITMAP_DRAWING_CALCULATOR__H_
#define YSS_GUI_BITMAP_DRAWING_CALCULATOR__H_

#include <stdint.h>
#include <yss/Area.h>

/**
 * @class BitmapDrawingCalculator
 * @brief Utility class that computes clipped source/destination areas and pixel buffer offsets when drawing a bitmap.
 *
 * @details
 * When blitting a bitmap (`bitmapSrcArea`) into a specific clipping region (`canvasDesArea`) of a canvas
 * of size `canvasSize`, parts of the destination region or the bitmap itself may fall outside the valid
 * canvas bounds or outside the clipping region.
 *
 * `BitmapDrawingCalculator` performs 2D intersection and clipping between:
 * 1. The full canvas bounds (`0, 0` to `canvasSize`),
 * 2. The target clipping rectangle on the canvas (`canvasDesArea`), and
 * 3. The bitmap rectangle placed on the canvas (`bitmapSrcArea`).
 *
 * After calling `calculate()`, callers can retrieve the trimmed dimensions, clipped areas, and linear
 * pixel offsets into both the source bitmap data array and the destination canvas framebuffer.
 *
 * ### Example Usage
 * @code
 * Area bitmapArea = {bitmapPos, {bitmap.width, bitmap.height}};
 * BitmapDrawingCalculator bdc(canvasSize, canvasDesArea, bitmapArea);
 *
 * if (bdc.calculate() == false)
 *     return; // Bitmap does not intersect the visible destination area
 *
 * uint16_t bwidth = bdc.getTrimedBitmapWidth();
 * uint16_t bheight = bdc.getTrimedBitmapHeight();
 * uint16_t *des = &frameBuffer[bdc.getCanvasDesFrameBufferOffset()];
 * uint16_t *src = &bitmapData[bdc.getBitmapSrcFrameBufferOffset()];
 * @endcode
 */
class BitmapDrawingCalculator
{
public:
	/**
	 * @brief Constructs an uninitialized BitmapDrawingCalculator instance.
	 * @author Yoon-Ki Hong
	 */
	BitmapDrawingCalculator(void);

	/**
	 * @brief Constructs a BitmapDrawingCalculator with the given canvas size, destination clipping area, and bitmap area.
	 * @param[in] canvasSize    Total width and height of the target canvas in pixels.
	 * @param[in] canvasDesArea Target clipping rectangle on the canvas where drawing is permitted.
	 * @param[in] bitmapSrcArea Bounding rectangle of the bitmap placed in canvas coordinates.
	 * @author Yoon-Ki Hong
	 */
	BitmapDrawingCalculator(Size &canvasSize, Area &canvasDesArea, Area &bitmapSrcArea);
	
	/**
	 * @brief Calculates the clipped source and destination rectangles and linear pixel offsets.
	 * @return `true` if the bitmap and destination clipping area overlap within the canvas; `false` if nothing is visible.
	 * @author Yoon-Ki Hong
	 * @post On returning `true`, trimmed areas and buffer offsets can be queried through the getter methods.
	 */
	bool calculate(void);

	/**
	 * @brief Gets the linear pixel offset from the beginning of the source bitmap buffer to the first visible pixel.
	 * @return Source buffer offset in pixels (multiply by bytes-per-pixel for byte offset).
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	uint32_t getBitmapSrcFrameBufferOffset(void);

	/**
	 * @brief Gets the linear pixel offset from the beginning of the destination canvas buffer to the first drawn pixel.
	 * @return Destination buffer offset in pixels (multiply by bytes-per-pixel for byte offset).
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	uint32_t getCanvasDesFrameBufferOffset(void);

	/**
	 * @brief Gets the width of the visible (clipped) portion of the bitmap.
	 * @return Trimmed bitmap width in pixels.
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	uint16_t getTrimedBitmapWidth(void);

	/**
	 * @brief Gets the height of the visible (clipped) portion of the bitmap.
	 * @return Trimmed bitmap height in pixels.
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	uint16_t getTrimedBitmapHeight(void);

	/**
	 * @brief Gets a reference to the clipped destination area on the canvas.
	 * @return Reference to the trimmed canvas destination `Area`.
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	Area &getTrimedCanvasDesArea(void);

	/**
	 * @brief Gets the total number of pixels in the clipped destination area (`width * height`).
	 * @return Total pixel count of the trimmed canvas destination area.
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	uint32_t getTrimedCanvasDesAreaValue(void);

	/**
	 * @brief Gets a reference to the clipped source bitmap area.
	 * @return Reference to the trimmed bitmap source `Area`.
	 * @author Yoon-Ki Hong
	 * @pre `calculate()` must have been called and returned `true`.
	 */
	Area &getTrimedBitmapSrcArea(void);

private :
	Size mCanvasSize;                  ///< Total dimensions of the target canvas.
	Area mCanvasDesArea;               ///< Requested destination clipping area on the canvas.
	Area mBitmapSrcArea;               ///< Requested bitmap placement area on the canvas.

	Area mTrimedCanvasDesArea;         ///< Clipped destination area within the canvas.
	Area mTrimedBitmapSrcArea;         ///< Clipped source bitmap area.
	uint32_t mSrcFrameBufferOffset;    ///< Linear pixel offset into the source bitmap buffer.
	uint32_t mDesFrameBufferOffset;    ///< Linear pixel offset into the destination canvas buffer.
};

#endif
