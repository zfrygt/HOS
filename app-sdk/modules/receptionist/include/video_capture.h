#ifndef RECEPTIONIST_VIDEO_CAPTURE_H
#define RECEPTIONIST_VIDEO_CAPTURE_H

#include <string>
#include <capture_base.h>
#include <assert.h>
#include <webcam_capture_factory.h>
#include <video_file_capture_factory.h>
#include <rtsp_capture_factory.h>

const auto MAX_LOG_FILE_SIZE = 1048576 * 5;

template<typename CaptureFactory>
class Capture
{
public:
	static std::unique_ptr<ICapture> get_capture(const std::string& connection_string, std::shared_ptr<spdlog::logger>&& logger)
	{
		CaptureFactory factory;
		return std::unique_ptr<ICapture>(factory.create(std::move(connection_string), std::forward<std::shared_ptr<spdlog::logger>>(logger)));
	}
};

typedef Capture<WebcamCaptureFactory> Webcam;
typedef Capture<VideoFileCaptureFactory> VideoFile;
typedef Capture<RTSPCaptureFactory> RTSP;

template<typename Cap>
class VideoCapture
{
public:
	explicit VideoCapture(const std::string& conn_string)
	{
		m_capture = Cap::get_capture(std::move(conn_string), spdlog::rotating_logger_mt("receptionist_VideoCapture", "logs/receptionist", MAX_LOG_FILE_SIZE, 5));
	}

	void init(CaptureSettings* settings = nullptr)
	{
		assert(m_capture != nullptr);
		m_capture->init(settings);
	}

	void start(CaptureCallback&& capture_callback)
	{
		m_capture->start(std::forward<CaptureCallback>(capture_callback));
	}

	~VideoCapture()
	{
		m_capture->stop();
	}

private:
	std::unique_ptr<ICapture> m_capture;
};

#endif // !RECEPTIONIST_VIDEO_CAPTURE_H