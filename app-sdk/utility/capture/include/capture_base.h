/*
 * capture_base.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef ICAPTURE_H_
#define ICAPTURE_H_

#include <macros.h>

typedef void (*CaptureCallback)(void*);

class CaptureSettings;

class CAPTURE_EXPORT ICapture {
public:
	virtual ~ICapture() {
	}
	virtual void init(CaptureSettings* settings) = 0;
	virtual void start(CaptureCallback func) = 0;
	virtual void startAsync(CaptureCallback func) = 0;
	virtual void stop() = 0;
	virtual bool completed() = 0;
	virtual void* getCodecInfo() = 0;
};

#endif /* ICAPTURE_H_ */