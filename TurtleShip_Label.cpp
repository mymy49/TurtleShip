/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Label.cpp
 * @brief Implementation of the Label widget class.
 */

#include <config.h>
#include <yss/instance.h>

#include "Label.h"

/**
 * @details Allocates `mFrameBuffer`, initializes `mText` to `nullptr`, sets the default text brush
 * color to black (`{0x00, 0x00, 0x00}`), and sets the default background color to white (`{0xFF, 0xFF, 0xFF}`).
 */
Label::Label(void)
{
	mFrameBuffer = new FrameBufferRgb565LE();

	mText = nullptr;
	mFrameBuffer->setBrushColor({0x00, 0x00, 0x00});
	mFrameBuffer->setBackgroundColor({0xFF, 0xFF, 0xFF});
}

/**
 * @details Clears `mFrameBuffer` with the background color and, if both `mText` and a `Font` are set,
 * renders `mText` centered (`Brush::ALIGN_CENTER_MID`) inside `mFrameBuffer`.
 * @see Brush::drawString(align_t, const char*)
 */
void Label::paint(void)
{
	if (mFrameBuffer == 0)
		return;

	Font *font = mFrameBuffer->getFont();

	mFrameBuffer->clear();
	if (mText && font != 0)
	{
		mFrameBuffer->drawString(Brush::ALIGN_CENTER_MID, (char *)mText);
	}
}

/**
 * @details Stores the string pointer `text` in `mText` and marks the widget dirty via `touch()`.
 */
void Label::setText(const char *text)
{
	mText = text;
	touch();
}

/**
 * @details Updates the brush color of `mFrameBuffer` to `color` and marks the widget dirty via `touch()`.
 */
void Label::setFontColor(Color color)
{
	mFrameBuffer->setBrushColor(color);
	touch();
}

/**
 * @details Updates the brush color of `mFrameBuffer` to `{red, green, blue}` and marks the widget dirty via `touch()`.
 */
void Label::setFontColor(uint8_t red, uint8_t green, uint8_t blue)
{
	mFrameBuffer->setBrushColor({red, green, blue});
	touch();
}

/**
 * @details Updates the background color of `mFrameBuffer` to `color` and marks the widget dirty via `touch()`.
 */
void Label::setBackgroundColor(Color color)
{
	mFrameBuffer->setBackgroundColor(color);
	touch();
}

/**
 * @details Updates the background color of `mFrameBuffer` to `{red, green, blue}` and marks the widget dirty via `touch()`.
 */
void Label::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	mFrameBuffer->setBackgroundColor({red, green, blue});
	touch();
}
