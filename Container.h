/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef YSS_GUI_CONTAINER__H_
#define YSS_GUI_CONTAINER__H_

#include "Object.h"

#if USE_GUI

class Container : public Object
{
  public:
	Container(void);

	virtual ~Container(void);

	virtual void paint(void);

	void increaseObjArr(void);

	void add(Object &obj);

	void add(Object *obj);

	void update(Rectangular rect);

	void update(Rectangular before, Rectangular current);

	void update(void);

	Object *handlerPush(Position pos);

	Object *handlerDrag(Position pos);

	Object *handlerUp(void);

	void setBackgroundColor(Color color);

  protected:
	uint16_t mNumOfObj, mMaxObj;
	Object **mObjArr, *mLastEventObj;
	bool mValidFlag;
	FrameBufferRgb565LE *mFrameBuffer;
};

#endif

#endif

