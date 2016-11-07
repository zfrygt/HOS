#ifndef CAPTURE_UTILS_H
#define CAPTURE_UTILS_H

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}

#include <cctype>

inline static bool is_number(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

static inline AVFrame* allocate_frame()
{
#if defined(_WIN32) && defined(_MSC_VER)
	return avcodec_alloc_frame();
#else
	return av_frame_alloc();
#endif
}

static inline void free_frame(AVFrame** frame)
{
#if defined(_WIN32) && defined(_MSC_VER)
	avcodec_free_frame(frame);
#else
	av_frame_free(frame);
#endif
}

static inline void free_packet(AVPacket* packet)
{
#if defined(_WIN32) && defined(_MSC_VER)
	av_free_packet(packet);
#else
	av_packet_unref(packet);
#endif
}

static inline void free_codec_context(AVCodecContext* context)
{
#if defined(_WIN32) && defined(_MSC_VER)
#else
	avcodec_free_context(&context);
#endif
}

static inline AVInputFormat* find_input_format()
{
#if defined(_WIN32) && defined(_MSC_VER)
	return av_find_input_format("vfwcap");
#else
	return av_find_input_format("video4linux2");
#endif
}

#endif