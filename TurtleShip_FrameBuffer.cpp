/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_FrameBuffer.cpp
 * @brief Implementation of the FrameBuffer base class for heap-managed off-screen pixel buffers.
 * @author Yoon-Ki Hong
 * @date 2025
 */

#include "FrameBuffer.h"
#include <yss/Size.h>
#include <config.h>
#include <yss/lmalloc.h>
#include <yss/hmalloc.h>

/**
 * @details Initializes an empty `FrameBuffer` with `mMallocFlag = false`, `mFrameBuffer = nullptr`,
 * and `mMaxPixelCount = 0`.
 * @see FrameBuffer
 * @todo None.
 * @bug None.
 */
FrameBuffer::FrameBuffer(void)
{
	mMallocFlag = false;
	mFrameBuffer = nullptr;
	mMaxPixelCount = 0;
}

/**
 * @details Frees `mFrameBuffer` via `lfree()` (when `YSS_L_HEAP_USE` is active) or `hfree()`
 * if `mMallocFlag` is `true`.
 * @see FrameBuffer::malloc()
 * @todo None.
 * @bug None.
 */
FrameBuffer::~FrameBuffer(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

/**
 * @details Checks whether `size.getWidth() * size.getHeight()` fits within `mMaxPixelCount`.
 * If so, updates `mSize` and returns `true`; otherwise leaves `mSize` unchanged and returns `false`.
 * @see FrameBuffer::malloc()
 * @todo None.
 * @bug None.
 */
bool FrameBuffer::setSize(Size size)
{
	if(size.getWidth() * size.getHeight() <= mMaxPixelCount)
	{
		mSize = size;
		return true;
	}
	else 
	{
		return false;
	}
}

/**
 * @details Checks whether `width * height` fits within `mMaxPixelCount`.
 * If so, updates `mSize` via `mSize.setSize(width, height)` and returns `true`; otherwise returns `false`.
 * @see FrameBuffer::malloc()
 * @todo None.
 * @bug None.
 */
bool FrameBuffer::setSize(uint16_t width, uint16_t height)
{
	if(width * height <= mMaxPixelCount)
	{
		mSize.setSize(width, height);
		return true;
	}
	else 
	{
		return false;
	}
}

/**
 * @details Extracts the `Size` from `area` (`area.getSize()`) and delegates to `setSize(Size)`.
 * @see FrameBuffer::setSize(Size)
 * @todo None.
 * @bug None.
 */
bool FrameBuffer::setSize(Area area)
{
	return setSize(area.getSize());
}

/**
 * @details Allocates a pixel buffer capable of holding `maxPixelPoints` pixels.
 *
 * -# Queries bytes-per-pixel via `getPixelCapacity()`.
 * -# Frees any previously allocated buffer if `mMallocFlag` is `true`.
 * -# Allocates `maxPixelPoints * getPixelCapacity()` bytes from the L-heap (`lmalloc`) or H-heap (`hmalloc`).
 * -# Updates `mMaxPixelCount` and `mMallocFlag` according to whether the allocation succeeded.
 * @see FrameBuffer
 * @todo None.
 * @bug None.
 */
void FrameBuffer::malloc(uint32_t maxPixelPoints)
{
	uint32_t size = getPixelCapacity();

	// Free existing allocation before reallocating
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif

	// Allocate new pixel buffer from configured yss OS heap
	size *= maxPixelPoints;
#if YSS_L_HEAP_USE
	mFrameBuffer = (uint8_t*)lmalloc(size);
#else
	mFrameBuffer = (uint8_t*)hmalloc(size);
#endif

	if(mFrameBuffer != nullptr)
	{
		mMaxPixelCount = maxPixelPoints;
		mMallocFlag = true;
	}
	else
		mMallocFlag = false;
}

/**
 * @details Returns the raw byte pointer `mFrameBuffer`.
 * @see FrameBuffer
 * @todo None.
 * @bug None.
 */
uint8_t* FrameBuffer::getFrameBuffer(void)
{
	return mFrameBuffer;
}

/**
 * @details Returns the maximum pixel capacity `mMaxPixelCount` of the allocated buffer.
 * @see FrameBuffer
 * @todo None.
 * @bug None.
 */
uint32_t FrameBuffer::getMaxPixelCount(void)
{
	return mMaxPixelCount;
}

/**
 * @details Returns the current logical canvas dimensions `mSize`.
 * @see FrameBuffer
 * @todo None.
 * @bug None.
 */
Size FrameBuffer::getSize(void)
{
	return mSize;
}
