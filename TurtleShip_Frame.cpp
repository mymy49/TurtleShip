/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <gui/Frame.h>
#include <yss/instance.h>

#include <yss.h>
#include <std_ext/malloc.h>
#include <sac/PointerDevice.h>
//#include <gui/OutputFrameBuffer.h>

Frame::Frame()
{
	Size size = system::getSystemTftLcdSize();
	mOutputFrameBuffer = 0;
	mFrameBuffer->malloc(size.getWidth() * size.getHeight());
	mFrameBuffer->setSize(size);
	mResizeAble = false;
	mFrameBuffer->clear();
	mPointerDevice = nullptr;
}

Frame::~Frame(void)
{
	if(mPointerDevice)
		mPointerDevice->resetFrame(this);
	//clearActiveFrame();
}

void Frame::setPosition(Position pos)
{
	// Frame의 위치 조정 못하게 막을 목적으로 생성
	(void)pos;
}

void Frame::setPosition(int16_t x, int16_t y)
{
	// Frame의 위치 조정 못하게 막을 목적으로 생성
	(void)x;
	(void)y;
}

void Frame::setOutputFrameBuffer(OutputFrameBuffer *outputFrameBuffer)
{
	mOutputFrameBuffer = outputFrameBuffer;

	update();
}

void Frame::update(void)
{
	Object *obj;
	Rectangular rect = {mPos, mFrameBuffer->getSize()};

	mFrameBuffer->clearRectangular(rect);

	for (uint16_t i = 0; i < mNumOfObj; i++)
	{
		obj = mObjArr[i];

		if(obj->isModified())
		{
			obj->paint();
			obj->clearModifyFlag();
		}

		if (obj->isVisible())
			mFrameBuffer->drawBitmap(rect, obj->getPosition(), obj->getFrameBuffer()->getBitmap());
	}

	system::drawBitmap(this, mPos, mFrameBuffer->getBitmap());
}

void Frame::update(Rectangular rect)
{
	Object *obj;

	mFrameBuffer->clearRectangular(rect);

	for (uint16_t i = 0; i < mNumOfObj; i++)
	{
		obj = mObjArr[i];

		if(obj->isModified())
		{
			obj->paint();
			obj->clearModifyFlag();
		}

		if (obj->isVisible())
			mFrameBuffer->drawBitmap(rect, obj->getPosition(), obj->getFrameBuffer()->getBitmap());
	}

	system::drawBitmap(this, rect, mPos, mFrameBuffer->getBitmap());
}

void Frame::update(Rectangular before , Rectangular current)
{
	Object *obj;

	mFrameBuffer->clearRectangular(before);
	mFrameBuffer->clearRectangular(current);

	for (uint16_t i = 0; i < mNumOfObj; i++)
	{
		obj = mObjArr[i];
		if(obj->isModified())
		{
			obj->paint();
			obj->clearModifyFlag();
		}

		if (obj->isVisible())
		{
			mFrameBuffer->drawBitmap(before, obj->getPosition(), obj->getFrameBuffer()->getBitmap());
			mFrameBuffer->drawBitmap(current, obj->getPosition(), obj->getFrameBuffer()->getBitmap());
		}
	}

	if (mOutputFrameBuffer)
	{
		before += mPos;
		current += mPos;
		mParent->update(before, current);
	}
}

void Frame::add(Object &obj)
{
	obj.paint();

	if (mNumOfObj + 1 >= mMaxObj)
		increaseObjArr();

	mObjArr[mNumOfObj] = &obj;
	mNumOfObj++;

	obj.setParent(this);

	update(obj.getRectangular());
}

void Frame::add(Object *obj)
{
	add(*obj);
}

Object *Frame::handlerPush(Position pos)
{
	return Container::handlerPush(pos);
}

Object *Frame::handlerDrag(Position pos)
{
	return Container::handlerDrag(pos);
}

Object *Frame::handlerUp(void)
{
	return Container::handlerUp();
}

void Frame::setAsSystemFrame(void)
{
	system::setSystemFrame(this);
}

void Frame::setPointerDevice(PointerDevice &obj)
{
	mPointerDevice = &obj;
}

#endif