/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Object.h
 * @brief Abstract base class (`Object`) for all hierarchical GUI widgets in the TurtleShip library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_OBJSYS__H_
#define YSS_GUI_OBJSYS__H_

#include "TurtleShipTypes.h"
#include <yss/Area.h>
#include "config.h"
#include "FrameBufferRgb565LE.h"

#ifndef YSS_GUI_FRAME_BUFFER
/**
 * @brief Default off-screen frame buffer class used by GUI `Object` widgets.
 */
#define YSS_GUI_FRAME_BUFFER			FrameBufferRgb565LE
#endif

/**
 * @brief Type alias for the configured GUI widget frame buffer class.
 */
typedef YSS_GUI_FRAME_BUFFER GuiFrameBuffer;

class Container;

/**
 * @class Object
 * @brief Abstract base class representing a positionable, sizable, and touch-interactive GUI widget.
 *
 * @details
 * `Object` forms the root of the retained-mode widget hierarchy in TurtleShip (`Container`, `Frame`,
 * `Button`, `Label`, etc.). Each `Object` maintains:
 * - Its position (`mPos`) relative to its parent `Container` (`mParent`),
 * - Visibility (`mVisibleFlag`), resizeability (`mResizeAble`), and dirty state (`mModifiedFlag`),
 * - Touch/pointer event callbacks (`handlerPush()`, `handlerDrag()`, `handlerUp()`),
 * - Partial invalidation propagation (`update()`) up the parent container hierarchy.
 *
 * Derived widgets implement `paint()` to draw their visual representation into their internal
 * `FrameBuffer` and call `touch()` whenever their visual properties change so the parent `Container`
 * repaints them on the next `update()`.
 *
 * @note Pending future update: Recent architectural updates (such as migrating dirty-region types
 * from legacy `Rectangular` to `Area`) are declared in this interface, while downstream implementations
 * and derived classes (Container, Frame) currently retain legacy signatures. Synchronization is planned
 * for upcoming revisions.
 */
class Object
{
public:
	/**
	 * @brief Constructs an `Object` with default flags (`mVisibleFlag = true`, `mResizeAble = true`, `mModifiedFlag = false`).
	 * @author Yoon-Ki Hong
	 */
	Object(void);

	/**
	 * @brief Virtual destructor.
	 * @author Yoon-Ki Hong
	 */
	virtual ~Object(void);

	/**
	 * @brief Sets the widget's position relative to its parent `Container` and triggers a dirty-region update.
	 * @param[in] pos New top-left `Position` relative to the parent container.
	 * @author Yoon-Ki Hong
	 */
	void setPosition(Position pos);

	/**
	 * @brief Sets the widget's position from explicit `(x, y)` coordinates and triggers a dirty-region update.
	 * @param[in] x New X coordinate relative to the parent container.
	 * @param[in] y New Y coordinate relative to the parent container.
	 * @author Yoon-Ki Hong
	 */
	void setPosition(int16_t x, int16_t y);

	/**
	 * @brief Resizes the widget's frame buffer, invokes `eventSizeChanged()` and `paint()`, and updates the parent.
	 * @param[in] size New widget `Size` (width and height in pixels).
	 * @author Yoon-Ki Hong
	 * @note Has no effect if `mResizeAble` is `false`.
	 */
	void setSize(Size size);

	/**
	 * @brief Resizes the widget from explicit width and height values in pixels.
	 * @param[in] width  New widget width in pixels.
	 * @param[in] height New widget height in pixels.
	 * @author Yoon-Ki Hong
	 * @note Has no effect if `mResizeAble` is `false`.
	 */
	void setSize(uint16_t width, uint16_t height);

	/**
	 * @brief Returns the current dimensions of the widget.
	 * @return Widget `Size` in pixels.
	 * @author Yoon-Ki Hong
	 */
	virtual Size getSize(void) = 0;

	/**
	 * @brief Returns the bounding `Area` (position and size) of the widget within its parent container.
	 * @return Bounding `Area` of the widget.
	 * @author Yoon-Ki Hong
	 */
	Area getRectangular(void);

	/**
	 * @brief Gets the widget's top-left position relative to its parent container.
	 * @return Relative `Position` of the widget.
	 * @author Yoon-Ki Hong
	 */
	Position getPosition(void);

	/**
	 * @brief Computes the absolute screen position of the widget by traversing its parent hierarchy.
	 * @return Absolute `Position` on the root frame.
	 * @author Yoon-Ki Hong
	 */
	Position getAbsolutePos(void);

	/**
	 * @brief Handles a touch-down (push) event at the given widget-local coordinate.
	 * @param[in] pos Touch coordinate relative to this widget's top-left corner.
	 * @return Pointer to the `Object` that handled the push event (`this` by default).
	 * @author Yoon-Ki Hong
	 */
	virtual Object *handlerPush(Position pos);

