/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Frame.h
 * @brief Top-level full-screen window container (`Frame`) of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_FRAME__H_
#define YSS_GUI_FRAME__H_

#include "Container.h"

#if USE_GUI

class OutputFrameBuffer;
class PointerDevice;

/**
 * @class Frame
 * @brief Root full-screen GUI window that composites child `Object` widgets and flushes updates to the system display.
 *
 * @details
 * `Frame` extends `Container` to represent a top-level screen or page fixed at `(0, 0)` with the
 * dimensions of the system TFT LCD (`system::getSystemTftLcdSize()`).
 *
 * - Its position and size are locked (`mResizeAble = false`, and `setPosition()` is a no-op).
 * - Whenever child widgets are added or invalidated via `update()`, `Frame` composites the dirty
 *   region into `mFrameBuffer` and flushes the updated bitmap to the display via `system::drawBitmap()`.
 * - It can be registered as the active system frame via `setAsSystemFrame()` and linked with a
 *   `PointerDevice` for automatic touch event routing and cleanup on destruction.
 *
 * @note Pending future update: Recent architectural updates (such as migration to Area) have not
 * yet been applied to Frame. Legacy Rectangular dirty-region management is intentionally preserved
 * pending upcoming refactoring.
 */
class Frame : public Container
{
public :
	/**
	 * @brief Constructs a `Frame` sized to the system TFT LCD resolution and clears its frame buffer.
	 * @author Yoon-Ki Hong
	 */
	Frame(void);

	/**
	 * @brief Virtual destructor. Unregisters this `Frame` from `mPointerDevice` if attached.
	 * @author Yoon-Ki Hong
	 */
	virtual ~Frame(void);

	/**
	 * @brief Disabled position setter (`Frame` position is fixed at the screen origin).
	 * @param[in] pos Ignored.
	 * @author Yoon-Ki Hong
	 * @note Intentionally overrides `Object::setPosition()` as a no-op so the root frame cannot be moved.
	 */
	void setPosition(Position pos);

	/**
	 * @brief Disabled position setter (`Frame` position is fixed at the screen origin).
	 * @param[in] x Ignored.
	 * @param[in] y Ignored.
	 * @author Yoon-Ki Hong
	 * @note Intentionally overrides `Object::setPosition()` as a no-op so the root frame cannot be moved.
	 */
	void setPosition(int16_t x, int16_t y);

	/**
	 * @brief Associates an `OutputFrameBuffer` with this frame and triggers a full `update()`.
	 * @param[in] outputFrameBuffer Pointer to the `OutputFrameBuffer`.
	 * @author Yoon-Ki Hong
	 */
	void setOutputFrameBuffer(OutputFrameBuffer *outputFrameBuffer);

	/**
	 * @brief Paints `obj`, adds it to this frame, sets its parent to `this`, and updates its screen region.
	 * @param[in] obj Reference to the child `Object` to add.
	 * @author Yoon-Ki Hong
	 */
	void add(Object &obj);

	/**
	 * @brief Paints `*obj`, adds it to this frame, sets its parent to `this`, and updates its screen region.
	 * @param[in] obj Pointer to the child `Object` to add.
	 * @author Yoon-Ki Hong
	 */
	void add(Object *obj);

	/**
	 * @brief Routes a touch-down event at `pos` to the appropriate child widget via `Container::handlerPush()`.
	 * @param[in] pos Touch coordinate on the frame.
	 * @return Pointer to the child `Object` that handled the push event, or `nullptr`.
	 * @author Yoon-Ki Hong
	 */
	Object *handlerPush(Position pos);

	/**
	 * @brief Routes a touch-drag event at `pos` to the appropriate child widget via `Container::handlerDrag()`.
	 * @param[in] pos Touch coordinate on the frame.
	 * @return Pointer to the child `Object` that handled the drag event, or `nullptr`.
	 * @author Yoon-Ki Hong
	 */
	Object *handlerDrag(Position pos);

	/**
	 * @brief Routes a touch-up event via `Container::handlerUp()`.
	 * @return Pointer to the `Object` that handled the release event, or `nullptr`.
	 * @author Yoon-Ki Hong
	 */
	Object *handlerUp(void);

	/**
	 * @brief Registers this `Frame` as the active system GUI frame via `system::setSystemFrame(this)`.
	 * @author Yoon-Ki Hong
	 */
	void setAsSystemFrame(void);

	/**
	 * @brief Repaints all visible children onto `mFrameBuffer` and flushes the entire frame to the system display.
	 * @author Yoon-Ki Hong
	 */
	virtual void update(void);

	/**
	 * @brief Links a `PointerDevice` to this frame so the pointer device is reset if this frame is destroyed.
	 * @param[in] obj Reference to the `PointerDevice`.
	 * @author Yoon-Ki Hong
	 */
	void setPointerDevice(PointerDevice &obj);

protected:
	/**
	 * @brief Composites visible children within `rect` and flushes only `rect` to the system display.
	 * @param[in] rect Dirty rectangular region to update.
	 * @author Yoon-Ki Hong
	 * @date 2024
	 * @warning Parameter currently uses legacy Rectangular type pending future update.
	 * @note Scheduled to update to Area in an upcoming revision.
	 * @pre Frame buffer must be allocated.
	 * @post Flushes dirty rectangle to system output frame buffer.
	 * @deprecated None.
	 */
	virtual void update(Rectangular rect);

	/**
	 * @brief Composites visible children within `before` and `current` and propagates the update.
	 * @param[in] before  Previous dirty rectangular region.
	 * @param[in] current Current dirty rectangular region.
	 * @author Yoon-Ki Hong
	 * @date 2024
	 * @warning Parameter currently uses legacy Rectangular type pending future update.
	 * @note Scheduled to update to Area in an upcoming revision.
	 * @pre Frame buffer must be allocated.
	 * @post Flushes dirty rectangles to system output frame buffer.
	 * @deprecated None.
	 */
	virtual void update(Rectangular before , Rectangular current);

private :
	OutputFrameBuffer *mOutputFrameBuffer; ///< Optional pointer to the system `OutputFrameBuffer`.
	PointerDevice *mPointerDevice;         ///< Optional pointer to the bound `PointerDevice`.
};

#endif

#endif
