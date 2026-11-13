/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_SystemOutputFrameBuffer.cpp
 * @brief Legacy implementation stubs for the OutputFrameBuffer class.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <gui/SystemOutputFrameBuffer.h>

/**
 * @details Default constructor stub for `OutputFrameBuffer`.
 */
OutputFrameBuffer::OutputFrameBuffer(void)
{
	//mFrame = 0;
	//mFrameBuffer = new YSS_OUTPUT_FRAME_BUFFER;
}

/**
 * @details Destructor stub for `OutputFrameBuffer`.
 */
OutputFrameBuffer::~OutputFrameBuffer(void)
{
	//delete mFrameBuffer;
}

/**
 * @details Partial-area update stub for a single dirty rectangle `rect`.
 */
void OutputFrameBuffer::update(Rectangular rect)
{
	//mFrameBuffer->drawObjectToPartialArea(pos, size, mFrame);	
}

/**
 * @details Partial-area update stub for `before` and `current` dirty rectangles.
 */
void OutputFrameBuffer::update(Rectangular before, Rectangular current)
{
	//mFrameBuffer->drawObjectToPartialArea(beforePos, beforeSize, mFrame);	
	//mFrameBuffer->drawObjectToPartialArea(currentPos, currentSize, mFrame);	
}

/**
 * @details Full-screen update stub for `OutputFrameBuffer`.
 */
void OutputFrameBuffer::update(void)
{
	//mFrameBuffer->drawObjectToPartialArea({0, 0}, mFrame->getSize(), mFrame);
}

/**
 * @details Frame binding stub for `OutputFrameBuffer`.
 */
void OutputFrameBuffer::setFrame(Frame *frame)
{
	//mFrame = frame;
}

/**
 * @details Frame buffer pointer getter stub for `OutputFrameBuffer`.
 */
uint8_t* OutputFrameBuffer::getFrameBuffer(void)
{
	//return (uint8_t*)mFrameBuffer->getFrameBuffer();
}

#endif
