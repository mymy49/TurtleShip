/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include "Brush.h"
#include <math.h>
#include "Font.h"

#define DEFAULT_SPACE_WIDTH		4

Brush::Brush(void)
{
	mFont = nullptr;
}

void Brush::fillRectBase(int16_t x, int16_t y, uint16_t width, uint16_t height, Color color)
{
	uint16_t cwidth = getCanvasSize().getWidth(), cheight = getCanvasSize().getHeight();
	int16_t ex = x + width, ey = y + height, offset = getPixelCapacity();
	uint32_t des;

	if(ey >= cheight - 1)
		ey = cheight - 1;
	if(ex >= cwidth - 1)
		ex = cwidth - 1;

	for (int16_t i = y; i <= ey; i++)
	{
		des = i * cwidth * offset + x * offset;
		fillDotArray(des, width, color);
	}
}

void Brush::fillRectangular(Rectangular rect)
{
	Position pos = rect.getPosition();
	Size size = rect.getSize();

	fillRectBase(pos.getX(), pos.getY(), size.getWidth(), size.getHeight(), getBrushColor());
}

void Brush::clearRectangular(Rectangular rect)
{
	Position pos = rect.getPosition();
	Size size = rect.getSize();

	fillRectBase(pos.getX(), pos.getY(), size.getWidth(), size.getHeight(), getBackgroundColor());
}

void Brush::fill(void)
{
	Size size = getCanvasSize();

	fillRectBase(0, 0, size.getWidth(), size.getHeight(), getBrushColor());
}

void Brush::clear(void)
{
	Size size = getCanvasSize();

	fillRectBase(0, 0, size.getWidth(), size.getHeight(), getBackgroundColor());
}

void Brush::fillCircle(Position pos, uint16_t radius)
{
	float r = radius, buf;
	int32_t sx, ex, count, y, px = pos.getX(), py = pos.getY();
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	int16_t offset = getPixelCapacity();
	Color color = getBrushColor();
	uint32_t des;

	if (radius < 3)
		return;

	for (uint16_t i = 0; i < radius; i++)
	{
		buf = i + 1;
		buf = r * r - buf * buf;
		ex = sx = pow(buf, (float)0.5) + 0.5f;

		sx = px - sx + 1;
		ex = px + ex;

		if(sx < 0)
			sx = 0;
		if(ex > width - 1)
			ex = width - 1;
		
		y = py + i + 1;
		if(y < height)
		{
			des = y * width * offset + sx * offset;
			count = ex - sx;
			if(count > 0)
				fillDotArray(des, ex - sx, color);
		}
		
		y = py - i;
		if(y >= 0)
		{
			des = y * width * offset + sx * offset;
			count = ex - sx;
			if(count > 0)
				fillDotArray(des, ex - sx, color);
		}
	}
}

void Brush::drawCircle(Position pos, uint16_t radius)
{
	int16_t px = pos.getX(), py = pos.getY();
	float r = radius, x, yp, yn;
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	int16_t offset = getPixelCapacity();
	uint32_t color = getBrushColor().getCode();
	uint32_t des;

	if (radius < 3)
		return;

	for (uint16_t i = 0; i < radius; i++)
	{
		x = i;
		yp = r * r - x * x;
		yp = pow(yp, (float)0.5) + (float)0.5;
		yn = yp - (float)1.0;
		
		drawDot(px + x, py + yp);
		drawDot(px + x, py - yn);
		drawDot(px - x, py - yn);
		drawDot(px - x, py + yp);
		drawDot(px + yp, py + x);
		drawDot(px + yp, py - x);
		drawDot(px - yn, py - x);
		drawDot(px - yn, py + x);
	}
}

