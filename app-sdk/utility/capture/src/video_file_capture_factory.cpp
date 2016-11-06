/*
 * VideoFileCaptureFactory.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <video_file_capture_factory.h>
#include <capture_base.h>
#include <string>
#include <video_file_capture.h>

VideoFileCaptureFactory::VideoFileCaptureFactory() {
}

VideoFileCaptureFactory::~VideoFileCaptureFactory() {

}

ICapture* VideoFileCaptureFactory::createFactory(const std::string& connectionString){
	return new VideoFileCapture(connectionString);
}
