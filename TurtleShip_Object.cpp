/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <yss/gui.h>
#include <yss/Mutex.h>

Object::Object(void)
{
	mParent = 0;
	mVisibleFlag = true;
	mResizeAble = true;
	mModifiedFlag = false;
}

Object::~Object(void)
{

}

void Object::update(Rectangular rect)
{
	if(mParent)
	{
		//pos.x += mPos.x;
		//pos.y += mPos.y;
		mParent->update(rect);
	}
}

void Object::update(Rectangular before , Rectangular current)
{
	if(mParent)
		mParent->update(before, current);
}

Rectangular Object::getRectangular(void)
{
	return Rectangular {mPos, mFrameBuffer->getSize()};
}

void Object::update(void)
{
	if(mParent)
		mParent->update({mPos, mFrameBuffer->getSize()});
}

void Object::setPosition(Position pos)
{
	setPosition(pos.getX(), pos.getY());
}

void Object::setPosition(int16_t x, int16_t y)
{
	Size size = mFrameBuffer->getSize();
	Position before = mPos;
	mPos = Position{x, y};
	update({before, size}, {mPos, size});
}

Position Object::getPosition(void)
{
	return mPos;
}

void Object::setSize(Size size)
{
	uint32_t pixels = size.getHeight() * size.getWidth();

	if(mResizeAble)
	{
		mFrameBuffer->malloc(pixels);
		mFrameBuffer->setSize(size);
		eventSizeChanged(size);
		paint();
		update({mPos, size}, {mPos, size});
	}
}

void Object::setSize(uint16_t width, uint16_t height)
{
	setSize({width, height});
}

void Object::setVisible(bool on)
{
	mVisibleFlag = on;
	update({mPos, mFrameBuffer->getSize()});
}

bool Object::isVisible(void)
{
	return mVisibleFlag;
}

void Object::setParent(Container *parent)
{
	mParent = parent;
}

Object *Object::handlerPush(Position pos)
{
	(void)pos;
	return this;
}

Object *Object::handlerDrag(Position pos)
{
	(void)pos;
	return this;
}

Object *Object::handlerUp(void)
{
	return this;
}

Position Object::getAbsolutePos(void)
{
	Position pos = mPos;

	if (mParent)
		pos = mParent->getAbsolutePos();
	
	return pos;
}

FrameBuffer* Object::getFrameBuffer(void)
{
	return mFrameBuffer;
}

void Object::eventSizeChanged(Size size)
{
	(void)size;
}

void Object::setFont(Font &font)
{
	mFrameBuffer->setFont(font);
}

void Object::touch(void)
{
	mModifiedFlag = true;
}

bool Object::isModified(void)
{
	return mModifiedFlag;
}

void Object::clearModifyFlag(void)
{
	mModifiedFlag = false;
}

#endif