void Brush::drawLine(Position p1, Position p2)
{
	uint16_t p1x = p1.getX(), p1y = p1.getY(), p2x = p2.getX(), p2y = p2.getY();
	uint16_t lenX, lenY, x, y;
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	float slope;

	if(p1x == p2x && p1y == p2y)
	{
		drawDot(p1x, p2y);
		return;
	}

	if (p1x <= p2x && p1y <= p2y)
	{
		lenX = p2x - p1x;
		lenY = p2y - p1y;

		if (lenX > lenY)
		{
			slope = (float)lenY / (float)lenX;
			for (uint16_t i = 0; i <= lenX; i++)
			{
				x = p1x + i;
				y = p1y + slope * (float)i;
				drawDot(x, y);
			}
		}
		else
		{
			slope = (float)lenX / (float)lenY;
			for (uint16_t i = 0; i <= lenY; i++)
			{
				x = p1x + slope * (float)i;
				y = p1y + i;
				drawDot(x, y);
			}
		}
	}
	else if (p1x >= p2x && p1y <= p2y)
	{
		lenX = p1x - p2x;
		lenY = p2y - p1y;

		if (lenX > lenY)
		{
			slope = (float)lenY / (float)lenX;
			for (uint16_t i = 0; i <= lenX; i++)
			{
				x = p1x - i;
				y = p1y + slope * (float)i;
				drawDot(x, y);
			}
		}
		else
		{
			slope = (float)lenX / (float)lenY * (float)-1;
			for (uint16_t i = 0; i <= lenY; i++)
			{
				x = p1x + slope * (float)i;
				y = p1y + i;
				drawDot(x, y);
			}
		}
	}
	else if (p1x <= p2x && p1y >= p2y)
	{
		lenX = p2x - p1x;
		lenY = p1y - p2y;

		if (lenX > lenY)
		{
			slope = (float)lenY / (float)lenX * (float)-1;
			for (uint16_t i = 0; i <= lenX; i++)
			{
				x = p1x + i;
				y = p1y + slope * (float)i;
				drawDot(x, y);
			}
		}
		else
		{
			slope = (float)lenX / (float)lenY;
			for (uint16_t i = 0; i <= lenY; i++)
			{
				x = p1x + slope * (float)i;
				y = p1y - i;
				drawDot(x, y);
			}
		}
	}
	else
	{
		p1x = p2.getX();
		p2x = p1.getX();
		p1y = p2.getY();
		p2y = p1.getY();

		lenX = p2x - p1x;
		lenY = p2y - p1y;

		if (lenX > lenY)
		{
			slope = (float)lenY / (float)lenX;
			for (uint16_t i = 0; i <= lenX; i++)
			{
				x = p1x + i;
				y = p1y + slope * (float)i;
				drawDot(x, y);
			}
		}
		else
		{
			slope = (float)lenX / (float)lenY;
			for (uint16_t i = 0; i <= lenY; i++)
			{
				x = p1x + slope * (float)i;
				y = p1y + i;
				drawDot(x, y);
			}
		}
	}
}

void Brush::drawTriangle(Position p1, Position p2, Position p3)
{
	drawLine(p1, p2);
	drawLine(p2, p3);
	drawLine(p3, p1);
}

void Brush::drawRectangular(Rectangular rect)
{
	Position p1, p2;
	Size size = rect.getSize();
	
	p1 = rect.getPosition();
	p2 = p1;
	p2.addX(size.getWidth());
	drawLine(p1, p2);
	
	p1 = p2;
	p1.addY(size.getHeight());
	drawLine(p1, p2);
	
	p2 = p1;
	p2.subX(size.getWidth());	
	drawLine(p1, p2);

	p1 = p2;
	p1.subY(size.getHeight());
	drawLine(p1, p2);
}

