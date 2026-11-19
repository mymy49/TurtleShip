/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Object.cpp
 * @brief Implementation of the Object base class for GUI widgets.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include <yss/gui.h>
#include <yss/Mutex.h>

/**
 * @details Initializes an unattached `Object` (`mParent = 0`) that is visible (`mVisibleFlag = true`),
 * resizable (`mResizeAble = true`), and clean (`mModifiedFlag = false`).
 * @see Object
 * @todo None.
 * @bug None.
 */
Object::Object(void)
{
	mParent = 0;
	mVisibleFlag = true;
	mResizeAble = true;
	mModifiedFlag = false;
}

/**
 * @details Virtual destructor for `Object`.
 * @see Object
 * @todo None.
 * @bug None.
 */
Object::~Object(void)
{

}

/**
 * @details Propagates a single-region dirty update (`rect`) to `mParent->update(rect)` if a parent
 * container is attached.
 * @see Container::update(Rectangular)
 * @todo Update parameter type from legacy `Rectangular` to `Area` to synchronize with Object.h in upcoming revision.
 * @bug Doxygen member matching warning occurs due to signature type difference (`Rectangular` vs `Area`) pending future update.
 */
void Object::update(Rectangular rect)
{
	if(mParent)
	{
		//pos.x += mPos.x;
		//pos.y += mPos.y;
		mParent->update(rect);
	}
}

/**
 * @details Propagates a dual-region dirty update (`before`, `current`) to `mParent->update(before, current)`
 * if a parent container is attached.
 * @see Container::update(Rectangular, Rectangular)
 * @todo Update parameter types from legacy `Rectangular` to `Area` in upcoming revision.
 * @bug Doxygen member matching warning occurs due to signature type difference (`Rectangular` vs `Area`) pending future update.
 */
void Object::update(Rectangular before , Rectangular current)
{
	if(mParent)
		mParent->update(before, current);
}

/**
 * @details Constructs and returns the bounding rectangle `{mPos, mFrameBuffer->getSize()}` of the widget.
 * @see Object::update()
 * @todo Update return type from legacy `Rectangular` to `Area` in upcoming revision.
 * @bug Legacy `Rectangular` type used pending future update.
 */
Rectangular Object::getRectangular(void)
{
	return Rectangular {mPos, mFrameBuffer->getSize()};
}

/**
 * @details Invalidates the widget's own bounding rectangle `{mPos, mFrameBuffer->getSize()}` on `mParent`.
 * @see Object::update(Rectangular)
 * @todo None.
 * @bug None.
 */
void Object::update(void)
{
	if(mParent)
		mParent->update({mPos, mFrameBuffer->getSize()});
}

/**
 * @details Unpacks `pos.getX()` and `pos.getY()` and delegates to `setPosition(int16_t, int16_t)`.
 * @see Object::setPosition(int16_t, int16_t)
 * @todo None.
 * @bug None.
 */
void Object::setPosition(Position pos)
{
	setPosition(pos.getX(), pos.getY());
}

/**
 * @details Saves the old position `before = mPos`, updates `mPos` to `{x, y}`, and calls
 * `update({before, size}, {mPos, size})` so both the old and new regions are redrawn by the parent.
 * @see Object::update(Rectangular, Rectangular)
 * @todo None.
 * @bug None.
 */
void Object::setPosition(int16_t x, int16_t y)
{
	Size size = mFrameBuffer->getSize();
	Position before = mPos;
	mPos = Position{x, y};
	update({before, size}, {mPos, size});
}

/**
 * @details Returns the widget's relative position `mPos`.
 * @see Object
 * @todo None.
 * @bug None.
 */
Position Object::getPosition(void)
{
	return mPos;
}

/**
 * @details If `mResizeAble` is `true`, reallocates `mFrameBuffer` for `width * height` pixels,
 * updates its logical `size`, triggers `eventSizeChanged(size)`, repaints the widget via `paint()`,
 * and notifies the parent container via `update()`.
 * @see Object::eventSizeChanged(), Object::paint()
 * @todo None.
 * @bug None.
 */
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

/**
 * @details Wraps `width` and `height` into a `Size` object and delegates to `setSize(Size)`.
 * @see Object::setSize(Size)
 * @todo None.
 * @bug None.
 */
void Object::setSize(uint16_t width, uint16_t height)
{
	setSize({width, height});
}

/**
 * @details Updates `mVisibleFlag` to `on` and triggers a parent `update()` for the widget's bounding area.
 * @see Object
 * @todo None.
 * @bug None.
 */
void Object::setVisible(bool on)
{
	mVisibleFlag = on;
	update({mPos, mFrameBuffer->getSize()});
}

/**
 * @details Returns the current visibility flag (`mVisibleFlag`).
 * @see Object
 * @todo None.
 * @bug None.
 */
bool Object::isVisible(void)
{
	return mVisibleFlag;
}

/**
 * @details Stores the pointer to the parent `Container` (`mParent = parent`).
 * @see Object
 * @todo None.
 * @bug None.
 */
void Object::setParent(Container *parent)
{
	mParent = parent;
}

/**
 * @details Default touch-down handler; ignores `pos` and returns `this`.
 * @see Object
 * @todo None.
 * @bug None.
 */
Object *Object::handlerPush(Position pos)
{
	(void)pos;
	return this;
}

/**
 * @details Default touch-drag handler; ignores `pos` and returns `this`.
 * @see Object
 * @todo None.
 * @bug None.
 */
Object *Object::handlerDrag(Position pos)
{
	(void)pos;
	return this;
}

/**
 * @details Default touch-up handler; returns `this`.
 * @see Object
 * @todo None.
 * @bug None.
 */
Object *Object::handlerUp(void)
{
	return this;
}

/**
 * @details Queries `mParent->getAbsolutePos()` if `mParent` is set; otherwise returns `mPos`.
 * @see Object
 * @todo None.
 * @bug None.
 */
Position Object::getAbsolutePos(void)
{
	Position pos = mPos;

	if (mParent)
		pos = mParent->getAbsolutePos();
	
	return pos;
}

/**
 * @details Returns a pointer to the widget's internal `FrameBuffer` (`mFrameBuffer`).
 * @see Object
 * @todo None.
 * @bug None.
 */
FrameBuffer* Object::getFrameBuffer(void)
{
	return mFrameBuffer;
}

/**
 * @details Default no-op hook called when the widget size changes.
 * @see Object
 * @todo None.
 * @bug None.
 */
void Object::eventSizeChanged(Size size)
{
	(void)size;
}

/**
 * @details Forwards `font` to `mFrameBuffer->setFont(font)`.
 * @see Object
 * @todo None.
 * @bug None.
 */
void Object::setFont(Font &font)
{
	mFrameBuffer->setFont(font);
}

/**
 * @details Sets `mModifiedFlag` to `true` to indicate that `paint()` must be called before the next composite.
 * @see Object
 * @todo None.
 * @bug None.
 */
void Object::touch(void)
{
	mModifiedFlag = true;
}

/**
 * @details Returns `mModifiedFlag`.
 * @see Object
 * @todo None.
 * @bug None.
 */
bool Object::isModified(void)
{
	return mModifiedFlag;
}

/**
 * @details Resets `mModifiedFlag` to `false`.
 * @see Object
 * @todo None.
 * @bug None.
 */
void Object::clearModifyFlag(void)
{
	mModifiedFlag = false;
}

#endif
