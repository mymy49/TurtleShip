/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Container.h
 * @brief Composite GUI widget class (`Container`) that manages and renders child `Object` instances.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#ifndef YSS_GUI_CONTAINER__H_
#define YSS_GUI_CONTAINER__H_

#include "Object.h"

#if USE_GUI

/**
 * @class Container
 * @brief Composite `Object` capable of holding, painting, updating, and routing touch events to child `Object` widgets.
 *
 * @details
 * `Container` maintains a dynamically grown array (`mObjArr`) of child `Object*` pointers and its own
 * `FrameBufferRgb565LE` canvas (`mFrameBuffer`).
 *
 * - **Rendering & Partial Updates**: When `update(rect)` or `update(before, current)` is called,
 *   `Container` clears the dirty region(s) in `mFrameBuffer`, calls `paint()` on any child whose
 *   `isModified()` flag is set, and blits each visible child's bitmap clipped to the dirty region(s).
 *   It then propagates the translated dirty region up to its own parent container (`mParent`).
 * - **Hit Testing & Event Routing**: `handlerPush()` and `handlerDrag()` iterate through `mObjArr`
 *   in reverse Z-order (top-most child first), test whether the touch position falls inside a visible
 *   child's bounding box, translate the coordinate into child-local space, and forward the event.
 *
 * @note Pending future update: Recent architectural updates to Object (such as migration to Area)
 * have not yet been synchronized into Container. Legacy Rectangular signatures are intentionally
 * preserved pending future updates.
 */
class Container : public Object
{
  public:
	/**
	 * @brief Constructs an empty `Container` and allocates the initial child pointer array.
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
	Container(void);

	/**
	 * @brief Virtual destructor. Deletes all child objects and frees the child pointer array.
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
	virtual ~Container(void);

	/**
	 * @brief Composites all visible child objects onto this container's `mFrameBuffer` in Z-order.
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
	virtual void paint(void);

	/**
	 * @brief Expands the capacity of the child pointer array (`mObjArr`) by 512 slots using `lmalloc()`.
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
	void increaseObjArr(void);

	/**
	 * @brief Adds a child `Object` (by reference) to this container, sets its parent to `this`, and updates its area.
	 * @param[in] obj Reference to the child `Object` to add.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void add(Object &obj);

	/**
	 * @brief Adds a child `Object` (by pointer) to this container.
	 * @param[in] obj Pointer to the child `Object` to add.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void add(Object *obj);

	/**
	 * @brief Repaints all visible children intersecting `rect` and propagates the dirty area to `mParent`.
	 * @param[in] rect Dirty rectangular area in container-local coordinates.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2024
	 * @warning Parameter currently uses legacy Rectangular type pending future update.
	 * @note Scheduled to update to Area to align with Object::update(Area).
	 * @pre Container must have an initialized frame buffer.
	 * @post Intersecting modified children are repainted.
	 * @deprecated None.
	 */
	void update(Rectangular rect);

	/**
	 * @brief Repaints all visible children intersecting `before` and `current` and propagates both areas to `mParent`.
	 * @param[in] before  Previous dirty rectangular area in container-local coordinates.
	 * @param[in] current Current dirty rectangular area in container-local coordinates.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2024
	 * @warning Parameter currently uses legacy Rectangular type pending future update.
	 * @note Scheduled to update to Area to align with Object::update(Area, Area).
	 * @pre Container must have an initialized frame buffer.
	 * @post Intersecting modified children in both areas are repainted.
	 * @deprecated None.
	 */
	void update(Rectangular before, Rectangular current);

	/**
	 * @brief Repaints the entire container area and propagates the update to `mParent`.
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
	void update(void);

	/**
	 * @brief Hit-tests visible children from top to bottom and dispatches a push event to the first child containing `pos`.
	 * @param[in] pos Touch coordinate in container-local space.
	 * @return Pointer to the child `Object` that handled the push event, or `nullptr` if no child was hit.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Object *handlerPush(Position pos);

	/**
	 * @brief Hit-tests visible children from top to bottom and dispatches a drag event to the first child containing `pos`.
	 * @param[in] pos Touch coordinate in container-local space.
	 * @return Pointer to the child `Object` that handled the drag event, or `nullptr` if no child was hit.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Object *handlerDrag(Position pos);

	/**
	 * @brief Handles a touch-up event on the container.
	 * @param[in] none
	 * @return `nullptr`.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Object *handlerUp(void);

	/**
	 * @brief Sets the background fill color of the container's `mFrameBuffer`.
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

  protected:
	uint16_t mNumOfObj;                  ///< Current number of child objects stored in `mObjArr`.
	uint16_t mMaxObj;                    ///< Allocated slot capacity of `mObjArr`.
	Object **mObjArr;                    ///< Dynamically allocated array of child `Object*` pointers.
	Object *mLastEventObj;               ///< Pointer to the child `Object` that last received an input event.
	bool mValidFlag;                     ///< `true` while the container is valid and not yet destroyed.
	FrameBufferRgb565LE *mFrameBuffer;   ///< Off-screen frame buffer owned by this container.
};

#endif

#endif
