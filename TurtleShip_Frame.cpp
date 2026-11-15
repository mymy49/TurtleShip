/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Frame.cpp
 * @brief Implementation of the Frame class for top-level full-screen GUI windows.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <gui/Frame.h>
#include <yss/instance.h>

#include <yss.h>
#include <std_ext/malloc.h>
#include <sac/PointerDevice.h>
//#include <gui/OutputFrameBuffer.h>

/**
 * @details Queries the system TFT LCD resolution via `system::getSystemTftLcdSize()`, allocates and
 * sizes `mFrameBuffer` to match the full screen, locks resizing (`mResizeAble = false`), clears the
 * frame buffer, and initializes `mOutputFrameBuffer` and `mPointerDevice` to `nullptr`.
 */
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

/**
 * @details Detaches this `Frame` from `mPointerDevice` via `mPointerDevice->resetFrame(this)` if bound.
 */
Frame::~Frame(void)
{
	if(mPointerDevice)
		mPointerDevice->resetFrame(this);
	//clearActiveFrame();
}

/**
 * @details Intentionally ignores `pos` to prevent changing the position of a top-level `Frame`.
 */
void Frame::setPosition(Position pos)
{
	// Intentionally empty to prevent repositioning a Frame
	(void)pos;
}

/**
 * @details Intentionally ignores `x` and `y` to prevent changing the position of a top-level `Frame`.
 */
void Frame::setPosition(int16_t x, int16_t y)
{
	// Intentionally empty to prevent repositioning a Frame
	(void)x;
	(void)y;
}

/**
 * @details Stores `outputFrameBuffer` in `mOutputFrameBuffer` and triggers a full-frame `update()`.
 * @see Frame::update(void)
 */
void Frame::setOutputFrameBuffer(OutputFrameBuffer *outputFrameBuffer)
{
	mOutputFrameBuffer = outputFrameBuffer;

	update();
}

/**
 * @details Clears the entire frame buffer, repaints any modified child widgets, composites all
 * visible children onto `mFrameBuffer`, and transfers the full bitmap to the display via
 * `system::drawBitmap(this, mPos, mFrameBuffer->getBitmap())`.
 */
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

/**
 * @details Clears `rect` in `mFrameBuffer`, repaints any modified child widgets, composites all
 * visible children clipped to `rect`, and transfers the updated region to the display via
 * `system::drawBitmap(this, rect, mPos, mFrameBuffer->getBitmap())`.
 * @see Container::update(Rectangular)
 * @todo Update parameter type from legacy Rectangular to Area in future revision.
 * @bug Uses legacy Rectangular type pending future update.
 */
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

/**
 * @details Clears both `before` and `current` regions in `mFrameBuffer`, repaints modified children,
 * composites visible children clipped to `before` and `current`, and forwards the update if
 * `mOutputFrameBuffer` is set.
 * @see Container::update(Rectangular, Rectangular)
 * @todo Update parameter types from legacy Rectangular to Area in future revision.
 * @bug Uses legacy Rectangular type pending future update.
 */
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

/**
 * @details Calls `obj.paint()`, appends `obj` to `mObjArr` (expanding via `increaseObjArr()` if needed),
 * sets `obj.setParent(this)`, and updates the child's bounding rectangle (`update(obj.getRectangular())`).
 */
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

/**
 * @details Dereferences `obj` and delegates to `add(Object &)`.
 * @see Frame::add(Object &)
 */
void Frame::add(Object *obj)
{
	add(*obj);
}

/**
 * @details Delegates push event handling to `Container::handlerPush(pos)`.
 * @see Container::handlerPush()
 */
Object *Frame::handlerPush(Position pos)
{
	return Container::handlerPush(pos);
}

/**
 * @details Delegates drag event handling to `Container::handlerDrag(pos)`.
 * @see Container::handlerDrag()
 */
Object *Frame::handlerDrag(Position pos)
{
	return Container::handlerDrag(pos);
}

/**
 * @details Delegates touch-up event handling to `Container::handlerUp()`.
 * @see Container::handlerUp()
 */
Object *Frame::handlerUp(void)
{
	return Container::handlerUp();
}

/**
 * @details Registers this `Frame` as the active system frame via `system::setSystemFrame(this)`.
 */
void Frame::setAsSystemFrame(void)
{
	system::setSystemFrame(this);
}

/**
 * @details Stores the address of `obj` in `mPointerDevice`.
 */
void Frame::setPointerDevice(PointerDevice &obj)
{
	mPointerDevice = &obj;
}

#endif
