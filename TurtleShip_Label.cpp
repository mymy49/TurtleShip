/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>
#include <yss/instance.h>

#include "Label.h"

Label::Label(void)
{
	mFrameBuffer = new FrameBufferRgb565LE();

	mText = nullptr;
	mFrameBuffer->setBrushColor({0x00, 0x00, 0x00});
	mFrameBuffer->setBackgroundColor({0xFF, 0xFF, 0xFF});
}

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

void Label::setText(const char *text)
{
	mText = text;
	touch();
}

void Label::setFontColor(Color color)
{
	mFrameBuffer->setBrushColor(color);
	touch();
}

void Label::setFontColor(uint8_t red, uint8_t green, uint8_t blue)
{
	mFrameBuffer->setBrushColor({red, green, blue});
	touch();
}

void Label::setBackgroundColor(Color color)
{
	mFrameBuffer->setBackgroundColor(color);
	touch();
}

void Label::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue)
{
	mFrameBuffer->setBackgroundColor({red, green, blue});
	touch();
}

