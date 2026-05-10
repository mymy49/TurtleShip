/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <gui/SystemOutputFrameBuffer.h>

OutputFrameBuffer::OutputFrameBuffer(void)
{
	//mFrame = 0;
	//mFrameBuffer = new YSS_OUTPUT_FRAME_BUFFER;
}

OutputFrameBuffer::~OutputFrameBuffer(void)
{
	//delete mFrameBuffer;
}

void OutputFrameBuffer::update(Rectangular rect)
{
	//mFrameBuffer->drawObjectToPartialArea(pos, size, mFrame);	
}

void OutputFrameBuffer::update(Rectangular before, Rectangular current)
{
	//mFrameBuffer->drawObjectToPartialArea(beforePos, beforeSize, mFrame);	
	//mFrameBuffer->drawObjectToPartialArea(currentPos, currentSize, mFrame);	
}

void OutputFrameBuffer::update(void)
{
	//mFrameBuffer->drawObjectToPartialArea({0, 0}, mFrame->getSize(), mFrame);
}

void OutputFrameBuffer::setFrame(Frame *frame)
{
	//mFrame = frame;
}

uint8_t* OutputFrameBuffer::getFrameBuffer(void)
{
	//return (uint8_t*)mFrameBuffer->getFrameBuffer();
}

#endif

