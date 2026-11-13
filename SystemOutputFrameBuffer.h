/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file SystemOutputFrameBuffer.h
 * @brief System output frame buffer bridge (`OutputFrameBuffer`) for the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 */

#ifndef YSS_GUI_OUTPUT_FRAME_BUFFER__H_
#define YSS_GUI_OUTPUT_FRAME_BUFFER__H_

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include "FrameBufferRgb565LE.h"
#include "Area.h"

/**
 * @brief Type alias for the system-configured output frame buffer type.
 */
typedef YSS_SYSTEM_COUTPUT_FRAME_BUFFER SystemOutputFrameBuffer;

class Frame;

/**
 * @class OutputFrameBuffer
 * @brief Adapter class bridging a top-level `Frame` and the system display output frame buffer.
 *
 * @details
 * Holds a reference to the active `Frame` (`mFrame`) and the underlying `SystemOutputFrameBuffer`
 * (`mFrameBuffer`) and provides virtual `update()` hooks for partial and full-screen refreshes.
 */
class OutputFrameBuffer
{
public :
	/**
	 * @brief Constructs an `OutputFrameBuffer` instance.
	 * @author Yoon-Ki Hong
	 */
	OutputFrameBuffer(void);

	/**
	 * @brief Destructor.
	 * @author Yoon-Ki Hong
	 */
	~OutputFrameBuffer(void);

	/**
	 * @brief Binds the active top-level `Frame` to this output frame buffer.
	 * @param[in] frame Pointer to the `Frame` instance.
	 * @author Yoon-Ki Hong
	 */
	void setFrame(Frame *frame);

	/**
	 * @brief Returns a raw byte pointer to the underlying system output frame buffer memory.
	 * @return Pointer to the first byte of the output frame buffer.
	 * @author Yoon-Ki Hong
	 */
	uint8_t* getFrameBuffer(void);

protected:
	/**
	 * @brief Updates a single dirty rectangular region on the output frame buffer.
	 * @param[in] rect Dirty rectangular area to redraw.
	 * @author Yoon-Ki Hong
	 */
	virtual void update(Rectangular rect);

	/**
	 * @brief Updates both the previous (`before`) and new (`current`) dirty rectangular regions.
	 * @param[in] before  Previous dirty rectangular area.
	 * @param[in] current Current dirty rectangular area.
	 * @author Yoon-Ki Hong
	 */
	virtual void update(Rectangular before , Rectangular current);

	/**
	 * @brief Updates the entire output frame buffer from `mFrame`.
	 * @author Yoon-Ki Hong
	 */
	virtual void update(void);

private :
	Frame *mFrame;                          ///< Pointer to the attached top-level `Frame`.
	SystemOutputFrameBuffer *mFrameBuffer;  ///< Pointer to the system output frame buffer instance.
};

#endif

#endif
