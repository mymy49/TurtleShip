/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Button.cpp
 * @brief Implementation of the Button widget class.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#include <yss/instance.h>
#include <config.h>
#include <string.h>

#include "Button.h"
#include "Font.h"

/**
 * @details Allocates `mFrameBuffer`, `mPushedFb`, and `mPulledFb`, initializes event handler pointers
 * and state flags to `nullptr` / `false`, sets the default font color to white (`0xFF, 0xFF, 0xFF`),
 * and sets the default button face background color to medium gray (`0x80, 0x80, 0x80`).
 * @see Button
 * @todo None.
 * @bug None.
 */
Button::Button(void)
{
	mFrameBuffer = new FrameBufferRgb565LE();

	mState = false;
	mButtonModifiedFlag = false;
	mUpHandler = nullptr;
	mPushHandler = nullptr;
	mText = nullptr;

	mPushedFb = new GuiFrameBuffer();
	mPulledFb = new GuiFrameBuffer();

	mFontColor.setColor({0xFF, 0xFF, 0xFF});
	mFrameBuffer->setBackgroundColor({0x80, 0x80, 0x80});
	mPushedFb->setBackgroundColor({0x80, 0x80, 0x80});
	mPulledFb->setBackgroundColor({0x80, 0x80, 0x80});
}

/**
 * @details Frees the heap-allocated `mPushedFb` and `mPulledFb` instances.
 * @see Button
 * @todo None.
 * @bug None.
 */
Button::~Button(void)
{
	if(mPushedFb != nullptr)
		delete mPushedFb;

	if(mPulledFb != nullptr)
		delete mPulledFb;
}

/**
 * @details Pre-renders the visual appearance of both the pressed (`mPushedFb`) and released
 * (`mPulledFb`) button states:
 * -# Clears `mPushedFb` and draws a 3-pixel sunken bevel (dark gray `{20,20,20}`, `{70,70,70}`,
 *    `{100,100,100}` on top/left; light gray `{160,160,160}`, `{192,192,192}`, `{224,224,224}` on bottom/right).
 * -# Clears `mPulledFb` and draws a 3-pixel raised bevel (light gray on top/left; dark gray on bottom/right).
 * -# If both `mText` and a `Font` are configured, centers `mText` on `mPulledFb` and draws it shifted
 *    by `(+2, +2)` pixels on `mPushedFb` to simulate physical button depression.
 * @see Button::paint()
 * @todo None.
 * @bug None.
 */
void Button::makeButtonImage(void)
{
	Size size = mFrameBuffer->getSize();

	if(size.getWidth() <= 0 || size.getHeight() <= 0)
		return;

	Font *font = mFrameBuffer->getFont();
	uint16_t width = mFrameBuffer->getSize().getWidth(), height = mFrameBuffer->getSize().getHeight();
	uint16_t fwidth, fheight;

	// Render sunken 3D bevel into mPushedFb (pressed state)
	mPushedFb->clear();
	mPushedFb->setBrushColor({20, 20, 20});
	mPushedFb->drawLine({0, 0}, {(int16_t)(width - 1), 0});
	mPushedFb->drawLine({0, 0}, {0, (int16_t)(height - 1)});
	mPushedFb->setBrushColor({70, 70, 70});
	mPushedFb->drawLine({1, 1}, {(int16_t)(width - 2), 1});
	mPushedFb->drawLine({1, 1}, {1, (int16_t)(height - 2)});
	mPushedFb->setBrushColor({100, 100, 100});
	mPushedFb->drawLine({2, 2}, {(int16_t)(width - 3), 2});
	mPushedFb->drawLine({2, 2}, {2, (int16_t)(height - 3)});

	mPushedFb->setBrushColor({160, 160, 160});
	mPushedFb->drawLine({(int16_t)(width - 3), (int16_t)(height - 3)}, {(int16_t)(width - 3), 2});
	mPushedFb->drawLine({2, (int16_t)(height - 3)}, {(int16_t)(width - 3), (int16_t)(height - 3)});

	mPushedFb->setBrushColor({192, 192, 192});
	mPushedFb->drawLine({(int16_t)(width - 2), (int16_t)(height - 2)}, {(int16_t)(width - 2), 1});
	mPushedFb->drawLine({1, (int16_t)(height - 2)}, {(int16_t)(width - 2), (int16_t)(height - 2)});

	mPushedFb->setBrushColor({224, 224, 224});
	mPushedFb->drawLine({(int16_t)(width - 1), (int16_t)(height - 1)}, {(int16_t)(width - 1), 0});
	mPushedFb->drawLine({0, (int16_t)(height - 1)}, {(int16_t)(width - 1), (int16_t)(height - 1)});

	// Render raised 3D bevel into mPulledFb (released state)
	mPulledFb->clear();
	mPulledFb->setBrushColor({224, 224, 224});
	mPulledFb->drawLine({0, 0}, {(int16_t)(width - 1), 0});
	mPulledFb->drawLine({0, 0}, {0, (int16_t)(height - 1)});
	mPulledFb->setBrushColor({192, 192, 192});
	mPulledFb->drawLine({1, 1}, {(int16_t)(width - 2), 1});
	mPulledFb->drawLine({1, 1}, {1, (int16_t)(height - 2)});
	mPulledFb->setBrushColor({160, 160, 160});
	mPulledFb->drawLine({2, 2}, {(int16_t)(width - 3), 2});
	mPulledFb->drawLine({2, 2}, {2, (int16_t)(height - 3)});

	mPulledFb->setBrushColor({100, 100, 100});
	mPulledFb->drawLine({(int16_t)(width - 3), (int16_t)(height - 3)}, {(int16_t)(width - 3), 2});
	mPulledFb->drawLine({2, (int16_t)(height - 3)}, {(int16_t)(width - 3), (int16_t)(height - 3)});

	mPulledFb->setBrushColor({70, 70, 70});
	mPulledFb->drawLine({(int16_t)(width - 2), (int16_t)(height - 2)}, {(int16_t)(width - 2), 1});
	mPulledFb->drawLine({1, (int16_t)(height - 2)}, {(int16_t)(width - 2), (int16_t)(height - 2)});

	mPulledFb->setBrushColor({20, 20, 20});
	mPulledFb->drawLine({(int16_t)(width - 1), (int16_t)(height - 1)}, {(int16_t)(width - 1), 0});
	mPulledFb->drawLine({0, (int16_t)(height - 1)}, {(int16_t)(width - 1), (int16_t)(height - 1)});

	// Draw centered label text on both states (with +2px offset when pushed)
	if (mText && font)
	{
		fwidth = font->getStringWidth((char*)mText);
		fheight = font->getStringHeight();
		mPushedFb->setBrushColor(mFontColor);
		mPulledFb->setBrushColor(mFontColor);
		mPushedFb->drawString({(int16_t)(width / 2 - fwidth / 2 + 2), (int16_t)(height / 2 - fheight / 2 + 2)}, (char *)mText);
		mPulledFb->drawString({(int16_t)(width / 2 - fwidth / 2), (int16_t)(height / 2 - fheight / 2)}, (char *)mText); 
	}
}

