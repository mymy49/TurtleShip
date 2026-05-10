/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "FrameBuffer.h"
#include "Size.h"
#include <config.h>
#include <std_ext/malloc.h>

FrameBuffer::FrameBuffer(void)
{
	mMallocFlag = false;
	mFrameBuffer = nullptr;
	mMaxPixelCount = 0;
}

FrameBuffer::FrameBuffer::~FrameBuffer(void)
{
	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif
}

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

void FrameBuffer::malloc(uint32_t maxPixelPoints)
{
	uint32_t size = getPixelCapacity();

	if(mMallocFlag)
#if YSS_L_HEAP_USE
		lfree(mFrameBuffer);
#else
		hfree(mFrameBuffer);
#endif

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

uint8_t* FrameBuffer::getFrameBuffer(void)
{
	return mFrameBuffer;
}

uint32_t FrameBuffer::getMaxPixelCount(void)
{
	return mMaxPixelCount;
}

Size FrameBuffer::getSize(void)
{
	return mSize;
}

