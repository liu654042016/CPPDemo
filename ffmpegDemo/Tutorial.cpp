//#include "stdafx.h"
//#include <fstream>
//#include <iostream>

//#include "stdafx.h"

#include <fstream>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <tchar.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/file.h>
#include <libavutil/imgutils.h>
#include<libavdevice/avdevice.h>
#include "libswscale/swscale.h"
}


//struct buffer_data {
//	uint8_t* ptr;
//	size_t size; ///< size left in the buffer
//};

//static int read_packet(void* opaque, uint8_t* buf, int buf_size)
//{
//	struct buffer_data* bd = (struct buffer_data*)opaque;
//	buf_size = FFMIN(buf_size, bd->size);
//
//	if (!buf_size)
//		return AVERROR_EOF;
//	printf("ptr:%p size:%zu\n", bd->ptr, bd->size);
//
//	/* copy internal buffer data to buf */
//	memcpy(buf, bd->ptr, buf_size);
//	bd->ptr += buf_size;
//	bd->size -= buf_size;
//
//	return buf_size;
//}



void show_dshow_device() {
	AVFormatContext* pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options, "list_devices", "true", 0);
	const AVInputFormat* iformat = av_find_input_format("dshow");
	printf("========Device Info=============\n");
	avformat_open_input(&pFormatCtx, "video=dummy", iformat, &options);
	printf("================================\n");
}


//将 AVFrame 保存成图片文件  ppm格式
void SaveFrame(AVFrame* pFrame, int width, int height, int iFrame)
{
	//（1）文件名
	char szFileName[32];
	//sprintf(szFileName, "frame%04d.ppm", iFrame);
	sprintf(szFileName, "frame%04d.bmp", iFrame);


	//（2）打开文件
	FILE *pFile;
	pFile = fopen(szFileName, "wb");
	if (pFile == NULL)
	{
		return;
	}

	//（3）写入ppm文件的文件头 
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	//（4）写入ppm文件的内容
	for (int i = 0; i < height; i++)
	{
		fwrite(pFrame->data[0] + i * pFrame->linesize[0], 1, width * 3, pFile);
	}
}


int main(int argc, _TCHAR* argv[])
{
	//文件名字 
	//char filepath[] = "nwn.mp4";
	char filepath[] = "video=OBS Virtual Camera";
//	char filepath[] = "video=HIK 2K Camera";

	//video = OBS Virtual Camera

	//(1)这里注册了所有的文件格式和编解码器的库 所以他们将被自动的使用在被打开的合适格式的文件  只需要注册一次
	//av_register_all();
	avdevice_register_all();
	//avformat_network_init();
	AVFormatContext *pFormatCtx;
	pFormatCtx = avformat_alloc_context();
	show_dshow_device();
	//(2)打开一个文件 打开之后pFormatCtx就有有了文件句柄  这个会打开文件且读取Header
	const AVInputFormat* ifmt = av_find_input_format("dshow");
	//Set own video device's name
	//if(avformat_open_input(&pFormatCtx,"video=Integrated Camera",ifmt,NULL)!=0){
	//if (avformat_open_input(&pFormatCtx, "video=OBS Virtual Camera", ifmt, NULL) != 0) {
	//if (avformat_open_input(&pFormatCtx, "video=OBS Virtual Camera", ifmt, NULL) != 0) {

	//char* buffer;
	//auto err_code;
	int a;
	a = avformat_open_input(&pFormatCtx, filepath, ifmt, NULL);
	if (a!=0 ) 
	{
		//av_strerror();
		std::cout << "failed:" << a << std::endl;
		return -1;
	}

	//(3)检查在文件中的流的信息
	if(avformat_find_stream_info(pFormatCtx,0)<0)
	{
		return -1;
	}

	//(4)dump下信息
	av_dump_format(pFormatCtx,0, filepath,0);


	AVCodecParameters *pCodePar;
	AVStream* stream;
	//AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);//需要使用avcodec_free_context释放
	int videoStream = -1;
	//pFormatCtx->Streams 仅仅是一组pFormatCtx->nb_streams 的指针 包含了哪些流
	for(int i=0; i<pFormatCtx->nb_streams;i++)
	{
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			stream = pFormatCtx->streams[i];
			break;
		}
	}


	if(videoStream == -1)
	{
		return -1; 
	}

	const AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(codec);

	
	pCodePar = pFormatCtx->streams[videoStream]->codecpar;
	//AVCodec *pCodec;
	avcodec_parameters_to_context(pCodecCtx,pCodePar);

	const AVCodec *pCodec = avcodec_find_decoder(pCodePar->codec_id);

	if(pCodec == NULL)
	{
		fprintf(stderr, "Unsupported codec!\n");
		return -1;
	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		return -1;
	}

	AVFrame *pFrame;
	pFrame = av_frame_alloc();
	AVFrame* pFrameRGB = av_frame_alloc();

	cv::Mat mat;
	mat.create(cv::Size(pCodecCtx->width, pCodecCtx->height), CV_8UC3);
	pFrameRGB->data[0] = (uint8_t*)mat.data;


	if(pFrameRGB == NULL)
	{
		return -1;
	}
