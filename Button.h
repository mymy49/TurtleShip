/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Button.h
 * @brief Interactive 3D beveled push-button widget class (`Button`) of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_BUTTON__H_
#define YSS_GUI_BUTTON__H_

#include "Object.h"
#include "Color.h"

class Font;

/**
 * @class Button
 * @brief Interactive push-button widget with pre-rendered 3D beveled pushed/pulled states and event callbacks.
 *
 * @details
 * `Button` extends `Object` to provide a tactile push-button widget. To minimize rendering latency
 * during touch interactions, `Button` maintains two internal off-screen frame buffers (`mPushedFb`
 * and `mPulledFb`) in addition to its composite buffer (`mFrameBuffer`):
 * - **`mPulledFb` (Released state)**: Rendered with a raised 3-pixel 3D bevel highlight on the
 *   top/left edges, shadow on the bottom/right edges, and centered label text.
 * - **`mPushedFb` (Pressed state)**: Rendered with an inset 3-pixel 3D bevel shadow on the top/left
 *   edges, highlight on the bottom/right edges, and label text shifted by `(+2, +2)` pixels.
 *
 * When a user presses or releases the button (`handlerPush()` / `handlerUp()`), `paint()` simply
 * blits the pre-rendered `mPushedFb` or `mPulledFb` bitmap and invokes the registered callback
 * (`mPushHandler` or `mUpHandler`).
 */
class Button : public Object
{
public:
	/**
	 * @brief Constructs a `Button` with default gray background (`0x80, 0x80, 0x80`) and white font color.
	 * @author Yoon-Ki Hong
	 */
	Button(void);

	/**
	 * @brief Virtual destructor. Deletes the internal `mPushedFb` and `mPulledFb` frame buffers.
	 * @author Yoon-Ki Hong
	 */
	virtual ~Button(void);

	/**
	 * @brief Registers a callback function invoked when the button is pressed (`handlerPush()`).
	 * @param[in] handler Function pointer `void (*)(void)` to call on touch-down.
	 * @author Yoon-Ki Hong
	 */
	void setPushEventHandler(void (*handler)(void));

	/**
	 * @brief Registers a callback function invoked when the button is released (`handlerUp()`).
	 * @param[in] handler Function pointer `void (*)(void)` to call on touch-up.
	 * @author Yoon-Ki Hong
	 */
	void setUpEventHandler(void (*handler)(void));

	/**
	 * @brief Sets the button caption text and marks the button image as dirty.
	 * @param[in] text Null-terminated UTF-8 string pointer (must remain valid while used).
	 * @author Yoon-Ki Hong
	 */
	void setText(const char *text);

	/**
	 * @brief Sets the `Font` used to render the button caption on all internal frame buffers.
	 * @param[in] font Reference to the `Font` object.
	 * @author Yoon-Ki Hong
	 */
	void setFont(Font &font);

	/**
	 * @brief Sets the button face background color and marks the button image as dirty.
	 * @param[in] color New button background `Color`.
	 * @author Yoon-Ki Hong
	 */
	void setColor(Color color);

	/**
	 * @brief Resizes the button and reallocates its composite, pushed, and pulled frame buffers.
	 * @param[in] size New button `Size` (width and height in pixels).
	 * @author Yoon-Ki Hong
	 */
	void setSize(Size size);

	/**
	 * @brief Resizes the button from explicit width and height values in pixels.
	 * @param[in] width  New button width in pixels.
	 * @param[in] height New button height in pixels.
	 * @author Yoon-Ki Hong
	 */
	void setSize(uint16_t width, uint16_t height);

	/**
	 * @brief Sets the color of the button caption text and marks the button image as dirty.
	 * @param[in] color New caption `Color`.
	 * @author Yoon-Ki Hong
	 */
	void setFontColor(Color color);

	/**
	 * @brief Regenerates cached button images if modified and blits the active state (`mPushedFb` or `mPulledFb`) to `mFrameBuffer`.
	 * @author Yoon-Ki Hong
	 */
	virtual void paint(void) override;

	/**
	 * @brief Switches the button to the pressed state (`mState = true`), repaints, updates, and invokes `mPushHandler`.
	 * @param[in] pos Touch coordinate relative to the button (unused).
	 * @return `this`.
	 * @author Yoon-Ki Hong
	 */
	virtual Object *handlerPush(Position pos) override;

	/**
	 * @brief Switches the button to the released state (`mState = false`), repaints, updates, and invokes `mUpHandler`.
	 * @return `this`.
	 * @author Yoon-Ki Hong
	 */
	virtual Object *handlerUp(void) override;

private :
	void (*mPushHandler)(void);          ///< Callback function invoked on button press (`nullptr` if unset).
	void (*mUpHandler)(void);            ///< Callback function invoked on button release (`nullptr` if unset).
	bool mState;                         ///< Current button press state (`true` = pushed, `false` = pulled/released).
	bool mButtonModifiedFlag;            ///< `true` when `makeButtonImage()` needs to regenerate `mPushedFb` and `mPulledFb`.
	const char *mText;                   ///< Pointer to the button caption string (`nullptr` if unset).
	Color mFontColor;                    ///< Color used to draw the button caption string.
	GuiFrameBuffer *mPushedFb;           ///< Pre-rendered frame buffer for the pressed (pushed) visual state.
	GuiFrameBuffer *mPulledFb;           ///< Pre-rendered frame buffer for the released (pulled) visual state.
	FrameBufferRgb565LE *mFrameBuffer;   ///< Active composite frame buffer of the button widget.

	/**
	 * @brief Renders the 3D beveled borders and centered caption text into both `mPushedFb` and `mPulledFb`.
	 * @author Yoon-Ki Hong
	 */
	void makeButtonImage(void);
};

#endif
