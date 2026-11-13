/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file FrameBuffer.h
 * @brief Abstract base class for heap-allocated off-screen drawing canvases.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_FRAME_BUFFER__H_
#define YSS_GUI_FRAME_BUFFER__H_

#include <stdint.h>
#include "TurtleShipTypes.h"
#include "Brush.h"

class Size;
class Area;

/**
 * @class FrameBuffer
 * @brief Heap-allocated off-screen pixel buffer that can be drawn onto through the `Brush` interface.
 *
 * @details
 * `FrameBuffer` manages a contiguous block of RAM allocated once via `malloc(maxPixelPoints)` for a
 * given maximum pixel capacity. Afterward, the logical canvas dimensions (`mSize`) can be resized
 * dynamically at zero allocation cost via `setSize()` as long as `width * height <= mMaxPixelCount`.
 *
 * Concrete subclasses (`FrameBufferRgb565LE`, `FrameBufferRgb565BE`, `FrameBufferArgb1555`, etc.)
 * define the pixel format, bytes per pixel (`getPixelCapacity()`), pixel plotting (`drawDot()`,
 * `blendDot()`), and export a `bitmap_t` descriptor via `getBitmap()` that can be blitted directly
 * to a `TftLcdDriver` or parent `Brush` to prevent screen flickering.
 *
 * @note Memory is allocated from the L-heap (`lmalloc` / `lfree`) when `YSS_L_HEAP_USE` is enabled,
 *       or from the H-heap (`hmalloc` / `hfree`) otherwise.
 *
 * ### Example Usage in User Application
 * @code
 * #include <bsp.h>
 * #include <font/Ubuntu_14_B.h>
 *
 * FrameBufferRgb565LE brush;
 *
 * void initAndDrawBanner(void)
 * {
 *     // Allocate buffer once for up to 5000 pixels (10,000 bytes in RGB565)
 *     brush.malloc(5000);
 *
 *     // Dynamically resize the logical canvas within the 5000-pixel budget (240 * 20 = 4800 <= 5000)
 *     brush.setSize(240, 20);
 *     brush.setBackgroundColor({0xFF, 0xFF, 0xFF});
 *     brush.setBrushColor({0x00, 0x00, 0x00});
 *     brush.setFont(Font_Ubuntu_14_B);
 *
 *     brush.clear();
 *     brush.drawString(Brush::ALIGN_CENTER_MID, "Powered by yss OS");
 *     lcd.drawBitmap({0, 280}, brush.getBitmap());
 * }
 * @endcode
 */
class FrameBuffer : public Brush
{
public:
	/**
	 * @brief Constructs an empty `FrameBuffer` with no memory allocated (`mFrameBuffer = nullptr`).
	 * @author Yoon-Ki Hong
	 */
	FrameBuffer(void);

	/**
	 * @brief Virtual destructor. Frees `mFrameBuffer` if it was allocated by `malloc()`.
	 * @author Yoon-Ki Hong
	 */
	virtual ~FrameBuffer(void);

	/**
	 * @brief Returns the number of bytes used to store a single pixel.
	 * @return Bytes per pixel of the concrete pixel format (e.g. `2` for RGB565).
	 * @author Yoon-Ki Hong
	 */
	virtual uint8_t getPixelCapacity(void) = 0;

	/**
	 * @brief Returns a `bitmap_t` descriptor referencing this frame buffer's memory and current `Size`.
	 * @return `bitmap_t` containing the current width, height, pixel format, and `mFrameBuffer` pointer.
	 * @author Yoon-Ki Hong
	 * @note The returned `bitmap_t` does not copy or own the pixel buffer; it points directly to `mFrameBuffer`.
	 */
	virtual bitmap_t getBitmap(void) = 0;

	/**
	 * @brief Sets the logical canvas dimensions from a `Size` object.
	 * @param[in] size Requested canvas width and height in pixels.
	 * @return `true` if `size.getWidth() * size.getHeight() <= mMaxPixelCount`; `false` otherwise (size unchanged).
	 * @author Yoon-Ki Hong
	 * @pre `malloc()` must have been called with sufficient pixel capacity.
	 */
	bool setSize(Size size);

	/**
	 * @brief Sets the logical canvas dimensions from explicit width and height values.
	 * @param[in] width  Requested canvas width in pixels.
	 * @param[in] height Requested canvas height in pixels.
	 * @return `true` if `width * height <= mMaxPixelCount`; `false` otherwise (size unchanged).
	 * @author Yoon-Ki Hong
	 * @pre `malloc()` must have been called with sufficient pixel capacity.
	 */
	bool setSize(uint16_t width, uint16_t height);

	/**
	 * @brief Sets the logical canvas dimensions from the `Size` component of an `Area`.
	 * @param[in] area `Area` whose size is applied (its position is ignored).
	 * @return `true` if the area's pixel count fits within `mMaxPixelCount`; `false` otherwise.
	 * @author Yoon-Ki Hong
	 * @pre `malloc()` must have been called with sufficient pixel capacity.
	 */
	bool setSize(Area area);

	/**
	 * @brief Returns the current logical canvas dimensions.
	 * @return Current `Size` (width and height in pixels).
	 * @author Yoon-Ki Hong
	 */
	Size getSize(void);

	/**
	 * @brief Allocates (or reallocates) the underlying heap buffer for `maxPixelPoints` pixels.
	 * @param[in] maxPixelPoints Maximum number of pixels the frame buffer must hold.
	 * @author Yoon-Ki Hong
	 * @note Any previously allocated buffer is freed first. The total byte allocation is
	 *       `maxPixelPoints * getPixelCapacity()`.
	 * @post On success, `mFrameBuffer` points to the allocated block, `mMaxPixelCount` equals
	 *       `maxPixelPoints`, and `mMallocFlag` is `true`. On failure, `mFrameBuffer` is `nullptr`
	 *       and `mMallocFlag` is `false`.
	 */
	void malloc(uint32_t maxPixelPoints);

	/**
	 * @brief Returns the raw byte pointer to the allocated pixel buffer.
	 * @return Pointer to the first byte of `mFrameBuffer`, or `nullptr` if not allocated.
	 * @author Yoon-Ki Hong
	 */
	uint8_t* getFrameBuffer(void);

	/**
	 * @brief Returns the maximum number of pixels that the currently allocated buffer can hold.
	 * @return Maximum pixel count set by the last successful `malloc()`, or `0` if unallocated.
	 * @author Yoon-Ki Hong
	 */
	uint32_t getMaxPixelCount(void);
	
protected:
	uint8_t *mFrameBuffer;		///< Pointer to the allocated pixel memory (`nullptr` if not allocated).
	bool mMallocFlag;			///< `true` if `mFrameBuffer` is owned and must be freed on destruction/reallocation.
	Size mSize;					///< Current logical canvas dimensions in pixels.
	uint32_t mMaxPixelCount;	///< Maximum number of pixels that fit into `mFrameBuffer`.
};

#endif
