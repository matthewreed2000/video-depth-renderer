// Mostly based on this video playlist:
// https://www.youtube.com/playlist?list=PLKucWgGjAuTbobNC28EaR9lbGQTVyD9IS

#include "video_reader.hpp"
#include "files.hpp"

#include <stdio.h>

VideoReader::VideoReader(const char* filename) {
	if (!filename) {
		printf("No video file given\n");
	}
	else if (!files::exists(filename)) {
		printf("Failed to locate video file '%s'\n", filename);
	}
	else {
		printf("Succesfully found video file '%s'\n", filename);
		success = init(filename);

		if (!success) {
			printf("Couldn't open video");
		}
	}
}

VideoReader::~VideoReader() {
	destroy();
}

bool VideoReader::init(const char* filename) {
	// Open file using libavformat
	avFormatCtx = avformat_alloc_context();
	if (!avFormatCtx) {
		printf("Couldn't create AVFormatContext\n");
		destroy();
		return false;
	}

	if (avformat_open_input(&avFormatCtx, filename, NULL, NULL)) {
		printf("Couldn't open video file '%s'\n", filename);
		destroy();
		return false;
	}

	// Find the first valid video stream
	AVCodecParameters* avCodecParams;
	const AVCodec* avCodec;

	bool videoCodecFound = false;
	bool audioCodecFound = false;

	for(int i = 0; i < avFormatCtx->nb_streams; i++) {
		avCodecParams = avFormatCtx->streams[i]->codecpar;
		avCodec = avcodec_find_decoder(avCodecParams->codec_id);

		if (!avCodec) {
			printf("Couldn't find codec for video stream %d, continuing\n", i);
			// avcodec_parameters_free(&avCodecParams);
			continue;
		}

		if ((avCodecParams->codec_type == AVMEDIA_TYPE_VIDEO) && (!videoCodecFound)) {
			videoStreamIndex = i;
			width = avCodecParams->width;
			height = avCodecParams->height;
			timeBase = avFormatCtx->streams[i]->time_base;

			videoCodecFound = true;
			// avcodec_parameters_free(&avCodecParams);
		}

		if ((avCodecParams->codec_type == AVMEDIA_TYPE_AUDIO) && (!audioCodecFound)) {
			audioStreamIndex = i;

			audioCodecFound = true;
		}

		if (videoCodecFound && videoCodecFound)
			break;
	}

	if (!videoCodecFound) {
		printf("Couldn't find valid video stream inside file\n");
		destroy();
		return false;
	}

	// Set up a codec context for the decoder
	avCodecCtx = avcodec_alloc_context3(avCodec);
	if (!avCodecCtx) {
		printf("Couldn't create AVCodecContext\n");
		destroy();
		return false;
	}
	if (avcodec_parameters_to_context(avCodecCtx, avCodecParams) < 0) {
		printf("Couldn't initialize AVCodecContext!\n");
		destroy();
		return false;
	}
	if (avcodec_open2(avCodecCtx, avCodec, NULL) < 0) {
		printf("Couldn't open video codec\n");
		destroy();
		return false;
	}

	avFrame = av_frame_alloc();
	if (!avFrame) {
		printf("Couldn't allocate AVFrame\n");
		destroy();
		return false;
	}
	avPacket = av_packet_alloc();
	if (!avPacket) {
		printf("Couldn't allocate AVPacket");
		destroy();
		return false;
	}

	return true;
}

void VideoReader::destroy() {
	if (avFormatCtx)
	{
		avformat_close_input(&avFormatCtx);
		avformat_free_context(avFormatCtx);
		avFormatCtx = nullptr;
	}

	if (avFrame)
	{
		av_frame_free(&avFrame);
		avFrame = nullptr;
	}

	if (avPacket)
	{
		av_packet_free(&avPacket);
		avPacket = nullptr;
	}

	if (avCodecCtx)
	{
		avcodec_free_context(&avCodecCtx);
		avCodecCtx = nullptr;
	}

	if (swsScalerCtx)
	{
		sws_freeContext(swsScalerCtx);
		swsScalerCtx = nullptr;
	}

	success = false;
	width = 0;
	height = 0;
	videoStreamIndex = -1;
}

bool VideoReader::readFrame(uint8_t** frameBuffer, int64_t* pts) {
	// Give bright magenta if there is an error
	if (!success) {
		width = 1;
		height = 1;

		if (*frameBuffer)
		{
			delete [] *frameBuffer;
			*frameBuffer = nullptr;
		}
		*frameBuffer = new uint8_t[4];

		(*frameBuffer)[0] = 255;
		(*frameBuffer)[1] = 0;
		(*frameBuffer)[2] = 255;
		(*frameBuffer)[3] = 255;

		if (pts)
			*pts = 0;

		return false;
	}

	// Otherwise, read the frame
	int readResponse;
	int response;

	for (readResponse = av_read_frame(avFormatCtx, avPacket); readResponse >= 0; readResponse = av_read_frame(avFormatCtx, avPacket)) {
		if (avPacket->stream_index != videoStreamIndex) {
			av_packet_unref(avPacket);
			continue;
		}

		response = avcodec_send_packet(avCodecCtx, avPacket);
		if (response < 0) {
			printf("Failed to decode packet: %s\n", av_err2str(response));
			return false;
		}

		response = avcodec_receive_frame(avCodecCtx, avFrame);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			av_packet_unref(avPacket);
			continue;
		}
		else if (response < 0) {
			printf("Failed to decode packet: %s\n", av_err2str(response));
			return false;
		}

		av_packet_unref(avPacket);
		break;
	}

	// Repeat when end is reached
	if ((readResponse == AVERROR_EOF) && repeat)
		av_seek_frame(avFormatCtx, videoStreamIndex, 0, 0);

	if (pts)
		*pts = avFrame->pts;

	width = avFrame->width;
	height = avFrame->height;

	// Set up sws scaler
	if (!swsScalerCtx) {

		printf("%dx%d\n", width, height);

		// sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat, int dstW, int dstH, enum AVPixelFormat dstFormat, int flags, SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param)
		swsScalerCtx = sws_getContext(width, height, avCodecCtx->pix_fmt, width, height, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL, NULL, NULL);

		if (!swsScalerCtx) {
			printf("Couldn't initialize SW Scaler\n");
			return false;
		}
	}

	if (*frameBuffer)
	{
		delete [] *frameBuffer;
		*frameBuffer = nullptr;
	}
	*frameBuffer = new uint8_t[width * height * 4];

	uint8_t* dest[4] = { *frameBuffer, NULL, NULL, NULL };
	int destLinesize[4] = { width * 4, 0, 0, 0 };

	// sws_scale(struct SwsContext *c, const uint8_t *const *srcSlice, const int *srcStride, int srcSliceY, int srcSliceH, uint8_t *const *dst, const int *dstStride)
	sws_scale(swsScalerCtx, avFrame->data, avFrame->linesize, 0, avFrame->height, dest, destLinesize);

	return true;
}