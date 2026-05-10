/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "BrushTftLcdRgb565.h"
#include "Size.h"
#include <config.h>
#include <std_ext/malloc.h>
#include <std_ext/string.h>

BrushTftLcdRgb565::BrushTftLcdRgb565(void)
{

}

BrushTftLcdRgb565::~BrushTftLcdRgb565(void)
{

}

void BrushTftLcdRgb565::fillDotArray(uint32_t offset, uint32_t count, Color color)
{
	//memsethw(&mFrameBuffer[offset], color.getCode(), count * 2);
}

Size BrushTftLcdRgb565::getCanvasSize(void)
{
	return {0, 0};
}

uint8_t BrushTftLcdRgb565::getPixelCapacity(void)
{
	return 2;
}

