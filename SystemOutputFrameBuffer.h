/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_OUTPUT_FRAME_BUFFER__H_
#define YSS_GUI_OUTPUT_FRAME_BUFFER__H_

#include <config.h>

#if USE_GUI && YSS_L_HEAP_USE

#include "FrameBufferRgb565LE.h"
#include "Rectangular.h"

typedef YSS_SYSTEM_COUTPUT_FRAME_BUFFER SystemOutputFrameBuffer;

class Frame;

class OutputFrameBuffer
{
public :
	OutputFrameBuffer(void);

	~OutputFrameBuffer(void);

	void setFrame(Frame *frame);

	uint8_t* getFrameBuffer(void);

protected:
	virtual void update(Rectangular rect);

	virtual void update(Rectangular before , Rectangular current);

	virtual void update(void);

private :
	Frame *mFrame;
	SystemOutputFrameBuffer *mFrameBuffer;
};

#endif

#endif

