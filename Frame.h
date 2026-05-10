/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_FRAME__H_
#define YSS_GUI_FRAME__H_

#include "Container.h"

#if USE_GUI

class OutputFrameBuffer;
class PointerDevice;

class Frame : public Container
{
public :
	Frame(void);

	virtual ~Frame(void);

	void setPosition(Position pos);

	void setPosition(int16_t x, int16_t y);

	void setOutputFrameBuffer(OutputFrameBuffer *outputFrameBuffer);

	void add(Object &obj);

	void add(Object *obj);

	Object *handlerPush(Position pos);

	Object *handlerDrag(Position pos);

	Object *handlerUp(void);

	void setAsSystemFrame(void);

	virtual void update(void);

	void setPointerDevice(PointerDevice &obj);

protected:
	virtual void update(Rectangular rect);

	virtual void update(Rectangular before , Rectangular current);

private :
	OutputFrameBuffer *mOutputFrameBuffer;
	PointerDevice *mPointerDevice;
};

#endif

#endif

