/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Brush.cpp
 * @brief Implementation of the device-independent 2D drawing algorithms of the Brush class.
 */

#include "Brush.h"
#include <math.h>
#include "Font.h"

/**
 * @brief Default horizontal advance in pixels for an ASCII space character when the active font does not specify one.
 */
#define DEFAULT_SPACE_WIDTH		4

/**
 * @details Initializes the `Brush` instance with `mFont` set to `nullptr` so text rendering functions
 * safely no-op until a font is attached via `setFont()`.
 */
Brush::Brush(void)
{
	mFont = nullptr;
}

/**
 * @details Fills a rectangular region on the canvas row by row using `fillDotArray()`.
 *
 * Queries the canvas dimensions (`cwidth`, `cheight`) and bytes-per-pixel (`offset`), clamps the
 * bottom-right corner (`ex`, `ey`) to the canvas boundaries, and iterates over each scanline `i`
 * from `y` to `ey`. For each scanline, computes the byte offset `des` in the frame buffer and fills
 * `width` consecutive pixels with `color`.
 * @see Brush::fillDotArray()
 */
void Brush::fillRectBase(int16_t x, int16_t y, uint16_t width, uint16_t height, Color color)
{
	uint16_t cwidth = getCanvasSize().getWidth(), cheight = getCanvasSize().getHeight();
	int16_t ex = x + width, ey = y + height, offset = getPixelCapacity();
	uint32_t des;

	// Clamp end coordinates to the maximum valid canvas indices
	if(ey >= cheight - 1)
		ey = cheight - 1;
	if(ex >= cwidth - 1)
		ex = cwidth - 1;

	// Fill each horizontal scanline of the rectangle
	for (int16_t i = y; i <= ey; i++)
	{
		des = i * cwidth * offset + x * offset;
		fillDotArray(des, width, color);
	}
}

/**
 * @details Extracts the position and dimensions from `rect` and delegates to `fillRectBase()`
 * using the current foreground brush color (`getBrushColor()`).
 * @see Brush::fillRectBase()
 */
void Brush::fillRectangular(Area rect)
{
	Position pos = rect.getPosition();
	Size size = rect.getSize();

	fillRectBase(pos.getX(), pos.getY(), size.getWidth(), size.getHeight(), getBrushColor());
}

/**
 * @details Extracts the position and dimensions from `rect` and delegates to `fillRectBase()`
 * using the current background color (`getBackgroundColor()`).
 * @see Brush::fillRectBase()
 */
void Brush::clearRectangular(Area rect)
{
	Position pos = rect.getPosition();
	Size size = rect.getSize();

	fillRectBase(pos.getX(), pos.getY(), size.getWidth(), size.getHeight(), getBackgroundColor());
}

/**
 * @details Queries the full canvas size via `getCanvasSize()` and fills the entire canvas from `(0, 0)`
 * with the current foreground brush color via `fillRectBase()`.
 * @see Brush::fillRectBase()
 */
void Brush::fill()
{
	Size size = getCanvasSize();

	fillRectBase(0, 0, size.getWidth(), size.getHeight(), getBrushColor());
}

/**
 * @details Queries the full canvas size via `getCanvasSize()` and fills the entire canvas from `(0, 0)`
 * with the current background color via `fillRectBase()`.
 * @see Brush::fillRectBase()
 */
void Brush::clear()
{
	Size size = getCanvasSize();

	fillRectBase(0, 0, size.getWidth(), size.getHeight(), getBackgroundColor());
}

/**
 * @details Renders a solid filled circle centered at `pos` with the given `radius` using horizontal
 * scanline spans.
 *
 * For each vertical offset `i` from `0` to `radius - 1`:
 * -# Computes the half-chord length $\sqrt{r^2 - (i + 1)^2}$ using `pow(..., 0.5f)`.
 * -# Determines the left and right X bounds (`sx`, `ex`) around `px` and clamps them to `[0, width - 1]`.
 * -# Fills the lower scanline at `y = py + i + 1` and the upper symmetric scanline at `y = py - i`
 *    via `fillDotArray()` if they lie within `[0, height - 1]`.
 * @see Brush::drawCircle(), Brush::fillDotArray()
 */
