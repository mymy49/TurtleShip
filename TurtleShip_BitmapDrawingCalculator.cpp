/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_BitmapDrawingCalculator.cpp
 * @brief Implementation of the BitmapDrawingCalculator class for 2D bitmap clipping and offset calculation.
 */

#include "BitmapDrawingCalculator.h"

/**
 * @details Default constructor. Does not initialize geometry fields; callers must initialize
 * the instance via the parameterized constructor before invoking `calculate()`.
 */
BitmapDrawingCalculator::BitmapDrawingCalculator(void)
{
}

/**
 * @details Stores the target canvas dimensions, the destination clipping rectangle on the canvas,
 * and the bounding rectangle of the source bitmap in canvas coordinates for subsequent clipping calculations.
 * @see BitmapDrawingCalculator::calculate()
 */
BitmapDrawingCalculator::BitmapDrawingCalculator(Size &canvasSize, Area &canvasDesArea, Area &bitmapSrcArea)
{
	mCanvasSize = canvasSize;
	mCanvasDesArea = canvasDesArea;
	mBitmapSrcArea = bitmapSrcArea;
}

/**
 * @details Computes the intersection between the canvas bounds, `mCanvasDesArea`, and `mBitmapSrcArea`.
 *
 * The algorithm proceeds in four main stages:
 * -# **Early rejection**: Returns `false` if the destination area starts beyond the canvas dimensions
 *    or if the bitmap rectangle and destination clipping rectangle do not overlap at all.
 * -# **Canvas boundary clamping**: Clamps `mTrimedCanvasDesArea` so that negative X/Y coordinates are
 *    shifted to 0 and right/bottom edges exceeding `mCanvasSize` are reduced to fit within the canvas.
 * -# **Horizontal intersection (X-axis)**:
 *    - If the bitmap starts to the left of the destination area (`bx < cax`), advances the source pixel
 *      offset (`mSrcFrameBufferOffset`) by `cax - bx` and reduces the trimmed bitmap width.
 *    - Otherwise, sets the relative X coordinate of `mTrimedBitmapSrcArea` and advances the destination
 *      buffer offset (`mDesFrameBufferOffset`) by `bx`.
 *    - Clips the right edge of whichever area extends further to the right.
 * -# **Vertical intersection (Y-axis)**:
 *    - If the bitmap starts above the destination area (`by < cay`), advances `mSrcFrameBufferOffset`
 *      by `(cay - by) * baw` (skipping full bitmap rows) and reduces the trimmed bitmap height.
 *    - Otherwise, sets the relative Y coordinate of `mTrimedBitmapSrcArea` and advances
 *      `mDesFrameBufferOffset` by `cw * by` (skipping full canvas rows).
 *    - Clips the bottom edge of whichever area extends further downward.
 */
