/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_BrushTftLcdRgb565.cpp
 * @brief Implementation of the BrushTftLcdRgb565 base class methods.
 */

#include "BrushTftLcdRgb565.h"
#include <yss/Size.h>
#include <config.h>
#include <yss/hmalloc.h>
#include <std_ext/string.h>

/**
 * @details Default constructor for `BrushTftLcdRgb565`.
 */
BrushTftLcdRgb565::BrushTftLcdRgb565(void)
{

}

/**
 * @details Virtual destructor for `BrushTftLcdRgb565`.
 */
BrushTftLcdRgb565::~BrushTftLcdRgb565(void)
{

}

/**
 * @details No-op default implementation of `fillDotArray()` because a direct TFT LCD brush does not
 * own a full-screen RAM frame buffer; hardware LCD subclasses override `fillRectBase()` to push pixel
 * runs directly to the display controller window.
 */
void BrushTftLcdRgb565::fillDotArray(uint32_t offset, uint32_t count, Color color)
{
	//memsethw(&mFrameBuffer[offset], color.getCode(), count * 2);
}

/**
 * @details Returns a zero-sized `Size{0, 0}` by default; concrete LCD panel subclasses override this
 * to report their actual resolution (e.g. `240 x 320`).
 */
Size BrushTftLcdRgb565::getCanvasSize(void)
{
	return {0, 0};
}

/**
 * @details Returns `2` since each RGB565 pixel occupies 2 bytes (16 bits).
 */
uint8_t BrushTftLcdRgb565::getPixelCapacity(void)
{
	return 2;
}