	/**
	 * @brief Handles a touch-drag event at the given widget-local coordinate.
	 * @param[in] pos Touch coordinate relative to this widget's top-left corner.
	 * @return Pointer to the `Object` that handled the drag event (`this` by default).
	 * @author Yoon-Ki Hong
	 */
	virtual Object *handlerDrag(Position pos);

	/**
	 * @brief Handles a touch-up (release) event on this widget.
	 * @return Pointer to the `Object` that handled the release event (`this` by default).
	 * @author Yoon-Ki Hong
	 */
	virtual Object *handlerUp(void);

	/**
	 * @brief Renders the widget's visual appearance into its internal `FrameBuffer`.
	 * @author Yoon-Ki Hong
	 */
	virtual void paint(void) = 0;

	/**
	 * @brief Checks whether the widget is currently visible.
	 * @return `true` if visible; `false` if hidden.
	 * @author Yoon-Ki Hong
	 */
	bool isVisible(void);

	/**
	 * @brief Sets the visibility of the widget and notifies the parent container to repaint its area.
	 * @param[in] on `true` to show the widget; `false` to hide it.
	 * @author Yoon-Ki Hong
	 */
	void setVisible(bool on);

	/**
	 * @brief Sets the parent `Container` that owns and composites this widget.
	 * @param[in] parent Pointer to the parent `Container`.
	 * @author Yoon-Ki Hong
	 */
	void setParent(Container *parent);

	/**
	 * @brief Returns a pointer to the widget's internal `FrameBuffer`.
	 * @return Pointer to the widget's `FrameBuffer`.
	 * @author Yoon-Ki Hong
	 */
	FrameBuffer* getFrameBuffer(void);

	/**
	 * @brief Sets the `Font` on the widget's internal `FrameBuffer`.
	 * @param[in] font Reference to the `Font` to use.
	 * @author Yoon-Ki Hong
	 */
	void setFont(Font &font);

	/**
	 * @brief Checks whether the widget is marked as modified (dirty) and needs `paint()` before compositing.
	 * @return `true` if modified since the last `clearModifyFlag()`; `false` otherwise.
	 * @author Yoon-Ki Hong
	 */
	bool isModified(void);

	/**
	 * @brief Clears the modified (dirty) flag after the widget has been repainted.
	 * @author Yoon-Ki Hong
	 */
	void clearModifyFlag(void);

protected:
	bool mVisibleFlag;  ///< `true` when the widget should be drawn by its parent container.
	bool mResizeAble;   ///< `true` if `setSize()` is allowed to resize the widget's frame buffer.
	bool mModifiedFlag; ///< `true` when visual properties have changed and `paint()` must be called.
	Position mPos;      ///< Top-left position of the widget relative to `mParent`.
	Container *mParent; ///< Pointer to the parent `Container` (`nullptr` if unattached).

	/**
	 * @brief Hook invoked by `setSize()` after the frame buffer has been resized and before `paint()`.
	 * @param[in] size New dimensions of the widget.
	 * @author Yoon-Ki Hong
	 */
	virtual void eventSizeChanged(Size size);

	/**
	 * @brief Requests the parent container to repaint the specified dirty rectangle `rect`.
	 * @param[in] rect Dirty `Area` in parent coordinates.
	 * @author Yoon-Ki Hong
	 * @date 2024
	 * @warning Implementation signature in TurtleShip_Object.cpp currently uses legacy Rectangular type.
	 * @note Refactoring to Area across all derived classes is pending in a future update.
	 * @pre Widget should be added to a parent Container.
	 * @post Parent container marks the region as dirty.
	 * @deprecated None.
	 */
	virtual void update(Area rect);

	/**
	 * @brief Requests the parent container to repaint both the previous (`before`) and new (`current`) areas.
	 * @param[in] before  Previous bounding `Area` before movement/resizing.
	 * @param[in] current New bounding `Area` after movement/resizing.
	 * @author Yoon-Ki Hong
	 * @date 2024
	 * @warning Implementation signature in TurtleShip_Object.cpp currently uses legacy Rectangular type.
	 * @note Refactoring to Area across all derived classes is pending in a future update.
	 * @pre Widget should be added to a parent Container.
	 * @post Parent container marks both regions as dirty.
	 * @deprecated None.
	 */
	virtual void update(Area before , Area current);

	/**
	 * @brief Requests the parent container to repaint this widget's entire bounding rectangle.
	 * @author Yoon-Ki Hong
	 */
	virtual void update(void);

	/**
	 * @brief Marks the widget as modified (`mModifiedFlag = true`) so it will be repainted on the next `update()`.
	 * @author Yoon-Ki Hong
	 */
	void touch(void);
};

#endif
