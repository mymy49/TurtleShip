/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_SAC_TFT_LCD_DRIVER__H_
#define YSS_SAC_TFT_LCD_DRIVER__H_

#include <yss/error.h>
#include <stdint.h>
#include <yss/Mutex.h>
#include "TurtleShipTypes.h"

class Rectangular;
class Position;
class Size;

// TFT LCD 드라이버가 통신 채널(I2C, SPI 등)을 통해 설정이 가능한 경우
// 이 class를 상속 받고 아래 기능들을 반드시 구현 해야한다.
class TftLcdDriver : public Mutex
{
public:
	virtual Size getLcdSize(void) = 0;

	virtual void drawBitmapBase(Size canvasSize, Rectangular canvasRect, Position bitmapPos, const bitmap_t bitmap) = 0;
};

#endif