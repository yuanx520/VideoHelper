/*
 * @file   st_videohelper.hpp
 * @author SenseTime Group Limited
 * @brief  Video IO helper for SenseTime C API.
 *
 * Copyright (c) 2014-2015, SenseTime Group Limited. All Rights Reserved.
 */

#ifndef __VIDEOHELPER_HPP__
#define __VIDEOHELPER_HPP__

#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include  <iostream>
#include "jpgd.h"
#include <algorithm>
using namespace std;
#ifndef DISABLE_TIMING
#include <ctime>
#include <cstdio>

#ifdef _MSC_VER
#define __TIC__() double __timing_start = clock()
#define __TOC__()                                                 \
	do {                                                      \
		double __timing_end = clock();            \
		fprintf(stdout, "TIME(ms): %lf\n",                \
			(__timing_end - __timing_start)   \
				/ CLOCKS_PER_SEC * 1000);         \
	} while (0)
#else
#include <unistd.h>
#include <sys/time.h>

#define __TIC__()                                    \
	struct timeval __timing_start, __timing_end; \
	gettimeofday(&__timing_start, NULL);

#define __TOC__()                                                        \
	do {                                                             \
		gettimeofday(&__timing_end, NULL);                       \
		double __timing_gap = (__timing_end.tv_sec -     \
					       __timing_start.tv_sec) *  \
					      1000.0 +                     \
				      (__timing_end.tv_usec -    \
					       __timing_start.tv_usec) / \
					      1000.0;                    \
		fprintf(stdout, "TIME(ms): %lf\n", __timing_gap);        \
	} while (0)

#endif

#else
#define __TIC__()
#define __TOC__()
#endif

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#ifndef USE_LAVI
#define USE_LAVI
#endif

#ifdef _MSC_VER
#include <windows.h>
#endif

#ifdef USE_LAVI
extern "C" {
#include "avilib.h"
};
#endif


namespace yy{

class Frame {
	public:
	enum ColorType { Frame_BGR}; //only supply one
	int cols;
	int rows;
	unsigned char *data;
	ColorType type;
};
enum FrameType {Compress,unCompress};
class Video {
	public:
		std::vector<Frame *> data;
		int frame_num;
};
class VideoHelper {
	public:
	virtual Video*LoadAVIVideo(const char *fn) = 0;
	virtual Frame *CreateFrame(int width, int height,Frame::ColorType ctype,FrameType ftype) {
		Frame *img = new Frame();
		img->cols = width;
		img->rows = height;
		img->type = ctype;
		img->data = new unsigned char[width * height * 3];
		return img;
	}
	void FreeFrame(Frame *image) {
		if (!image) return;
		if (image->data) delete[] image->data;
		delete image;
	}
	void FreeVideo(Video *video){
  	for (std::vector<Frame *>::iterator it = (video->data).begin() ; it != (video->data).end(); ++it)
	{
		FreeFrame(*it);
	}
     		video->data.clear();
		delete video;
	}
	virtual ~VideoHelper() {}
};

#ifdef USE_LAVI
class VideoHelperLAVI: public VideoHelper {
	public:
		virtual Video *LoadAVIVideo(const char *fn) {
		long framesize;
		avi_t *avifile;
		int frame;
		int frames;
		int framew = 0;
		int frameh = 0;

		avifile = AVI_open_input_file(fn,1);
		frames = AVI_video_frames(avifile);
		framew = AVI_video_width(avifile);
		frameh = AVI_video_height(avifile);
		Video * video = new Video();
		video->data.clear();
	for(frame =0;frame < frames;frame ++){
		int iskeyframe; //is keyframe
		AVI_set_video_position(avifile,frame);
		size_t len = AVI_frame_size(avifile, frame);
		char * buffer = new char[len];
		AVI_read_frame(avifile, buffer, &iskeyframe);
		Frame *img = CreateFrame(framew,frameh,Frame::Frame_BGR,Compress);
		unsigned char* decom_data;
		//decompress to BGR
		int w,h,channels;
		decom_data = jpgd::decompress_jpeg_image_from_memory((unsigned char*)buffer,len,&w,&h,&channels,3);
		for (int y = 0; y < frameh; y++) {
			for (int x = 0; x < framew; x++) {
				std::swap(decom_data[(y * framew+ x) * 3 + 0] , decom_data[(y * framew+ x) * 3 + 2]);
				img->data[(y * framew+ x) * 3 + 0] = decom_data[(y * framew+ x) * 3 + 0];
				img->data[(y * framew+ x) * 3 + 1] = decom_data[(y * framew+ x) * 3 + 1];
				img->data[(y * framew+ x) * 3 + 2] = decom_data[(y * framew+ x) * 3 + 2];
				}
			}
		free(decom_data);
		delete[] buffer;
		video->data.push_back(img);

	}
		video->frame_num = frames;
		AVI_close(avifile);
	return video;
}
};
#ifndef VideoHelperBackend
#define VideoHelperBackend VideoHelperLAVI
#endif
#endif
}  // namespace yy

#endif /* end of include guard:  __VIDEOHELPER_HPP__*/
