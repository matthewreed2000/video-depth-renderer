#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

#ifdef av_err2str
#undef av_err2str
#include <string>
av_always_inline std::string av_err2string(int errnum) {
    char str[AV_ERROR_MAX_STRING_SIZE];
    return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}
#define av_err2str(err) av_err2string(err).c_str()
#endif  // av_err2str

class VideoReader {
public:
	VideoReader(const char* filename);
	~VideoReader();

	bool readFrame(uint8_t** frameBuffer, int64_t* pts = nullptr);

	inline bool getSuccess() const { return success; };
	inline int getWidth() const { return width; };
	inline int getHeight() const { return height; };
	inline AVRational getTimeBase() const { return timeBase; };
private:
	AVFormatContext* avFormatCtx = nullptr;
	AVCodecContext* avCodecCtx = nullptr;
	AVFrame* avFrame = nullptr;
	AVPacket* avPacket = nullptr;
	SwsContext* swsScalerCtx = nullptr;

	bool success = false;
	int width = 0;
	int height = 0;
	int videoStreamIndex = -1;
	int audioStreamIndex = -1;
	AVRational timeBase;

	bool init(const char* filename);
	void destroy();
};