/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_OBJSYS__H_
#define YSS_GUI_OBJSYS__H_

#include "TurtleShipTypes.h"
#include "Rectangular.h"
#include "config.h"
#include "FrameBufferRgb565LE.h"

#ifndef YSS_GUI_FRAME_BUFFER
#define YSS_GUI_FRAME_BUFFER			FrameBufferRgb565LE
#endif

typedef YSS_GUI_FRAME_BUFFER GuiFrameBuffer;

class Container;

class Object
{
public:
	Object(void);

	virtual ~Object(void);

	void setPosition(Position pos);

	void setPosition(int16_t x, int16_t y);

	void setSize(Size size);

	void setSize(uint16_t width, uint16_t height);

	virtual Size getSize(void) = 0;

	Rectangular getRectangular(void);

	Position getPosition(void);

	Position getAbsolutePos(void);

	virtual Object *handlerPush(Position pos);

	virtual Object *handlerDrag(Position pos);

	virtual Object *handlerUp(void);

	virtual void paint(void) = 0;

	bool isVisible(void);

	void setVisible(bool on);

	void setParent(Container *parent);

	FrameBuffer* getFrameBuffer(void);

	void setFont(Font &font);

	bool isModified(void);

	void clearModifyFlag(void);

protected:
	bool mVisibleFlag;
	bool mResizeAble;
	bool mModifiedFlag;
	Position mPos;
	Container *mParent;

	virtual void eventSizeChanged(Size size);

	virtual void update(Rectangular rect);

	virtual void update(Rectangular before , Rectangular current);

	virtual void update(void);

	void touch(void);
};

#endif