//	uint8_t *buffer;
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height,1);
	av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, pFrameRGB->data[0], AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
	int frameFinished;
	AVPacket packet;
	while(av_read_frame(pFormatCtx,&packet)>=0)
	{
		if(packet.stream_index == videoStream)
		{
			//把视频帧解压到Frame中
			
			avcodec_send_packet(pCodecCtx, &packet);

			avcodec_receive_frame(pCodecCtx, pFrame);


			//if(frameFinished)
			{
			
				SwsContext* img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
				
				std::cout << "aaaaaa" << std::endl;
				cv::imshow("show", mat);
				int key = cv::waitKey(30);
				if (113 == key) break;
				sws_freeContext(img_convert_ctx);
			}
			//av_free(pFrame);
			//av_free(pFrameRGB);
			av_frame_unref(pFrame);
			av_frame_unref(pFrameRGB);
		//	av_packet_unref(&packet);
			
		}
	   av_packet_unref(&packet);
	}
	av_free(pFrame);
	av_free(pFrameRGB);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	cv::destroyAllWindows();
	return 0;
}
//
//int main(int argc, char* argv[])
//{
//	AVFormatContext* fmt_ctx = NULL;
//	AVIOContext* avio_ctx = NULL;
//	uint8_t* buffer = NULL, * avio_ctx_buffer = NULL;
//	size_t buffer_size, avio_ctx_buffer_size = 4096;
//	char* input_filename = NULL;
//	int ret = 0;
//	struct buffer_data bd = { 0 };
//
//	if (argc != 2) {
//		fprintf(stderr, "usage: %s input_file\n"
//			"API example program to show how to read from a custom buffer "
//			"accessed through AVIOContext.\n", argv[0]);
//		return 1;
//	}
//	input_filename = argv[1];
//
//	/* slurp file content into buffer */
//	ret = av_file_map(input_filename, &buffer, &buffer_size, 0, NULL);
//	if (ret < 0)
//		goto end;
//
//	/* fill opaque structure used by the AVIOContext read callback */
//	bd.ptr = buffer;
//	bd.size = buffer_size;
//
//	if (!(fmt_ctx = avformat_alloc_context())) {
//		ret = AVERROR(ENOMEM);
//		goto end;
//	}
//
//	avio_ctx_buffer = (uint8_t*)av_malloc(avio_ctx_buffer_size);
//	if (!avio_ctx_buffer) {
//		ret = AVERROR(ENOMEM);
//		goto end;
//	}
//	avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size,
//		0, &bd, &read_packet, NULL, NULL);
//	if (!avio_ctx) {
//		ret = AVERROR(ENOMEM);
//		goto end;
//	}
//	fmt_ctx->pb = avio_ctx;
//
//	ret = avformat_open_input(&fmt_ctx, NULL, NULL, NULL);
//	if (ret < 0) {
//		fprintf(stderr, "Could not open input\n");
//		goto end;
//	}
//
//	ret = avformat_find_stream_info(fmt_ctx, NULL);
//	if (ret < 0) {
//		fprintf(stderr, "Could not find stream information\n");
//		goto end;
//	}
//
//	av_dump_format(fmt_ctx, 0, input_filename, 0);
//
//end:
//	avformat_close_input(&fmt_ctx);
//
//	/* note: the internal buffer could have changed, and be != avio_ctx_buffer */
//	if (avio_ctx)
//		av_freep(&avio_ctx->buffer);
//	avio_context_free(&avio_ctx);
//
//	av_file_unmap(buffer, buffer_size);
//
//	if (ret < 0) {
//		//fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
//		return 1;
//	}
//
//	return 0;
//}