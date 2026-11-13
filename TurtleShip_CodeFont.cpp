/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file TurtleShip_CodeFont.cpp
 * @brief Implementation of the CodeFont class for Flash-resident bitmap font lookup.
 */

#include <config.h>

#include "CodeFont.h"

/**
 * @details Stores the pointer to the static `codeFontInfo_t` descriptor (`mInfo`), sets the nominal
 * font height `mSize` from `info->size`, and sets the default space width `mSpaceWidth` to one-third
 * of the font height (`mSize / 3`).
 */
CodeFont::CodeFont(const codeFontInfo_t *info)
{
	mInfo = info;
	mSize = info->size;
	mSpaceWidth = mSize / 3;
}

/**
 * @details Selects the glyph group using the lower 3 bits of `ch` (`group = ch & 0x7`), then performs
 * a linear search over the `mInfo->numOfChar[group]` entries in `mInfo->fontInfo[group]` to find an
 * entry whose `utf8Code` equals `ch`. Returns `&info[i]` on match, or `0` (`nullptr`) if not found.
 * @see Font::getFontInfo()
 */
Font::fontInfo_t *CodeFont::getFontInfo(uint32_t ch)
{
	// Select hash bucket using the lowest 3 bits of the UTF-8 code
	uint8_t group = ch & 0x7;
	uint32_t loop = mInfo->numOfChar[group];
	Font::fontInfo_t *info = (Font::fontInfo_t*)mInfo->fontInfo[group];

	// Scan the bucket for the matching UTF-8 character code
	for(uint32_t i = 0; i < loop; i++)
	{
		if(info[i].utf8Code == ch)
			return &info[i];
	}

	return 0;
}
