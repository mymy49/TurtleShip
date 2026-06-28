/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "Area.h"

Area::Area(void)
{
}

Area::Area(Position pos, Size size)
{
	mPos = pos;
	mSize = size;
}

Area::Area(int16_t x, int16_t y, uint16_t width, uint16_t height)
{
	mSize.setSize(width, height);
	mPos.setPosition(x, y);
}

void Area::operator+=(Position obj)
{
	mPos += obj;
}

void Area::operator-=(Position obj)
{
	mPos -= obj;
}

void Area::operator=(Position obj)
{
	mPos = obj;
}

void Area::operator+=(Size obj)
{
	mSize += obj;
}

void Area::operator-=(Size obj)
{
	mSize -= obj;
}

void Area::operator=(Size obj)
{
	mSize = obj;
}

void Area::setRectanglar(Position pos, Size size)
{
	mSize = size;
}

void Area::setRectanglar(int16_t x, int16_t y, uint16_t width, uint16_t height)
{
	mSize.setSize(width, height);
	mPos.setPosition(x, y);
}

Size &Area::getSize(void)
{
	return mSize;
}

Position &Area::getPosition(void)
{
	return mPos;
}

void Area::addX(int16_t x)
{
	mPos.addX(x);
}

void Area::subX(int16_t x)
{
	mPos.subX(x);
}

void Area::addY(int16_t y)
{
	mPos.addY(y);
}

void Area::subY(int16_t y)
{
	mPos.subY(y);
}

void Area::setX(int16_t x)
{
	mPos.setX(x);
}

void Area::setY(int16_t y)
{
	mPos.setY(y);
}

void Area::addWidth(uint16_t width)
{
	mSize.addWidth(width);
}

void Area::subWidth(uint16_t width)
{
	mSize.subWidth(width);
}

void Area::addHeight(uint16_t height)
{
	mSize.addHeight(height);
}

void Area::subHeight(uint16_t height)
{
	mSize.subHeight(height);
}

void Area::setWidth(uint16_t width)
{
	mSize.setWidth(width);
}

void Area::setHeight(uint16_t height)
{
	mSize.setHeight(height);
}

int16_t Area::getRightBound()
{
	return mPos.getX() + mSize.getWidth();
}

int16_t Area::getBotBound()
{
	return mPos.getY() + mSize.getHeight();
}

