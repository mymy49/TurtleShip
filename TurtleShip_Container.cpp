/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <yss/instance.h>
#include <config.h>
#include <yss/gui.h>
#include <std_ext/malloc.h>

Container::Container()
{
	mFrameBuffer = new FrameBufferRgb565LE();

	mObjArr = 0;
	mMaxObj = 0;
	mNumOfObj = 0;
	mLastEventObj = 0;
	mValidFlag = true;
	increaseObjArr();
}

Container::~Container()
{
	for(int32_t  i=0;i<mNumOfObj;i++)
	{
		delete mObjArr[i];
	}

	if (mObjArr)
		lfree(mObjArr);
	
	mValidFlag = false;
}

void Container::paint(void)
{
	uint16_t i;
	Object *obj;

	for (i = 0; i < mNumOfObj; i++)
	{
		obj = mObjArr[i];

		if (obj->isVisible())
		{
			mFrameBuffer->drawBitmap(obj->getPosition(), obj->getFrameBuffer()->getBitmap());
		}
	}
}

void Container::add(Object &obj)
{
	if(!mValidFlag)
	{
		// Container가 이미 제거된 상태에서 add를 시도할 경우 빠져나감
		return;
	}

	if (mNumOfObj + 1 >= mMaxObj)
		increaseObjArr();

	mObjArr[mNumOfObj] = &obj;
	mNumOfObj++;

	obj.setParent(this);

	update({obj.getPosition(), obj.getSize()});
}

void Container::add(Object *obj)
{
	add(*obj);
}

void Container::increaseObjArr(void)
{
	uint16_t i;
	Object **temp;

	temp = (Object **)lmalloc(sizeof(Object *) * (mMaxObj + 512));

	if (mMaxObj)
	{
		for (i = 0; i < mMaxObj; i++)
			temp[i] = mObjArr[i];

		lfree(mObjArr);
	}
	mMaxObj += 512;

	mObjArr = temp;
}

void Container::update(void)
{
	update({mPos, mFrameBuffer->getSize()});
}

void Container::update(Rectangular rect)
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

	if (mParent)
	{
		rect += mPos;
		mParent->update(rect);
	}
}

void Container::update(Rectangular before, Rectangular current)
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

	if (mParent)
	{
		before += mPos;
		current += mPos;
		mParent->update(before, current);
	}
}

Object *Container::handlerPush(Position pos)
{
	Position objPos;
	Size objSize;
	Position calculatedPos = pos;
	Object *rt;
	int16_t ox, oy, x = pos.getX(), y = pos.getY();

	for (int16_t i = mNumOfObj - 1; i >= 0; i--)
	{
		objPos = mObjArr[i]->getPosition();
		objSize = mObjArr[i]->getSize();

		ox = objPos.getX();
		oy = objPos.getY();

		if (mObjArr[i]->isVisible() && ox < x && oy < y && ox + objSize.getWidth() > x && oy + objSize.getHeight() > y)
		{
			calculatedPos = pos;
			calculatedPos -= objPos;

			rt = mObjArr[i]->handlerPush(calculatedPos);

			return rt;
		}
	}

	return nullptr;
}

Object *Container::handlerDrag(Position pos)
{
	Position objPos;
	Size objSize;
	Position calculatedPos;
	Object *rt;
	int16_t ox, oy, x = pos.getX(), y = pos.getY();

	for (int16_t i = mNumOfObj - 1; i >= 0; i--)
	{
		objPos = mObjArr[i]->getPosition();
		objSize = mObjArr[i]->getSize();

		ox = objPos.getX();
		oy = objPos.getY();

		if (mObjArr[i]->isVisible() && ox < x && oy < y && ox + objSize.getWidth() > x && oy + objSize.getHeight() > y)
		{
			calculatedPos = pos;
			calculatedPos -= objPos;

			rt = mObjArr[i]->handlerDrag(calculatedPos);
			return rt;
		}
	}

	return nullptr;
}

Object *Container::handlerUp(void)
{
	return nullptr;
}

void Container::setBackgroundColor(Color color)
{
	mFrameBuffer->setBackgroundColor(color);
}

#endif