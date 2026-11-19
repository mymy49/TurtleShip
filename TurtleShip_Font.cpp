/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_Font.cpp
 * @brief Implementation of the Font base class for UTF-8 decoding and string width calculation.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#include <config.h>

#include "Font.h"

/**
 * @details Initializes all font metric fields (`mSpaceWidth`, `mCharWidth`, `mDotWidth`, `mSize`) to `0`.
 * @see Font
 * @todo None.
 * @bug None.
 */
Font::Font(void)
{
	mSpaceWidth = 0;
	mCharWidth = 0;
	mDotWidth = 0;
	mSize = 0;
}

/**
 * @details Returns the custom space advance width (`mSpaceWidth`).
 * @see Font
 * @todo None.
 * @bug None.
 */
uint8_t Font::getSpaceWidth(void)
{
	return mSpaceWidth;
}

/**
 * @details Updates `mSpaceWidth` with the specified pixel width for space characters (`' '`).
 * @see Font
 * @todo None.
 * @bug None.
 */
void Font::setSpaceWidth(uint8_t width)
{
	mSpaceWidth = width;
}

/**
 * @details Returns the configured minimum character advance width (`mCharWidth`).
 * @see Font
 * @todo None.
 * @bug None.
 */
uint8_t Font::getCharWidth(void)
{
	return mCharWidth;
}

/**
 * @details Updates `mCharWidth` with the specified minimum pixel advance width for characters.
 * @see Font
 * @todo None.
 * @bug None.
 */
void Font::setCharWidth(uint8_t width)
{
	mCharWidth = width;
}

/**
 * @details Returns the configured period/dot advance width (`mDotWidth`).
 * @see Font
 * @todo None.
 * @bug None.
 */
uint8_t Font::getDotWidth(void)
{
	return mDotWidth;
}

/**
 * @details Updates `mDotWidth` with the specified pixel advance width for the period character (`'.'`).
 * @see Font
 * @todo None.
 * @bug None.
 */
void Font::setDotWidth(uint8_t width)
{
	mDotWidth = width;
}

/**
 * @details Inspects the leading byte at `*(*src)` to determine the UTF-8 sequence length and packs
 * the raw UTF-8 bytes into a 32-bit unsigned integer:
 * - **3-byte sequence (`>= 0xE0`)**: Packs 3 consecutive bytes into bits `[23:16]`, `[15:8]`, and `[7:0]`.
 * - **2-byte sequence (`>= 0xC0`)**: Packs 2 consecutive bytes into bits `[15:8]` and `[7:0]`.
 * - **1-byte ASCII (`< 0x80`)**: Returns the single byte in bits `[7:0]`.
 * - **Continuation/invalid byte (`0x80` ~ `0xBF`)**: Advances `*src` by 1 byte and returns `0`.
 * @see Font
 * @todo None.
 * @bug None.
 */
uint32_t Font::getUtf8(const char **src)
{
	uint32_t utf8;

	// 3-byte UTF-8 sequence (e.g. Korean Hangul syllables and CJK characters)
	if (*(*src) >= 0xe0)
	{
		utf8 = (uint32_t)*(*src)++ << 16;
		utf8 |= (uint32_t)*((*src)++) << 8;
		utf8 |= *(*src)++;
		return utf8; 
	}
	// 2-byte UTF-8 sequence
	else if (*(*src) >= 0xc0)
	{
		utf8 = (uint32_t)*((*src)++) << 8;
		utf8 |= *(*src)++;
		return utf8; 
	}
	// 1-byte standard ASCII character (0x00 ~ 0x7F)
	else if (*(*src) < 0x80)
	{
		return *(*src)++;
	}

	// Unexpected continuation byte; skip and return 0
	(*src)++;
	return 0;
}

/**
 * @details Decodes every UTF-8 character in `str` via `getUtf8()` and sums the horizontal advance
 * widths of all characters.
 *
 * - When `mCharWidth` is non-zero, each character advances by at least `mCharWidth` pixels (or its
 *   actual glyph width if wider).
 * - When `mCharWidth` is zero, proportional glyph widths (`fontInfo->width + (int8_t)fontInfo->xpos`,
 *   plus 1 pixel if `xpos == 0`) are summed, using `mSpaceWidth` (or 4) for spaces and `mDotWidth`
 *   (or 4) for periods.
 * @see Font::getUtf8(), Font::getFontInfo()
 * @todo None.
 * @bug None.
 */
uint16_t Font::getStringWidth(const char *str)
{
	uint16_t width = 0, buf = 0;
	uint32_t utf8;
	Font::fontInfo_t *fontInfo;

	if(mCharWidth)
	{
		// Fixed minimum character width mode
		while (*str)
		{
			utf8 = getUtf8(&str);
			if (utf8 == ' ' && mSpaceWidth > 0)
				buf = mSpaceWidth;
			else if(utf8 == '.' && mDotWidth > 0)
				buf = mDotWidth;
			else
			{
				fontInfo = getFontInfo(utf8);
				if(fontInfo != 0)
				{
					if(fontInfo->xpos == 0)
						buf = fontInfo->width + (int8_t)fontInfo->xpos + 1;
					else
						buf = fontInfo->width + (int8_t)fontInfo->xpos;
				}
			}

			if(buf > mCharWidth)
				width += buf;
			else
				width += mCharWidth;
		}
	}
	else
	{
		// Proportional character width mode
		while (*str)
		{
			utf8 = getUtf8(&str);
			if (utf8 == ' ' && mSpaceWidth > 0)
				width += mSpaceWidth;
			else if(utf8 == ' ')
				width += 4;
			else if(utf8 == '.' && mDotWidth > 0)
				width += mDotWidth;
			else if(utf8 == '.')
				width += 4;
			else
			{
				fontInfo = getFontInfo(utf8);
				if(fontInfo != 0)
				{
					if(fontInfo->xpos == 0)
						width += fontInfo->width + (int8_t)fontInfo->xpos + 1;
					else
						width += fontInfo->width + (int8_t)fontInfo->xpos;
				}
			}
		}
	}

	return width;
}

/**
 * @details Returns the nominal font size (`mSize`) in pixels.
 * @see Font
 * @todo None.
 * @bug None.
 */
uint16_t Font::getStringHeight(void)
{
	return mSize;
}
