/*
 * WebcamCaptureFactory.cpp
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <string>
#include <webcam_capture_factory.h>
#include <webcam_capture.h>

ICapture* WebcamCaptureFactory::createFactory(const std::string& connectionString){
	return new WebcamCapture(std::move(connectionString));
}
