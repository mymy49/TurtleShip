/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <config.h>

#include "Font.h"

Font::Font(void)
{
	mSpaceWidth = 0;
	mCharWidth = 0;
	mDotWidth = 0;
	mSize = 0;
}

uint8_t Font::getSpaceWidth(void)
{
	return mSpaceWidth;
}

void Font::setSpaceWidth(uint8_t width)
{
	mSpaceWidth = width;
}

uint8_t Font::getCharWidth(void)
{
	return mCharWidth;
}

void Font::setCharWidth(uint8_t width)
{
	mCharWidth = width;
}

uint8_t Font::getDotWidth(void)
{
	return mDotWidth;
}

void Font::setDotWidth(uint8_t width)
{
	mDotWidth = width;
}

uint32_t Font::getUtf8(const char **src)
{
	uint32_t utf8;

	if (*(*src) >= 0xe0)
	{
		utf8 = (uint32_t)*(*src)++ << 16;
		utf8 |= (uint32_t)*((*src)++) << 8;
		utf8 |= *(*src)++;
		return utf8; 
	}
	else if (*(*src) >= 0xc0)
	{
		utf8 = (uint32_t)*((*src)++) << 8;
		utf8 |= *(*src)++;
		return utf8; 
	}
	else if (*(*src) < 0x80)
	{
		return *(*src)++;
	}

	(*src)++;
	return 0;
}

uint16_t Font::getStringWidth(const char *str)
{
	uint16_t width = 0, buf = 0;
	uint32_t utf8;
	Font::fontInfo_t *fontInfo;

	if(mCharWidth)
	{
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

uint16_t Font::getStringHeight(void)
{
	return mSize;
}

