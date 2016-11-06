/*
 * RTSPCaptureFactory.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <rtsp_capture_factory.h>
#include <rtsp_capture.h>

RTSPCaptureFactory::RTSPCaptureFactory() {
	// TODO Auto-generated constructor stub

}

RTSPCaptureFactory::~RTSPCaptureFactory() {
	// TODO Auto-generated destructor stub
}

ICapture* RTSPCaptureFactory::createFactory(const std::string& connectionString){
	return new RTSPCapture(connectionString);
}