void Brush::fillTriangle(Position p1, Position p2, Position p3)
{
	int16_t p1x, p2x, p3x;
	int16_t p1y, p2y, p3y;
	float slopeL, slopeR;
	int16_t sx, ex, sy, ey, buf;
	Position p;
	int16_t count;
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	uint8_t offset = getPixelCapacity();

	if(p1.getY() > p2.getY())
	{
		p = p1;
		p1 = p2;
		p2 = p;
	}
	
	if(p1.getY() > p3.getY())
	{
		p = p1;
		p1 = p3;
		p3 = p;
	}
	
	if(p2.getX() > p3.getX())
	{
		p = p2;
		p2 = p3;
		p3 = p;
	}
	
	p1x = p1.getX();
	p2x = p2.getX();
	p3x = p3.getX();
	p1y = p1.getY();
	p2y = p2.getY();
	p3y = p3.getY();
	
	if(p1y != p2y && p1y != p3y)
	{
		if(p2y < p3y)
			ey = p2y;
		else
			ey = p3y;
		
		if(ey > height)
			ey = height;
		
		sy = p1y;
		if(sy < 0)
			sy = 0;
		
		slopeL = (float)(p1x - p2x) / (float)(p1y - p2y);
		slopeR = (float)(p1x - p3x) / (float)(p1y - p3y);
		
		for(int32_t y = sy; y <= ey; y++)
		{
			sx = p1x + ((y - sy) * slopeL + 0.5f);
			ex = p1x + ((y - sy) * slopeR + 0.5f);

			if(sx > ex)
			{
				buf = sx;
				sx = ex;
				ex = buf;
			}

			if(sx < 0)
				sx = 0;
		
			if(ex > width - 1)
				ex = width - 1;
			
			count = ex - sx + 1;
			if(count < 0)
				count = 0;

			fillDotArray(y * width * offset + sx * offset, count, getBrushColor());
		}
	}

	if(p2y != p3y)
	{
		if(p2.getY() > p3.getY())
		{
			p = p3;
			p3 = p2;
			p2 = p;
		}
	
		if(p1.getX() > p2.getX())
		{
			p = p1;
			p1 = p2;
			p2 = p;
		}
	
		p1x = p1.getX();
		p2x = p2.getX();
		p3x = p3.getX();
		p1y = p1.getY();
		p2y = p2.getY();
		p3y = p3.getY();

		ey = p3y;

		if(p1y > p2y)
			sy = p1y;
		else
			sy = p2y;

		if(ey > height)
			ey = height;

		if(sy < 0)
			sy = 0;
		
		slopeL = (float)(p3x - p1x) / (float)(p3y - p1y);
		slopeR = (float)(p3x - p2x) / (float)(p3y - p2y);
		
		for(int32_t y = sy; y <= ey; y++)
		{
			sx = p3x - ((ey - y) * slopeL + 0.5f);
			ex = p3x - ((ey - y) * slopeR + 0.5f);

			if(sx > ex)
			{
				buf = sx;
				sx = ex;
				ex = buf;
			}

 			if(sx < 0)
				sx = 0;
		
			if(ex > width - 1)
				ex = width - 1;

			count = ex - sx + 1;
			if(count < 0)
				count = 0;

			fillDotArray(y * width * offset + sx * offset, count, getBrushColor());
		}
	}
}

void Brush::fillQuadrangle(Position p1, Position p2, Position p3, Position p4)
{
	fillTriangle(p1, p2, p4);
	fillTriangle(p2, p3, p4);
}

Size Brush::calculateStringArea(const char *str)
{
	Size size;

	if(mFont == nullptr)
		return size;

	Font::fontInfo_t *fontInfo;
	uint8_t spaceWidth = mFont->getSpaceWidth(), dotWidth = mFont->getDotWidth(), charWidth = mFont->getCharWidth();
	uint32_t utf8;

	while (*str)
	{
		utf8 = mFont->getUtf8(&str);
		fontInfo = mFont->getFontInfo(utf8);

		if(fontInfo == nullptr)
		{
			if(spaceWidth > 0)
				size.addWidth(spaceWidth);
			else
				size.addWidth(DEFAULT_SPACE_WIDTH);

			continue;
		}

		if(utf8 == '.')
		{
			if(dotWidth > 0)
			{
				if(fontInfo->width + (int8_t)fontInfo->xpos > charWidth)
					size.addWidth(fontInfo->width + (int8_t)fontInfo->xpos);
				else
					size.addWidth(dotWidth);
			}
			else
				size.addWidth(fontInfo->width + (int8_t)fontInfo->xpos);
		}
		else if(utf8 == ' ')
		{
			if(spaceWidth > 0)
				size.addWidth(spaceWidth);
			else
				size.addWidth(DEFAULT_SPACE_WIDTH);
		}
		else 
		{
			if(charWidth > 0)
			{
				if(fontInfo->width + (int8_t)fontInfo->xpos > charWidth)
					size.addWidth(fontInfo->width + (int8_t)fontInfo->xpos);
				else
					size.addWidth(charWidth);
			}
			else
				size.addWidth(fontInfo->width + (int8_t)fontInfo->xpos);
		}

		if(fontInfo->xpos == 0) 
			size.addWidth(1); 

		if(size.getHeight() < (int8_t)fontInfo->ypos + fontInfo->height)
		{
			size.setHeight((int8_t)fontInfo->ypos + fontInfo->height);
		}
	}

	return size;
}