/**
 * @details Rebuilds the cached button bitmaps via `makeButtonImage()` if `mButtonModifiedFlag` is set,
 * then copies either `mPushedFb` (when `mState == true`) or `mPulledFb` (when `mState == false`)
 * onto `mFrameBuffer`.
 * @see Button::makeButtonImage()
 * @todo None.
 * @bug None.
 */
void Button::paint(void)
{
	if(mButtonModifiedFlag)
	{
		mButtonModifiedFlag = false;
		makeButtonImage();
	}

	if(mState)
		mFrameBuffer->drawBitmap({0, 0}, mPushedFb->getBitmap());
	else
		mFrameBuffer->drawBitmap({0, 0}, mPulledFb->getBitmap());
}

/**
 * @details Stores the push callback function pointer in `mPushHandler`.
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setPushEventHandler(void (*handler)(void))
{
	mPushHandler = handler;
}

/**
 * @details Stores the release callback function pointer in `mUpHandler`.
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setUpEventHandler(void (*handler)(void))
{
	mUpHandler = handler;
}

/**
 * @details Updates `mText`, sets `mButtonModifiedFlag = true`, and calls `touch()` so the button
 * image is regenerated on the next paint cycle.
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setText(const char *text)
{
	mText = text;
	mButtonModifiedFlag = true;
	touch();
}

/**
 * @details If `mResizeAble` is `true`, reallocates and resizes `mFrameBuffer`, `mPushedFb`, and
 * `mPulledFb` for `size.getWidth() * size.getHeight()` pixels, calls `eventSizeChanged(size)`,
 * and marks the button modified (`mButtonModifiedFlag = true`, `touch()`).
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setSize(Size size)
{
	uint32_t pixels = size.getHeight() * size.getWidth();

	if(mResizeAble)
	{
		mFrameBuffer->malloc(pixels);
		mFrameBuffer->setSize(size);
		mPushedFb->malloc(pixels);
		mPushedFb->setSize(size);
		mPulledFb->malloc(pixels);
		mPulledFb->setSize(size);
		eventSizeChanged(size);
		mButtonModifiedFlag = true;
		touch();
	}
}

/**
 * @details Wraps `width` and `height` into a `Size` and delegates to `setSize(Size)`.
 * @see Button::setSize(Size)
 * @todo None.
 * @bug None.
 */
void Button::setSize(uint16_t width, uint16_t height)
{
	setSize({width, height});
}

/**
 * @details Sets `mState = true`, repaints and updates the button on screen, and invokes
 * `mPushHandler()` if registered.
 * @see Button
 * @todo None.
 * @bug None.
 */
Object *Button::handlerPush(Position pos)
{
	(void)pos;
	
	mState = true;
	paint();
	update();
	if (mPushHandler)
		mPushHandler();
	return this;
}

/**
 * @details Sets `mState = false`, repaints and updates the button on screen, and invokes
 * `mUpHandler()` if registered.
 * @see Button
 * @todo None.
 * @bug None.
 */
Object *Button::handlerUp(void)
{
	mState = false;
	paint();
	update();
	if (mUpHandler)
		mUpHandler();
	return this;
}

/**
 * @details Attaches `font` to `mFrameBuffer`, `mPushedFb`, and `mPulledFb`.
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setFont(Font &font)
{
	mFrameBuffer->setFont(font);
	mPushedFb->setFont(font);
	mPulledFb->setFont(font);
}

/**
 * @details Sets the background color on `mFrameBuffer`, `mPushedFb`, and `mPulledFb`, marks
 * `mButtonModifiedFlag = true`, and calls `touch()`.
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setColor(Color color)
{
	mFrameBuffer->setBackgroundColor(color);
	mPushedFb->setBackgroundColor(color);
	mPulledFb->setBackgroundColor(color);
	mButtonModifiedFlag = true;
	touch();
}

/**
 * @details Updates `mFontColor`, marks `mButtonModifiedFlag = true`, and calls `touch()`.
 * @see Button
 * @todo None.
 * @bug None.
 */
void Button::setFontColor(Color color)
{
	mFontColor = color;
	mButtonModifiedFlag = true;
	touch();
}
