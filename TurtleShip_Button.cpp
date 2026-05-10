/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <yss/instance.h>
#include <config.h>
#include <string.h>

#include "Button.h"
#include "Font.h"

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

Button::~Button(void)
{
	if(mPushedFb != nullptr)
		delete mPushedFb;

	if(mPulledFb != nullptr)
		delete mPulledFb;
}

void Button::makeButtonImage(void)
{
	Size size = mFrameBuffer->getSize();

	if(size.getWidth() <= 0 || size.getHeight() <= 0)
		return;

	Font *font = mFrameBuffer->getFont();
	uint16_t width = mFrameBuffer->getSize().getWidth(), height = mFrameBuffer->getSize().getHeight();
	uint16_t fwidth, fheight;

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

void Button::setPushEventHandler(void (*handler)(void))
{
	mPushHandler = handler;
}

void Button::setUpEventHandler(void (*handler)(void))
{
	mUpHandler = handler;
}

void Button::setText(const char *text)
{
	mText = text;
	mButtonModifiedFlag = true;
	touch();
}

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

void Button::setSize(uint16_t width, uint16_t height)
{
	setSize({width, height});
}

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

Object *Button::handlerUp(void)
{
	mState = false;
	paint();
	update();
	if (mUpHandler)
		mUpHandler();
	return this;
}

void Button::setFont(Font &font)
{
	mFrameBuffer->setFont(font);
	mPushedFb->setFont(font);
	mPulledFb->setFont(font);
}

void Button::setColor(Color color)
{
	mFrameBuffer->setBackgroundColor(color);
	mPushedFb->setBackgroundColor(color);
	mPulledFb->setBackgroundColor(color);
	mButtonModifiedFlag = true;
	touch();
}

void Button::setFontColor(Color color)
{
	mFontColor = color;
	mButtonModifiedFlag = true;
	touch();
}

