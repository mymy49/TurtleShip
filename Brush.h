/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_BRUSH__H_
#define YSS_GUI_BRUSH__H_

#include <stdint.h>
#include "Color.h"
#include "Position.h"
#include "Area.h"
#include "Size.h"
#include "TurtleShipTypes.h"

class Font;

class Brush
{
public:
	typedef enum
	{
		ALGIN_LEFT_TOP = (0x01 | 0x08),
		ALGIN_LEFT_MID = (0x01 | 0x10),
		ALGIN_LEFT_BOT = (0x01 | 0x20),

		ALIGN_CENTER_TOP = (0x02 | 0x08),
		ALIGN_CENTER_MID = (0x02 | 0x10),
		ALIGN_CENTER_BOT = (0x02 | 0x20),

		ALIGN_RIGHT_TOP = (0x04 | 0x08),
		ALIGN_RIGHT_MID = (0x04 | 0x10),
		ALIGN_RIGHT_BOT = (0x04 | 0x20)
	}align_t;

	Brush(void);

	virtual void drawDot(int16_t x, int16_t y) __attribute__((optimize("-O1"))) = 0;

	virtual void blendDot(int16_t x, int16_t y, uint8_t alpha) __attribute__((optimize("-O1"))) = 0;
	
	virtual void setBrushColor(Color color) __attribute__((optimize("-O1"))) = 0;

	virtual Color getBrushColor() __attribute__((optimize("-O1"))) = 0;

	virtual void setBackgroundColor(Color color) __attribute__((optimize("-O1"))) = 0;

	virtual Color getBackgroundColor() __attribute__((optimize("-O1"))) = 0;

	void drawLine(Position p1, Position p2) __attribute__((optimize("-O1")));

	void drawCircle(Position pos, uint16_t r) __attribute__((optimize("-O1")));

	void fillCircle(Position p1, uint16_t r) __attribute__((optimize("-O1")));

	void drawTriangle(Position p1, Position p2, Position p3) __attribute__((optimize("-O1")));

	void fillTriangle(Position p1, Position p2, Position p3) __attribute__((optimize("-O1")));

	void drawRectangular(Area rect) __attribute__((optimize("-O1")));

	void fillRectangular(Area rect) __attribute__((optimize("-O1")));

	void fillQuadrangle(Position p1, Position p2, Position p3, Position p4) __attribute__((optimize("-O1")));

	void clearRectangular(Area rect) __attribute__((optimize("-O1")));

	void fill() __attribute__((optimize("-O1")));

	void clear() __attribute__((optimize("-O1")));

	void setFont(Font &font) __attribute__((optimize("-O1")));

	Font* getFont() __attribute__((optimize("-O1")));

	uint8_t drawChar(Position pos, uint32_t utf8) __attribute__((optimize("-O1")));

	Position drawString(Position pos, const char *str) __attribute__((optimize("-O1")));

	Position drawString(align_t align, const char *str) __attribute__((optimize("-O1")));

	Size calculateStringArea(const char *str) __attribute__((optimize("-O1")));

	void drawBitmap(Position pos, const bitmap_t bitmap) __attribute__((optimize("-O1")));

	void drawBitmap(Area canvasRect, Position bitmapPos, const bitmap_t bitmap) __attribute__((optimize("-O1")));

	void drawBitmap(Size canvasSize, Area canvasRect, Position bitmapPos, const bitmap_t bitmap) __attribute__((optimize("-O1")));

	Area calculateValidArea() __attribute__((optimize("-O1")));

	bool isOutsideCanvas() __attribute__((optimize("-O1")));

	void setCalculatorSource(Area src) __attribute__((optimize("-O1")));

protected:
	Font *mFont;

	int32_t mSrcWidth, mSrcHeight, mSrcX, mSrcY, mSrcOffset, mCanvasWidth, mCanvasHeight;
	
	bool checkDrawingAble(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	uint32_t calculateSrcFrameBufferOffset(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	uint16_t calculateSrcWidth(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	uint16_t calculateSrcHeight(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	uint32_t calculateDesFrameBufferOffset(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	bool calculate2BytesPixelDrawingInfo(Area &des, Area &src, uint16_t **frameBuffer);

	virtual void fillRectBase(int16_t x, int16_t y, uint16_t width, uint16_t height, Color color) __attribute__((optimize("-O1")));

	virtual void drawBitmapBase(Size canvasSize, Area canvasDesArea, Position bitmapDrawingPos, const bitmap_t bitmap) __attribute__((optimize("-O1"))) = 0;

	virtual void drawBitmapBase(Position pos, const bitmap_t bitmap) __attribute__((optimize("-O1"))) = 0;

	virtual void fillDotArray(uint32_t offset, uint32_t count, Color color) __attribute__((optimize("-O1"))) = 0;

	virtual uint8_t getPixelCapacity() __attribute__((optimize("-O1"))) = 0;

	virtual Size getCanvasSize() __attribute__((optimize("-O1"))) = 0;
};

#endif

