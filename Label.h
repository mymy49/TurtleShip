/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Label.h
 * @brief Static text label widget class (`Label`) of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#ifndef YSS_GUI_LABEL__H_
#define YSS_GUI_LABEL__H_

#include "Object.h"

class Color;
class Font;

/**
 * @class Label
 * @brief GUI widget that renders a centered single-line UTF-8 text string on a solid background.
 *
 * @details
 * `Label` extends `Object` to display a text caption (`mText`) horizontally and vertically centered
 * (`Brush::ALIGN_CENTER_MID`) inside its `FrameBufferRgb565LE` canvas (`mFrameBuffer`).
 * Whenever the text string, font color, or background color is updated, `Label` marks itself dirty
 * via `touch()` so its parent `Container` or `Frame` repaints it on the next update cycle.
 */
class Label : public Object
{
public:
	/**
	 * @brief Constructs a `Label` with black text (`0x00, 0x00, 0x00`) on a white background (`0xFF, 0xFF, 0xFF`).
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Label(void);

	/**
	 * @brief Clears the label's frame buffer and draws `mText` centered using the configured font.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void paint(void);

	/**
	 * @brief Sets the null-terminated UTF-8 string to display and marks the label as modified.
	 * @param[in] text Pointer to the null-terminated text string.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void setText(const char *text);

	/**
	 * @brief Sets the text foreground color from a `Color` object and marks the label as modified.
	 * @param[in] color Text `Color`.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void setFontColor(Color color);

	/**
	 * @brief Sets the text foreground color from 8-bit RGB components and marks the label as modified.
	 * @param[in] red   Red intensity (`0` ~ `255`).
	 * @param[in] green Green intensity (`0` ~ `255`).
	 * @param[in] blue  Blue intensity (`0` ~ `255`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void setFontColor(uint8_t red, uint8_t green, uint8_t blue);

	/**
	 * @brief Sets the background fill color of the label from a `Color` object and marks the label as modified.
	 * @param[in] color Background `Color`.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void setBackgroundColor(Color color);
	
	/**
	 * @brief Sets the background fill color of the label from 8-bit RGB components and marks the label as modified.
	 * @param[in] red   Red intensity of the background color (`0` ~ `255`).
	 * @param[in] green Green intensity of the background color (`0` ~ `255`).
	 * @param[in] blue  Blue intensity of the background color (`0` ~ `255`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);

private :
	const char *mText;                   ///< Pointer to the null-terminated caption string (`nullptr` if unset).
	FrameBufferRgb565LE *mFrameBuffer;   ///< Off-screen frame buffer owned by this label widget.
};

#endif