void Brush::fillCircle(Position pos, uint16_t radius)
{
	float r = radius, buf;
	int32_t sx, ex, count, y, px = pos.getX(), py = pos.getY();
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	int16_t offset = getPixelCapacity();
	Color color = getBrushColor();
	uint32_t des;

	// Circles with a radius smaller than 3 pixels are not drawn
	if (radius < 3)
		return;

	for (uint16_t i = 0; i < radius; i++)
	{
		// Compute horizontal chord half-width at vertical distance (i + 1) from center
		buf = i + 1;
		buf = r * r - buf * buf;
		ex = sx = pow(buf, (float)0.5) + 0.5f;

		sx = px - sx + 1;
		ex = px + ex;

		// Clamp horizontal span to canvas bounds
		if(sx < 0)
			sx = 0;
		if(ex > width - 1)
			ex = width - 1;
		
		// Fill lower hemisphere scanline
		y = py + i + 1;
		if(y < height)
		{
			des = y * width * offset + sx * offset;
			count = ex - sx;
			if(count > 0)
				fillDotArray(des, ex - sx, color);
		}
		
		// Fill upper hemisphere scanline
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

/**
 * @details Draws the perimeter of a circle centered at `pos` with the given `radius` using 8-way
 * octant/quadrant symmetry.
 *
 * Iterates `x` from `0` to `radius - 1`, calculates the corresponding vertical offset
 * $y_p = \lfloor\sqrt{r^2 - x^2} + 0.5\rfloor$, and plots 8 symmetric points around `(px, py)`
 * using `drawDot()`.
 * @see Brush::fillCircle(), Brush::drawDot()
 */
void Brush::drawCircle(Position pos, uint16_t radius)
{
	int16_t px = pos.getX(), py = pos.getY();
	float r = radius, x, yp, yn;
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	int16_t offset = getPixelCapacity();
	uint32_t color = getBrushColor().getCode();
	uint32_t des;

	// Circles with a radius smaller than 3 pixels are not drawn
	if (radius < 3)
		return;

	for (uint16_t i = 0; i < radius; i++)
	{
		x = i;
		yp = r * r - x * x;
		yp = pow(yp, (float)0.5) + (float)0.5;
		yn = yp - (float)1.0;
		
		// Plot 8 symmetric points along the circle perimeter
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

/**
 * @details Draws a straight line segment from `p1` to `p2` by stepping along the major axis
 * ($\Delta x$ or $\Delta y$) and interpolating the minor axis via floating-point slope calculation.
 *
 * Handles degenerate single-point lines (`p1 == p2`) and all four directional quadrants
 * (`p1x <= p2x && p1y <= p2y`, `p1x >= p2x && p1y <= p2y`, `p1x <= p2x && p1y >= p2y`, and
 * `p1x > p2x && p1y > p2y`), calling `drawDot()` for each interpolated pixel.
 * @see Brush::drawDot()
 */
void Brush::drawLine(Position p1, Position p2)
{
	uint16_t p1x = p1.getX(), p1y = p1.getY(), p2x = p2.getX(), p2y = p2.getY();
	uint16_t lenX, lenY, x, y;
	uint16_t width = getCanvasSize().getWidth(), height = getCanvasSize().getHeight();
	float slope;

	// Single pixel case when start and end positions are identical
	if(p1x == p2x && p1y == p2y)
	{
		drawDot(p1x, p2y);
		return;
	}

	// Quadrant 1: right-downward direction (p1x <= p2x, p1y <= p2y)
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
	// Quadrant 2: left-downward direction (p1x >= p2x, p1y <= p2y)
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
	// Quadrant 3: right-upward direction (p1x <= p2x, p1y >= p2y)
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
	// Quadrant 4: left-upward direction (p1x > p2x, p1y > p2y), swapped to right-downward
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

/**
 * @details Draws the three edges of a triangle (`p1` to `p2`, `p2` to `p3`, and `p3` to `p1`)
 * by invoking `drawLine()` three times.
 * @see Brush::drawLine(), Brush::fillTriangle()
 */
void Brush::drawTriangle(Position p1, Position p2, Position p3)
{
	drawLine(p1, p2);
	drawLine(p2, p3);
	drawLine(p3, p1);
}

/**
 * @details Draws the four border edges of `rect` clockwise starting from its top-left corner
 * using `drawLine()`.
 * @see Brush::drawLine(), Brush::fillRectangular()
 */
void Brush::drawRectangular(Area rect)
{
	Position p1, p2;
	Size size = rect.getSize();
	
	// Top edge
	p1 = rect.getPosition();
	p2 = p1;
	p2.addX(size.getWidth());
	drawLine(p1, p2);
	
	// Right edge
	p1 = p2;
	p1.addY(size.getHeight());
	drawLine(p1, p2);
	
	// Bottom edge
	p2 = p1;
	p2.subX(size.getWidth());	
	drawLine(p1, p2);

	// Left edge
	p1 = p2;
	p1.subY(size.getHeight());
	drawLine(p1, p2);
}

/**
 * @details Rasterizes a filled triangle by sorting the three vertices and scanning horizontal spans.
 *
 * -# Sorts the vertices so that `p1` is at the top (minimum Y) and orders `p2`, `p3` horizontally.
 * -# Rasterizes the upper sub-triangle from `p1.y` down to $\min(p2.y, p3.y)$ by computing the left
 *    and right edge slopes (`slopeL`, `slopeR`) and filling each scanline span via `fillDotArray()`.
 * -# If `p2.y != p3.y`, reorders the remaining vertices so `p3` is the bottom-most vertex and
 *    rasterizes the lower sub-triangle from $\max(p1.y, p2.y)$ down to `p3.y`.
 * @see Brush::drawTriangle(), Brush::fillDotArray()
 */
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

	// Sort vertices so that p1 has the smallest Y coordinate (top-most vertex)
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
	
	// Order p2 and p3 from left to right along the X axis
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
	
	// Rasterize the upper part of the triangle
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

	// Rasterize the lower part of the triangle when the bottom edge is not horizontal
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

/**
 * @details Fills a quadrangle defined by vertices `(p1, p2, p3, p4)` by splitting it along the
 * diagonal `(p2, p4)` into two triangles `(p1, p2, p4)` and `(p2, p3, p4)` and calling `fillTriangle()`.
 * @see Brush::fillTriangle()
 */
void Brush::fillQuadrangle(Position p1, Position p2, Position p3, Position p4)
{
	fillTriangle(p1, p2, p4);
	fillTriangle(p2, p3, p4);
}

/**
 * @details Iterates over each UTF-8 character in `str`, queries its glyph metrics from `mFont`,
 * and accumulates the total horizontal advance width and maximum vertical height (`ypos + height`).
 *
 * Accounts for custom space width (`mFont->getSpaceWidth()`), custom dot width (`mFont->getDotWidth()`),
 * fixed minimum character width (`mFont->getCharWidth()`), and adds a 1-pixel leading margin when
 * `fontInfo->xpos == 0`.
 * @see Font::getFontInfo(), Brush::drawString()
 */
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

		// Fallback to space width if glyph information is missing
		if(fontInfo == nullptr)
		{
			if(spaceWidth > 0)
				size.addWidth(spaceWidth);
			else
				size.addWidth(DEFAULT_SPACE_WIDTH);

			continue;
		}

		// Accumulate horizontal advance based on character type and font width overrides
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

		// Add 1 pixel of leading margin when the glyph has zero horizontal offset
		if(fontInfo->xpos == 0) 
			size.addWidth(1); 

		// Track maximum vertical extent across all glyphs in the string
		if(size.getHeight() < (int8_t)fontInfo->ypos + fontInfo->height)
		{
			size.setHeight((int8_t)fontInfo->ypos + fontInfo->height);
		}
	}

	return size;
}

/**
 * @details Renders a single anti-aliased glyph from `mFont` onto the canvas using `blendDot()`.
 *
 * -# Returns `0` if no font is set, or returns the space advance width if `utf8 == ' '`.
 * -# Looks up `Font::fontInfo_t` for `utf8`. Each byte in `fontInfo->data` packs two 4-bit alpha
 *    nibbles (even pixel index in the low nibble, odd pixel index in the high nibble), padded to
 *    an even row width.
 * -# Applies the glyph bearing offsets (`xpos`, `ypos`), ensuring at least 1 pixel of leading
 *    horizontal offset if `xpos == 0`, and clips the glyph rectangle to the canvas bounds.
 * -# Iterates through the visible pixels of the glyph, expanding each 4-bit alpha value to 8 bits
 *    and blending it onto the canvas via `blendDot()`.
 * -# Returns the horizontal advance width to the next character cell.
 * @see Brush::blendDot(), Font::getFontInfo()
 */
uint8_t Brush::drawChar(Position pos, uint32_t utf8)
{
	if (mFont == 0)
		return 0;

	Font::fontInfo_t *fontInfo = mFont->getFontInfo(utf8);
	uint8_t spaceWidth = mFont->getSpaceWidth(), dotWidth = mFont->getDotWidth(), charWidth = mFont->getCharWidth();

	// Handle ASCII space character without drawing any pixels
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

	if(xoffset == 0) // If the character has no leading horizontal margin
		xoffset = 1; // Add a 1-pixel leading margin
	
	xs += xoffset;

	if(width & 0x01) // If the character width is odd
		width++; // Round up to an even width to match packed 4-bit nibble alignment

	// Clip glyph width and height against canvas boundaries
	if (xs + width > cwidth)
	{
		width = cwidth - xs;
		offset = fontInfo->width - width;
	}
	if (ys + height > cheight)
		height = cheight - ys;

	width += xs;
	height += ys;

	// Render 4-bit packed alpha bitmap via blendDot()
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

	// Return horizontal advance width for the drawn character
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

/**
 * @details Stores the address of `font` in `mFont` for subsequent text measurement and rendering operations.
 */
void Brush::setFont(Font &font)
{
	mFont = &font;
}

/**
 * @details Returns the pointer to the currently configured `Font` object (`mFont`).
 */
Font* Brush::getFont()
{
	return mFont;
}

/**
 * @details Sequentially decodes each UTF-8 character from `str` via `mFont->getUtf8()`, draws it at
 * `pos` using `drawChar()`, and advances `pos` horizontally by the returned character width.
 * @see Brush::drawChar(), Font::getUtf8()
 */
Position Brush::drawString(Position pos, const char *str)
{
	if(mFont == 0)
		return pos;

	while (*str)
		pos.addX(drawChar(pos, mFont->getUtf8(&str)));

	return pos;
}

/**
 * @details Measures the bounding box of `str` via `calculateStringArea()`, computes the starting
 * coordinate `pos` within the canvas (`getCanvasSize()`) according to the horizontal (`align & 0x07`)
 * and vertical (`align >> 3`) alignment flags (with a 2-pixel margin from canvas edges for left/right/top/bottom),
 * and then renders each UTF-8 character in `str`.
 * @see Brush::calculateStringArea(), Brush::drawChar()
 */
Position Brush::drawString(align_t align, const char *str)
{
	Position pos;
	Size size = calculateStringArea(str), csize = getCanvasSize();

	if(size.getWidth() == 0)
		return pos;
	
	// Determine starting X coordinate from horizontal alignment bits [2:0]
	switch(align & 0x7)
	{
	default :
	case 0x01 : // LEFT
		pos.setX(2);
		break;
	
	case 0x02 : // CENTER
		pos.setX(((int32_t)csize.getWidth() - (int32_t)size.getWidth()) / 2);
		break;
	
	case 0x04 : // RIGHT
		pos.setX((int32_t)csize.getWidth() - (int32_t)size.getWidth() - 2);
		break;
	}

	// Determine starting Y coordinate from vertical alignment bits [5:3]
	switch(align >> 3)
	{
	default :
	case 0x01 : // TOP
		pos.setY(2);
		break;
	
	case 0x02 : // MID
		pos.setY(((int32_t)csize.getHeight() - (int32_t)size.getHeight()) / 2);
		break;
	
	case 0x04 : // BOT
		pos.setY((int32_t)csize.getHeight() - (int32_t)size.getHeight() - 2);
		break;
	}

	// Draw each character sequentially across the line
	while (*str)
		pos.addX(drawChar(pos, mFont->getUtf8(&str)));
	
	return pos;
}

/**
 * @details Forwards the bitmap drawing request to the pure virtual `drawBitmapBase(pos, bitmap)`
 * implemented by the derived canvas or LCD driver class.
 * @see Brush::drawBitmapBase()
 */
void Brush::drawBitmap(Position pos, const bitmap_t bitmap)
{
	drawBitmapBase( pos, bitmap);
}

/**
 * @details Forwards the clipped bitmap drawing request to `drawBitmapBase(getCanvasSize(), rect, bitmapPos, bitmap)`.
 * @see Brush::drawBitmapBase()
 */
void Brush::drawBitmap(Area rect, Position bitmapPos, const bitmap_t bitmap)
{
	drawBitmapBase(getCanvasSize(), rect, bitmapPos, bitmap);
}

/**
 * @details Clips the source rectangle `src` against the destination rectangle `des` for a 16-bit
 * (2 bytes per pixel) buffer and advances the source pixel pointer `*frameBuffer` by any clipped
 * left columns or top rows.
 *
 * Returns `false` immediately if `src` and `des` do not overlap.
 */
bool Brush::calculate2BytesPixelDrawingInfo(Area &des, Area &src, uint16_t **frameBuffer)
{
	int16_t sx = src.getPosition().getX(), sy = src.getPosition().getY();
	int16_t dx = des.getPosition().getX(), dy = des.getPosition().getY();
	uint16_t dwidth = des.getSize().getWidth(), swidth = src.getSize().getWidth(), owidth = swidth;
	uint16_t dheight = des.getSize().getHeight(), sheight = src.getSize().getHeight(), buf;

	//if(sx + swidth < dx || dx + dwidth < sx || sy + sheight < dy || dy + dheight < sy)
	//	return false;

	// Return false if source and destination areas do not intersect
	if(sx + swidth < dx)
		return false;

	if(dx + dwidth < sx)
		return false;
	
	if(sy + sheight < dy)
		return false;

	if(dy + dheight < sy)
		return false;

	// Clip left overhang and advance source pixel pointer horizontally
	if(sx < dx)
	{
		buf = dx - sx;
		*frameBuffer = *frameBuffer + buf;
		sx = dx;
		src.setX(dx);
		src.subWidth(buf);
		swidth = src.getSize().getWidth();
	}
	
	// Clip right overhang
	if(sx + swidth > dx + dwidth)
	{
		buf = swidth + sx - dwidth - dx;
		src.subWidth(buf);
		swidth = src.getSize().getWidth();
	}

	// Clip top overhang and advance source pixel pointer by full source scanlines
	if(sy < dy)
	{
		buf = dy - sy;
		*frameBuffer = *frameBuffer + (buf * owidth);
		src.subHeight(buf);
		src.setY(dy);
		sy = dy;
		sheight = src.getSize().getHeight();
	}
	
	// Clip bottom overhang
	if(sy + sheight > dy + dheight)
	{
		buf = sheight + sy - dheight - dy;
		src.subHeight(buf);
	}

	return true;
}

/**
 * @details Verifies that `canvasDesArea` starts within the valid canvas dimensions (`[0, cw]`, `[0, ch]`)
 * and that `bitmapArea` and `canvasDesArea` overlap on both X and Y axes.
 */
bool Brush::checkDrawingAble(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea)
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

/**
 * @details Computes the linear pixel offset into the source bitmap buffer when the bitmap's top-left
 * corner `(bx, by)` lies to the left of or above `canvasDesArea` `(cax, cay)`.
 */
uint32_t Brush::calculateSrcFrameBufferOffset(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea)
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

/**
 * @details Computes the visible width of `bitmapArea` after clipping its left and right edges
 * against `canvasDesArea`.
 */
uint16_t Brush::calculateSrcWidth(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea)
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

/**
 * @details Computes the linear pixel index (`canvasSize.getWidth() * by + bx`) in the destination
 * canvas frame buffer after clamping negative `bx` and `by` coordinates to `0`.
 */
uint32_t Brush::calculateDesFrameBufferOffset(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea)
{
	int16_t by = bitmapArea.getPosition().getY();
	int16_t bx = bitmapArea.getPosition().getX();

	if(bx < 0)
		bx = 0;
	
	if(by < 0)
		by = 0;
	
	return canvasSize.getWidth() * by + bx;
}

/**
 * @details Computes the visible height of `bitmapArea` after clipping negative top coordinates (`by < 0`)
 * and bottom coordinates exceeding `canvasDesArea` height (`cah`).
 */
uint16_t Brush::calculateSrcHeight(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea)
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

/**
 * @details Stores the position and dimensions of `src` into `mSrcX`, `mSrcY`, `mSrcWidth`, and `mSrcHeight`,
 * resets `mSrcOffset` to `0`, and caches the current canvas dimensions from `getCanvasSize()` into
 * `mCanvasWidth` and `mCanvasHeight`.
 * @see Brush::calculateValidArea(), Brush::isOutsideCanvas()
 */
void Brush::setCalculatorSource(Area src)
{
	Position pos = src.getPosition();
	Size size = src.getSize();

	mSrcX = pos.getX();
	mSrcY = pos.getY();
	mSrcWidth = size.getWidth();
	mSrcHeight = size.getHeight();
	mSrcOffset = 0;
	
	size = getCanvasSize();
	mCanvasWidth = size.getWidth();
	mCanvasHeight = size.getHeight();
}

/**
 * @details Clips the source rectangle (`mSrcX`, `mSrcY`, `mSrcWidth`, `mSrcHeight`) against the cached
 * canvas dimensions (`mCanvasWidth`, `mCanvasHeight`) and accumulates any clipped top/left pixel offset
 * into `mSrcOffset`.
 * @see Brush::setCalculatorSource()
 */
Area Brush::calculateValidArea()
{
	Area validArea(mSrcX, mSrcY, mSrcWidth, mSrcHeight);
	int32_t buf;

	// Clip left overhang (mSrcX < 0) and advance source pixel offset
	if(mSrcX < 0)
	{
		buf = 0 - mSrcX;
		validArea.setX(0);
		validArea.subWidth(buf);
		mSrcOffset += buf;
	}

	// Clip top overhang (mSrcY < 0) and advance source pixel offset by full rows
	if(mSrcY < 0)
	{
		buf = 0 - mSrcY;
		validArea.setY(0);
		validArea.subHeight(buf);
		mSrcOffset += mSrcWidth * buf;
	}
	
	// Clip right overhang exceeding canvas width
	buf = mSrcX + mSrcWidth;
	if(buf > mCanvasWidth)
	{
		validArea.subWidth(buf - mCanvasWidth);
	}

	// Clip bottom overhang exceeding canvas height
	buf = mSrcY + mSrcHeight;
	if(buf > mCanvasHeight)
	{
		validArea.subHeight(buf - mCanvasHeight);
	}

	return validArea;
}

/**
 * @details Returns `true` if any of the four edges of the source rectangle (`mSrcX`, `mSrcY`,
 * `mSrcX + mSrcWidth`, `mSrcY + mSrcHeight`) lies outside `[0, mCanvasWidth]` or `[0, mCanvasHeight]`.
 * @see Brush::setCalculatorSource(), Brush::calculateValidArea()
 */
bool Brush::isOutsideCanvas()
{
	if(mSrcX < 0)
		return true;
	
	if(mSrcY < 0)
		return true;
	
	if(mSrcX + mSrcWidth > mCanvasWidth)
		return true;
	
	if(mSrcY + mSrcHeight > mCanvasHeight)
		return true;

	return false;
}
