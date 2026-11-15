/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Container.cpp
 * @brief Implementation of the Container class for managing composite child widgets.
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <yss/instance.h>
#include <config.h>
#include <yss/gui.h>
#include <std_ext/malloc.h>

/**
 * @details Allocates a new `FrameBufferRgb565LE` for the container, initializes child tracking counters
 * (`mObjArr = 0`, `mMaxObj = 0`, `mNumOfObj = 0`, `mLastEventObj = 0`, `mValidFlag = true`), and calls
 * `increaseObjArr()` to allocate the initial child pointer array.
 * @see Container::increaseObjArr()
 */
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

/**
 * @details Iterates through all `mNumOfObj` registered children and `delete`s each child object,
 * frees the `mObjArr` pointer array from the L-heap (`lfree()`), and marks `mValidFlag = false`.
 */
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

/**
 * @details Loops through all `mNumOfObj` child objects in registration order and blits each visible
 * child's bitmap (`obj->getFrameBuffer()->getBitmap()`) at `obj->getPosition()` onto `mFrameBuffer`.
 */
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

/**
 * @details Appends `obj` to `mObjArr` (growing the array via `increaseObjArr()` if full), sets `this`
 * as the parent of `obj` (`obj.setParent(this)`), and triggers a partial `update()` for the child's area.
 * Returns immediately if `mValidFlag` is `false`.
 * @see Container::increaseObjArr(), Container::update(Rectangular)
 */
void Container::add(Object &obj)
{
	if(!mValidFlag)
	{
		// Return early if add() is called after the Container has already been destroyed
		return;
	}

	if (mNumOfObj + 1 >= mMaxObj)
		increaseObjArr();

	mObjArr[mNumOfObj] = &obj;
	mNumOfObj++;

	obj.setParent(this);

	update({obj.getPosition(), obj.getSize()});
}

/**
 * @details Dereferences `obj` and forwards to `add(Object &)`.
 * @see Container::add(Object &)
 */
void Container::add(Object *obj)
{
	add(*obj);
}

/**
 * @details Allocates a new `Object*` array with `mMaxObj + 512` entries from the L-heap (`lmalloc()`),
 * copies existing child pointers from `mObjArr`, frees the old array via `lfree()`, and updates
 * `mMaxObj` and `mObjArr`.
 */
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

/**
 * @details Repaints the entire container rectangle `{mPos, mFrameBuffer->getSize()}` by delegating
 * to `update(Rectangular)`.
 * @see Container::update(Rectangular)
 */
void Container::update(void)
{
	update({mPos, mFrameBuffer->getSize()});
}

/**
 * @details Clears `rect` in `mFrameBuffer` with the background color, repaints any child whose
 * `isModified()` flag is set, blits all visible children clipped to `rect`, and—if `mParent` is set—
 * translates `rect` by `mPos` and propagates `mParent->update(rect)`.
 * @see Object::update(Area)
 * @todo Update parameter type from legacy Rectangular to Area in future revision.
 * @bug Uses legacy Rectangular type pending future update.
 */
void Container::update(Rectangular rect)
{
	Object *obj;

	// Clear the dirty region with the container's background color
	mFrameBuffer->clearRectangular(rect);

	// Repaint dirty children and composite visible children clipped to rect
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

	// Propagate dirty region to parent container in parent-local coordinates
	if (mParent)
	{
		rect += mPos;
		mParent->update(rect);
	}
}

/**
 * @details Clears both `before` and `current` rectangles in `mFrameBuffer`, repaints any modified
 * children, blits all visible children clipped to `before` and `current`, and propagates the
 * translated rectangles to `mParent->update(before, current)`.
 * @see Object::update(Area, Area)
 * @todo Update parameter types from legacy Rectangular to Area in future revision.
 * @bug Uses legacy Rectangular type pending future update.
 */
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

/**
 * @details Scans `mObjArr` in reverse order (`mNumOfObj - 1` down to `0`) so top-most children are
 * tested first. When a visible child whose bounding box contains `pos` is found, subtracts `objPos`
 * from `pos` and invokes `mObjArr[i]->handlerPush(calculatedPos)`. Returns `nullptr` if no child is hit.
 */
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

/**
 * @details Scans `mObjArr` in reverse order (`mNumOfObj - 1` down to `0`) and forwards the drag
 * event (in child-local coordinates) to the first visible child whose bounding box contains `pos`.
 * Returns `nullptr` if no child is hit.
 */
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

/**
 * @details Returns `nullptr` on touch-up at the container level.
 */
Object *Container::handlerUp(void)
{
	return nullptr;
}

/**
 * @details Sets the background color on the container's `mFrameBuffer`.
 */
void Container::setBackgroundColor(Color color)
{
	mFrameBuffer->setBackgroundColor(color);
}

#endif
