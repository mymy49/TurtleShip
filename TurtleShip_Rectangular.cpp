/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "Rectangular.h"

Rectangular::Rectangular(void)
{
}

Rectangular::Rectangular(Position pos, Size size)
{
	mPos = pos;
	mSize = size;
}

Rectangular::Rectangular(int16_t x, int16_t y, uint16_t width, uint16_t height)
{
	mSize.setSize(width, height);
	mPos.setPosition(x, y);
}

void Rectangular::operator+=(Position obj)
{
	mPos += obj;
}

void Rectangular::operator-=(Position obj)
{
	mPos -= obj;
}

void Rectangular::operator=(Position obj)
{
	mPos = obj;
}

void Rectangular::operator+=(Size obj)
{
	mSize += obj;
}

void Rectangular::operator-=(Size obj)
{
	mSize -= obj;
}

void Rectangular::operator=(Size obj)
{
	mSize = obj;
}

void Rectangular::setRectanglar(Position pos, Size size)
{
	mSize = size;
}

void Rectangular::setRectanglar(int16_t x, int16_t y, uint16_t width, uint16_t height)
{
	mSize.setSize(width, height);
	mPos.setPosition(x, y);
}

Size &Rectangular::getSize(void)
{
	return mSize;
}

Position &Rectangular::getPosition(void)
{
	return mPos;
}

void Rectangular::addX(int16_t x)
{
	mPos.addX(x);
}

void Rectangular::subX(int16_t x)
{
	mPos.subX(x);
}

void Rectangular::addY(int16_t y)
{
	mPos.addY(y);
}

void Rectangular::subY(int16_t y)
{
	mPos.subY(y);
}

void Rectangular::setX(int16_t x)
{
	mPos.setX(x);
}

void Rectangular::setY(int16_t y)
{
	mPos.setY(y);
}

void Rectangular::addWidth(uint16_t width)
{
	mSize.addWidth(width);
}

void Rectangular::subWidth(uint16_t width)
{
	mSize.subWidth(width);
}

void Rectangular::addHeight(uint16_t height)
{
	mSize.addHeight(height);
}

void Rectangular::subHeight(uint16_t height)
{
	mSize.subHeight(height);
}

void Rectangular::setWidth(uint16_t width)
{
	mSize.setWidth(width);
}

void Rectangular::setHeight(uint16_t height)
{
	mSize.setHeight(height);
}