uint8_t Brush::drawChar(Position pos, uint32_t utf8)
{
	if (mFont == 0)
		return 0;

	Font::fontInfo_t *fontInfo = mFont->getFontInfo(utf8);
	uint8_t spaceWidth = mFont->getSpaceWidth(), dotWidth = mFont->getDotWidth(), charWidth = mFont->getCharWidth();

	if(utf8 == ' ')
	{
		if(spaceWidth > 0)
			return spaceWidth;
		else
			return DEFAULT_SPACE_WIDTH;
	}

	if(fontInfo == 0)
		return 0;
	
	uint8_t *fontFb, color;
	int32_t  index = 0;
	int16_t width = fontInfo->width, height = fontInfo->height, offset = 0, xoffset;
	int16_t xs = pos.getX(), ys = pos.getY() + (int8_t)fontInfo->ypos;
	int16_t cwidth = getCanvasSize().getWidth(), cheight = getCanvasSize().getHeight();
	
	fontFb = fontInfo->data;
	xoffset = (int8_t)fontInfo->xpos;

	if(xoffset == 0) // 문자의 앞 여백이 없을 경우
		xoffset = 1; // 문자의 앞 여백 하나를 추가해줌
	
	xs += xoffset;

	if(width & 0x01) // 문자 크기가 홀수 일 경우
		width++; // 짝수로 만들어줌

	if (xs + width > cwidth)
	{
		width = cwidth - xs;
		offset = fontInfo->width - width;
	}
	if (ys + height > cheight)
		height = cheight - ys;

	width += xs;
	height += ys;

	for (int32_t  y = ys; y < height; y++)
	{
		for (int32_t  x = xs; x < width; x++, index++)
		{
			if (index % 2 == 0)
			{
				color = (fontFb[index / 2] & 0x0f) << 4 | 0x0F;
				blendDot(x, y, color);
			}
			else
			{
				color = fontFb[index / 2] | 0x0f;
				blendDot(x, y, color);
			}
		}
		index += offset;
	}

	if(utf8 == '.')
	{
		if(dotWidth > 0)
		{
			if(fontInfo->width + xoffset > charWidth)
				return fontInfo->width + xoffset;
			else
				return dotWidth;
		}
		else
			return fontInfo->width + xoffset;
	}
	else 
	{
		if(charWidth > 0)
		{
			if(fontInfo->width + xoffset > charWidth)
				return fontInfo->width + xoffset;
			else
				return charWidth;
		}
		else
			return fontInfo->width + xoffset;
	}
}

void Brush::setFont(Font &font)
{
	mFont = &font;
}

Font* Brush::getFont(void)
{
	return mFont;
}

Position Brush::drawString(Position pos, const char *str)
{
	if(mFont == 0)
		return pos;

	while (*str)
		pos.addX(drawChar(pos, mFont->getUtf8(&str)));

	return pos;
}

Position Brush::drawString(align_t align, const char *str)
{
	Position pos;
	Size size = calculateStringArea(str), csize = getCanvasSize();

	if(size.getWidth() == 0)
		return pos;
	
	switch(align & 0x7)
	{
	default :
	case 0x01 : // LEFT
		break;
	
	case 0x02 : // CENTER
		pos.setX(((int32_t)csize.getWidth() - (int32_t)size.getWidth()) / 2);
		break;
	
	case 0x04 : // RIGHT
		pos.setX((int32_t)csize.getWidth() - (int32_t)size.getWidth());
		break;
	}

	switch(align >> 3)
	{
	default :
	case 0x01 : // TOP
		break;
	
	case 0x02 : // MID
		pos.setY(((int32_t)csize.getHeight() - (int32_t)size.getHeight()) / 2);
		break;
	
	case 0x04 : // BOT
		pos.setY((int32_t)csize.getHeight() - (int32_t)size.getHeight());
		break;
	}

	while (*str)
		pos.addX(drawChar(pos, mFont->getUtf8(&str)));
	
	return pos;
}

void Brush::drawBitmap(Position pos, const bitmap_t bitmap)
{
	Size size = getCanvasSize();
	drawBitmapBase(size, {pos, size}, pos, bitmap);
}

void Brush::drawBitmap(Rectangular rect, Position bitmapPos, const bitmap_t bitmap)
{
	drawBitmapBase(getCanvasSize(), rect, bitmapPos, bitmap);
}

