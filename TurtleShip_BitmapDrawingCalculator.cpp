/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "BitmapDrawingCalculator.h"

BitmapDrawingCalculator::BitmapDrawingCalculator(void)
{
}

BitmapDrawingCalculator::BitmapDrawingCalculator(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapSrcArea)
{
	mCanvasSize = canvasSize;
	mCanvasDesArea = canvasDesArea;
	mBitmapSrcArea = bitmapSrcArea;
}

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
	
	mDesFrameBufferOffset = mSrcFrameBufferOffset = 0;
	btw = baw;
	mTrimedCanvasDesArea = mCanvasDesArea;
	mTrimedBitmapSrcArea = mBitmapSrcArea;

	if(cax > cw  || cay > ch)
		return false;

	if(bx + baw < cax || by + bah < cay)
		return false;

	if(cax + caw < bx || cay + cah < by)
		return false;
	
	// 캔버스 작업영역의 x축 시작 위치가 0보다 작을 경우  v
	if(cax < 0)
	{
		buf = cax * -1;
		mTrimedCanvasDesArea.subWidth(buf);
		caw -= buf;
		mTrimedCanvasDesArea.setX(0);
		cax = 0;
	}
	
	// 캔버스 작업영역의 x축 끝 위치가 캔버스 크기를 벗어나는 경우
	if(cax + caw > cw)
	{
		buf = cax + caw - cw;
		mTrimedCanvasDesArea.subWidth(buf);
		caw -= buf;
	}
	
	// 캔버스 작업영역의 y축 시작 위치가 0보다 작을 경우 v
	if(cay < 0)
	{
		buf = cay * -1;
		mTrimedCanvasDesArea.subHeight(buf);
		cah -= buf;
		mTrimedCanvasDesArea.setY(0);
		cay = 0;
	}
	
	// 캔버스 작업영역의 y축 끝 위치가 캔버스 크기를 벗어나는 경우
	if(cay + cah > ch)
	{
		buf = cay + cah - ch;
		mTrimedCanvasDesArea.subHeight(buf);
		cah -= buf;
	}
	
	// 이미지 영역의 x축 시작 위치가 캔버스 작업영역의 x축 시작 위치보다 작을 경우 v
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
	{	// 같거나 클 경우
		mTrimedBitmapSrcArea.setX(bx - cax);
		mDesFrameBufferOffset += bx;
	}
	
	// 이미지 영역의 x축 끝 위치가 캔버스 작업 영역의 x축 끝 위치보다 클 경우
	if(bx + btw > cax + caw)
	{
		buf = btw + bx - caw - cax;
		mTrimedBitmapSrcArea.subWidth(buf);
		btw -= buf;
	}
	else // v
	{
		buf = caw + cax - btw - bx;
		mTrimedCanvasDesArea.subWidth(buf);
		caw -= buf;
	}
	
	// 이미지 영역의 y축 시작 위치가 캔버스 작업영역의 y축 시작 위치보다 작을 경우 v
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
	{
		mTrimedBitmapSrcArea.setY(by - cay);
		mDesFrameBufferOffset += cw * by;
	}

	// 이미지 영역의 y축 끝 위치가 캔버스 작업 영역의 y축 끝 위치보다 클 경우
	if(by + bth > cay + cah)
	{
		buf = bth + by - cah - cay;
		mTrimedBitmapSrcArea.subHeight(buf);
		bth -= buf;
	}
	else // v
	{
		buf = cah + cay - bth - by;
		mTrimedCanvasDesArea.subHeight(buf);
		cah -= buf;
	}

	return true;
}

uint32_t BitmapDrawingCalculator::getBitmapSrcFrameBufferOffset(void)
{
	return mSrcFrameBufferOffset;
}

uint32_t BitmapDrawingCalculator::getCanvasDesFrameBufferOffset(void)
{
	return mDesFrameBufferOffset;
}

uint16_t BitmapDrawingCalculator::getTrimedBitmapWidth(void)
{
	return mTrimedBitmapSrcArea.getSize().getWidth();
}

uint16_t BitmapDrawingCalculator::getTrimedBitmapHeight(void)
{
	return mTrimedBitmapSrcArea.getSize().getHeight();
}

Rectangular &BitmapDrawingCalculator::getTrimedCanvasDesArea(void)
{
	return mTrimedCanvasDesArea;
}

uint32_t BitmapDrawingCalculator::getTrimedCanvasDesAreaValue(void)
{
	return mTrimedCanvasDesArea.getSize().getWidth() * mTrimedCanvasDesArea.getSize().getHeight();
}

Rectangular &BitmapDrawingCalculator::getTrimedBitmapSrcArea(void)
{
	return mTrimedBitmapSrcArea;
}