bool BitmapDrawingCalculator::calculate(void)
{
	int16_t cax = mCanvasDesArea.getPosition().getX();
	int16_t cay = mCanvasDesArea.getPosition().getY();
	uint16_t caw = mCanvasDesArea.getSize().getWidth();
	uint16_t cah = mCanvasDesArea.getSize().getHeight();
	uint16_t cw = mCanvasSize.getWidth();
	uint16_t ch = mCanvasSize.getHeight();
	int16_t bx = mBitmapSrcArea.getPosition().getX();
	int16_t by = mBitmapSrcArea.getPosition().getY();
	uint16_t baw = mBitmapSrcArea.getSize().getWidth();
	uint16_t bah = mBitmapSrcArea.getSize().getHeight();
	uint16_t bth = bah;
	uint16_t btw = baw;
	int32_t buf;
	
	// Reset calculated offsets and initialize trimmed areas from the input areas
	mDesFrameBufferOffset = mSrcFrameBufferOffset = 0;
	btw = baw;
	mTrimedCanvasDesArea = mCanvasDesArea;
	mTrimedBitmapSrcArea = mBitmapSrcArea;

	// Reject if the destination area starts outside the right or bottom edge of the canvas
	if(cax > cw  || cay > ch)
		return false;

	// Reject if the bitmap lies completely to the left of or above the destination area
	if(bx + baw < cax || by + bah < cay)
		return false;

	// Reject if the bitmap lies completely to the right of or below the destination area
	if(cax + caw < bx || cay + cah < by)
		return false;
	
	// Clamp when the X start position of the canvas working area is less than 0
	if(cax < 0)
	{
		buf = cax * -1;
		mTrimedCanvasDesArea.subWidth(buf);
		caw -= buf;
		mTrimedCanvasDesArea.setX(0);
		cax = 0;
	}
	
	// Clamp when the X end position of the canvas working area exceeds the canvas width
	if(cax + caw > cw)
	{
		buf = cax + caw - cw;
		mTrimedCanvasDesArea.subWidth(buf);
		caw -= buf;
	}
	
	// Clamp when the Y start position of the canvas working area is less than 0
	if(cay < 0)
	{
		buf = cay * -1;
		mTrimedCanvasDesArea.subHeight(buf);
		cah -= buf;
		mTrimedCanvasDesArea.setY(0);
		cay = 0;
	}
	
	// Clamp when the Y end position of the canvas working area exceeds the canvas height
	if(cay + cah > ch)
	{
		buf = cay + cah - ch;
		mTrimedCanvasDesArea.subHeight(buf);
		cah -= buf;
	}
	
	// Clip when the X start position of the bitmap area is to the left of the canvas working area
	if(bx < cax)
	{
		buf = cax - bx;
		mSrcFrameBufferOffset += buf;
		mTrimedBitmapSrcArea.subWidth(buf);
		btw -= buf;
		bx = cax;
//		mTrimedBitmapSrcArea.setX(cax);
	}
	else 
	{	// When the bitmap X start position is greater than or equal to the canvas working area X start
		mTrimedBitmapSrcArea.setX(bx - cax);
		mDesFrameBufferOffset += bx;
	}
	
	// Clip when the X end position of the bitmap area extends past the canvas working area
	if(bx + btw > cax + caw)
	{
		buf = btw + bx - caw - cax;
		mTrimedBitmapSrcArea.subWidth(buf);
		btw -= buf;
	}
	else // Trim the destination working area width to match the bitmap right edge
	{
		buf = caw + cax - btw - bx;
		mTrimedCanvasDesArea.subWidth(buf);
		caw -= buf;
	}
	
	// Clip when the Y start position of the bitmap area is above the canvas working area
	if(by < cay)
	{
		buf = cay - by;
		mSrcFrameBufferOffset += buf * baw;
		mTrimedBitmapSrcArea.subHeight(buf);
		bth -= buf;
		by = cay;
		//mTrimedBitmapSrcArea.setY(cay);
	}
	else
	{	// When the bitmap Y start position is greater than or equal to the canvas working area Y start
		mTrimedBitmapSrcArea.setY(by - cay);
		mDesFrameBufferOffset += cw * by;
	}

	// Clip when the Y end position of the bitmap area extends below the canvas working area
	if(by + bth > cay + cah)
	{
		buf = bth + by - cah - cay;
		mTrimedBitmapSrcArea.subHeight(buf);
		bth -= buf;
	}
	else // Trim the destination working area height to match the bitmap bottom edge
	{
		buf = cah + cay - bth - by;
		mTrimedCanvasDesArea.subHeight(buf);
		cah -= buf;
	}

	return true;
}

/**
 * @details Returns the linear pixel offset (`mSrcFrameBufferOffset`) into the source bitmap buffer
 * corresponding to the top-left visible pixel after clipping.
 * @see BitmapDrawingCalculator::calculate()
 */
uint32_t BitmapDrawingCalculator::getBitmapSrcFrameBufferOffset(void)
{
	return mSrcFrameBufferOffset;
}

/**
 * @details Returns the linear pixel offset (`mDesFrameBufferOffset`) into the destination canvas
 * frame buffer corresponding to the top-left drawn pixel after clipping.
 * @see BitmapDrawingCalculator::calculate()
 */
uint32_t BitmapDrawingCalculator::getCanvasDesFrameBufferOffset(void)
{
	return mDesFrameBufferOffset;
}

/**
 * @details Returns the width in pixels of the clipped bitmap region (`mTrimedBitmapSrcArea`).
 */
uint16_t BitmapDrawingCalculator::getTrimedBitmapWidth(void)
{
	return mTrimedBitmapSrcArea.getSize().getWidth();
}

/**
 * @details Returns the height in pixels of the clipped bitmap region (`mTrimedBitmapSrcArea`).
 */
uint16_t BitmapDrawingCalculator::getTrimedBitmapHeight(void)
{
	return mTrimedBitmapSrcArea.getSize().getHeight();
}

/**
 * @details Returns a reference to `mTrimedCanvasDesArea`, representing the clipped destination region on the canvas.
 */
Area &BitmapDrawingCalculator::getTrimedCanvasDesArea(void)
{
	return mTrimedCanvasDesArea;
}

/**
 * @details Multiplies the width and height of `mTrimedCanvasDesArea` to compute the total number of
 * visible pixels to be drawn.
 */
uint32_t BitmapDrawingCalculator::getTrimedCanvasDesAreaValue(void)
{
	return mTrimedCanvasDesArea.getSize().getWidth() * mTrimedCanvasDesArea.getSize().getHeight();
}

/**
 * @details Returns a reference to `mTrimedBitmapSrcArea`, representing the clipped source bitmap region.
 */
Area &BitmapDrawingCalculator::getTrimedBitmapSrcArea(void)
{
	return mTrimedBitmapSrcArea;
}
