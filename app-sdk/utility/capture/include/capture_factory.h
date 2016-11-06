/*
 * ICaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTUREFACTORY_H_
#define ICAPTUREFACTORY_H_

#include <string>
#include <macros.h>

class ICapture;

class CAPTURE_EXPORT ICaptureFactory {
public:
	virtual ~ICaptureFactory() { }
	virtual ICapture* createFactory(const std::string& connectionString) = 0;
};

#endif /* ICAPTUREFACTORY_H_ */