bool Brush::calculate2BytesPixelDrawingInfo(Rectangular &des, Rectangular &src, uint16_t **frameBuffer)
{
	int16_t sx = src.getPosition().getX(), sy = src.getPosition().getY();
	int16_t dx = des.getPosition().getX(), dy = des.getPosition().getY();
	uint16_t dwidth = des.getSize().getWidth(), swidth = src.getSize().getWidth(), owidth = swidth;
	uint16_t dheight = des.getSize().getHeight(), sheight = src.getSize().getHeight(), buf;

	//if(sx + swidth < dx || dx + dwidth < sx || sy + sheight < dy || dy + dheight < sy)
	//	return false;

	if(sx + swidth < dx)
		return false;

	if(dx + dwidth < sx)
		return false;
	
	if(sy + sheight < dy)
		return false;

	if(dy + dheight < sy)
		return false;

	if(sx < dx)
	{
		buf = dx - sx;
		*frameBuffer = *frameBuffer + buf;
		sx = dx;
		src.setX(dx);
		src.subWidth(buf);
		swidth = src.getSize().getWidth();
	}
	
	if(sx + swidth > dx + dwidth)
	{
		buf = swidth + sx - dwidth - dx;
		src.subWidth(buf);
		swidth = src.getSize().getWidth();
	}

	if(sy < dy)
	{
		buf = dy - sy;
		*frameBuffer = *frameBuffer + (buf * owidth);
		src.subHeight(buf);
		src.setY(dy);
		sy = dy;
		sheight = src.getSize().getHeight();
	}
	
	if(sy + sheight > dy + dheight)
	{
		buf = sheight + sy - dheight - dy;
		src.subHeight(buf);
	}

	return true;
}


bool Brush::checkDrawingAble(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea)
{
	int16_t cax = canvasDesArea.getPosition().getX();
	int16_t cay = canvasDesArea.getPosition().getY();
	uint16_t caw = canvasDesArea.getSize().getWidth();
	uint16_t cah = canvasDesArea.getSize().getHeight();
	uint16_t cw = canvasSize.getWidth();
	uint16_t ch = canvasSize.getHeight();
	int16_t bx = bitmapArea.getPosition().getX();
	int16_t by = bitmapArea.getPosition().getY();
	uint16_t baw = bitmapArea.getSize().getWidth();
	uint16_t bah = bitmapArea.getSize().getHeight();
	
	if(cax < 0 || cax > cw  || cay < 0 || cay > ch)
		return false;

	if(bx + baw < cax || by + bah < cay)
		return false;

	if(cax + caw < bx || cay + cah < by)
		return false;
	
	return true;
}

uint32_t Brush::calculateSrcFrameBufferOffset(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea)
{
	int16_t cax = canvasDesArea.getPosition().getX();
	int16_t cay = canvasDesArea.getPosition().getY();
	int16_t bx = bitmapArea.getPosition().getX();
	int16_t by = bitmapArea.getPosition().getY();
	uint32_t offset = 0;

	if(bx < cax)
		offset += cax - bx;

	if(by < cay)
		offset += (cay - by) * bitmapArea.getSize().getWidth();

	return offset;
}

uint16_t Brush::calculateSrcWidth(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea)
{
	int16_t cax = canvasDesArea.getPosition().getX();
	uint16_t caw = canvasDesArea.getSize().getWidth();
	int16_t bx = bitmapArea.getPosition().getX();
	uint16_t baw = bitmapArea.getSize().getWidth();
	uint16_t bwidth = baw;

	if(bx < cax)
		bwidth -= cax - bx;

	if(bx + bwidth > cax + caw)
		bwidth -= baw + bx - caw - cax;
	
	return bwidth;	
}

uint32_t Brush::calculateDesFrameBufferOffset(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea)
{
	int16_t by = bitmapArea.getPosition().getY();
	int16_t bx = bitmapArea.getPosition().getX();

	if(bx < 0)
		bx = 0;
	
	if(by < 0)
		by = 0;
	
	return canvasSize.getWidth() * by + bx;
}

uint16_t Brush::calculateSrcHeight(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea)
{
	int16_t by = bitmapArea.getPosition().getY();
	uint16_t bah = bitmapArea.getSize().getHeight();
	uint16_t cah = canvasDesArea.getSize().getHeight();

	if(by < 0)
	{
		bah += by;
		by = 0;
	}

	if(by + bah > cah)
		bah = cah - by;

	return bah;
}

uint32_t calculateSrcLineOffset(Size &canvasSize, Rectangular &canvasDesArea, Rectangular &bitmapArea)
{

}


