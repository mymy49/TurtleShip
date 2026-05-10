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
#include "Rectangular.h"
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

	virtual void drawDot(int16_t x, int16_t y) = 0;

	virtual void blendDot(int16_t x, int16_t y, uint8_t alpha) = 0;
	
	virtual void setBrushColor(Color color) = 0;

	virtual Color getBrushColor(void) = 0;

	virtual void setBackgroundColor(Color color) = 0;

	virtual Color getBackgroundColor(void) = 0;

	void drawLine(Position p1, Position p2);

	void drawCircle(Position pos, uint16_t r);

	void fillCircle(Position p1, uint16_t r);

	void drawTriangle(Position p1, Position p2, Position p3);

	void fillTriangle(Position p1, Position p2, Position p3);

	void drawRectangular(Rectangular rect);

	void fillRectangular(Rectangular rect);

	void fillQuadrangle(Position p1, Position p2, Position p3, Position p4);

	void clearRectangular(Rectangular rect);

	void fill(void);

	void clear(void);

	void setFont(Font &font);

	Font* getFont(void);

	uint8_t drawChar(Position pos, uint32_t utf8);

	Position drawString(Position pos, const char *str);

	Position drawString(align_t align, const char *str);

	Size calculateStringArea(const char *str);

	void drawBitmap(Position bitmapPos, const bitmap_t bitmap);

	void drawBitmap(Rectangular canvasRect, Position bitmapPos, const bitmap_t bitmap);

	void drawBitmap(Size canvasSize, Rectangular canvasRect, Position bitmapPos, const bitmap_t bitmap);

protected:
	Font *mFont;
	
	bool checkDrawingAble(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea);

	uint32_t calculateSrcFrameBufferOffset(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea);

	uint16_t calculateSrcWidth(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea);

	uint16_t calculateSrcHeight(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea);

	uint32_t calculateDesFrameBufferOffset(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea);

	bool calculate2BytesPixelDrawingInfo(Rectangular &des, Rectangular &src, uint16_t **frameBuffer);

	virtual void fillRectBase(int16_t x, int16_t y, uint16_t width, uint16_t height, Color color);

	virtual void drawBitmapBase(Size canvasSize, Rectangular canvasDesArea, Position bitmapDrawingPos, const bitmap_t bitmap) = 0;

	virtual void fillDotArray(uint32_t offset, uint32_t count, Color color) = 0;

	virtual uint8_t getPixelCapacity(void) = 0;

	virtual Size getCanvasSize(void) = 0;
};

#endif

