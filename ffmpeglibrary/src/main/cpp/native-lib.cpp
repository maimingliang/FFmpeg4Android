#include <jni.h>
#include <string>
#include <android/log.h>



extern "C" {


#define LOG_TAG "FFmpeg"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)



#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include "ffmpeg.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "android/native_window_jni.h"
#include "android/native_window.h"

JNIEXPORT jstring JNICALL
Java_com_maiml_ffmpeglibrary_FFmpeg_avcodecinfo(JNIEnv *env, jobject instance) {

    char info[40000] = {0};
    av_register_all();
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%sdecode:", info);
        } else {
            sprintf(info, "%sencode:", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }
    return env->NewStringUTF(info);


}

extern "C"
jstring
Java_com_maiml_ffmpegdemo2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {


    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}



void my_logcat(void *ptr, int level, const char *fmt, va_list vl) {
    va_list vl2;
    char line[1024];
    static int print_prefix = 1;

    va_copy(vl2, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);


    switch (level) {
        case AV_LOG_VERBOSE:
            LOGV("%s", line);
            break;

        case AV_LOG_INFO:
            LOGI("%s", line);
            break;

        case AV_LOG_DEBUG:
            LOGD("%s", line);
            break;

        case AV_LOG_FATAL:
            LOGF("%s", line);
            break;

        case AV_LOG_WARNING:
            LOGW("%s", line);
            break;

        case AV_LOG_TRACE:
        case AV_LOG_ERROR:
        default:
            LOGE("%s", line);
    }
}


JNIEXPORT jint JNICALL
Java_com_maiml_ffmpeglibrary_FFmpeg_execute(JNIEnv *env, jobject instance, jobjectArray commands) {

    // TODO
    av_log_set_callback(my_logcat);

    int argc = env->GetArrayLength(commands);
    char *argv[argc];
    int i;
    for (i = 0; i < argc; i++) {
        jstring js = (jstring) env->GetObjectArrayElement(commands, i);
        argv[i] = (char *) env->GetStringUTFChars(js, 0);
    }
    return execute(argc, argv);
}

JNIEXPORT jint JNICALL
Java_com_maiml_ffmpeglibrary_FFmpeg_play(JNIEnv *env, jobject instance, jobject surface,jstring path_) {

    // TODO
    const char *file_name = env->GetStringUTFChars(path_, 0);
    LOGE("file name %s\n",file_name);

//     av_register_all();
//
//    AVFormatContext* pFromatCtx = avformat_alloc_context();
//
//    //open video file
//    if(avformat_open_input(&pFromatCtx,file_name,NULL,NULL) != 0){
//        LOGD("Conldn't open file:%s\n",file_name);
//        return -1;
//    }
//
//    //retrieve stream information
//    if(avformat_find_stream_info(pFromatCtx,NULL)<0){
//        LOGD("Couldn't find stream information.");
//        return -1;
//    }
//
//    int videoStream = -1;
//    for(int i = 0; i < pFromatCtx->nb_streams; i++){
//        if(pFromatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStream < 0){
//            videoStream = i;
//        }
//    }
//
//    if(videoStream == -1){
//        LOGD("Didn't find a video stream.");
//        return -1;
//    }
//
//    //找到video stream 然后进行codec  param
//    //find codec Parame videoStream相关的信息
//    AVCodecParameters *pCodecParam = pFromatCtx->streams[videoStream]->codecpar;
//
//    //video 解码器
//
//    AVCodec *pCodec = avcodec_find_decoder(pCodecParam->codec_id);
//
//    if(pCodec == NULL) {
//        LOGD("codec not find");
//        return -1;
//    }
//
//    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
//
//    if(avcodec_open2(pCodecCtx,pCodec,NULL) < 0) {
//        LOGD("codec not open");
//        return -1;
//    }
//
//    //获取native widow
//    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env,surface);
//
//    //获取视频宽高
//    int videoWidth = pCodecParam->width;
//    int videoHeight = pCodecParam->height;
//
//    //设置native widow 的buffer 大小，可自动拉伸
//    ANativeWindow_setBuffersGeometry(nativeWindow,videoWidth,videoHeight,WINDOW_FORMAT_RGBA_8888);
//
//    ANativeWindow_Buffer *window_buffer;
//
//    if(avcodec_open2(pCodecCtx,pCodec,NULL) < 0) {
//        LOGD("codec not open");
//        return -1;
//    }
//
//    //allocate vide frame
//    AVFrame *pFrame = av_frame_alloc();
//    //用于渲染
//    AVFrame *pFrameRGBA = av_frame_alloc();
//    if(pFrame == NULL || pFrameRGBA == NULL){
//        LOGD("Could not allocate video frame.");
//        return  -1;
//    }
//
//
//    //buffer 中的数据用于渲染，格式为RGBA
//    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA,videoWidth,videoHeight,1);
//
//    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
//    av_image_fill_arrays(pFrameRGBA->data,pFrameRGBA->linesize,buffer,AV_PIX_FMT_RGBA,videoWidth,videoHeight,1);
//
//
//    //由于解码出来的帧格式不是RGBA的，在渲染之前需要进行格式转换。
//
//    struct SwsContext *swsContext = sws_getContext(videoWidth,
//                                                   videoHeight,
//                                                   pCodecCtx->pix_fmt,
//                                                   videoWidth,
//                                                   videoHeight,
//                                                   AV_PIX_FMT_RGBA,
//                                                   SWS_BILINEAR,
//                                                   NULL,
//                                                   NULL,
//                                                   NULL);
//
//    int frameFinish;
//    AVPacket packet;
//
//    while (av_read_frame(pFromatCtx,&packet) >= 0) {
//
//        //Is this a  packet from the video steam?
//        if (packet.stream_index == videoStream) {
//
//            //Decode video frame vcodec_send_packet() and avcodec_receive_frame().
////            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
//
//            avcodec_send_packet(pCodecCtx, &packet);
//
//            frameFinish = avcodec_receive_frame(pCodecCtx,pFrame);
//
//            if(frameFinish){
//                //lock native window buffer
//                ANativeWindow_lock(nativeWindow,window_buffer,0);
//
//                //格式转换
//                sws_scale(swsContext, (const uint8_t *const *) pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
//                          pFrameRGBA->data, pFrameRGBA->linesize);
//
//                //获取 stride
//                uint8_t *dst = (uint8_t *) window_buffer->bits;
//                int dstStride = window_buffer->stride * 4;
//                uint8_t *src = pFrameRGBA->data[0];
//                int srcStride = pFrameRGBA->linesize[0];
//
//                //由于window的stride 和帧的stride不同，因此需要逐行复制
//                for (int h = 0; h < videoHeight; h++) {
//                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
//                }
//
//                ANativeWindow_unlockAndPost(nativeWindow);
//
//            }
//
//        }
//
//        av_packet_unref(&packet);
//
//    }
//
//    av_free(buffer);
//    av_free(pFrameRGBA);
//
//    //free the YUV frame
//    av_free(pFrame);
//
//    //close the codec
//    avcodec_close(pCodecCtx);
//
//    avformat_close_input(&pFromatCtx);
//
//    env->ReleaseStringUTFChars(path_, file_name);
//
//    return 0;



    av_register_all();

    AVFormatContext *pFormatCtx = avformat_alloc_context();

    // Open video file
    if (avformat_open_input(&pFormatCtx, file_name, NULL, NULL) != 0) {

        LOGD("Couldn't open file:%s\n", file_name);
        return -1; // Couldn't open file
    }

    // Retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGD("Couldn't find stream information.");
        return -1;
    }

    // Find the first video stream
    int videoStream = -1, i;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO
            && videoStream < 0) {
            videoStream = i;
        }
    }
    if (videoStream == -1) {
        LOGD("Didn't find a video stream.");
        return -1; // Didn't find a video stream
    }

    // Get a pointer to the codec context for the video stream
    AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        LOGD("Codec not found.");
        return -1; // Codec not found
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGD("Could not open codec.");
        return -1; // Could not open codec
    }

    // 获取native window
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);

    // 获取视频宽高
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;

    // 设置native window的buffer大小,可自动拉伸
    ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                     WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGD("Could not open codec.");
        return -1; // Could not open codec
    }

    // Allocate video frame
    AVFrame *pFrame = av_frame_alloc();

    // 用于渲染
    AVFrame *pFrameRGBA = av_frame_alloc();
    if (pFrameRGBA == NULL || pFrame == NULL) {
        LOGD("Could not allocate video frame.");
        return -1;
    }

    // Determine required buffer size and allocate buffer
    // buffer中数据就是用于渲染的,且格式为RGBA
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height,
                                            1);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                         pCodecCtx->width, pCodecCtx->height, 1);

    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,
                                                pCodecCtx->height,
                                                pCodecCtx->pix_fmt,
                                                pCodecCtx->width,
                                                pCodecCtx->height,
                                                AV_PIX_FMT_RGBA,
                                                SWS_BILINEAR,
                                                NULL,
                                                NULL,
                                                NULL);

    int frameFinished;
    AVPacket packet;
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {

            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
//            avcodec_send_packet(pCodecCtx,&packet);
//            frameFinished =avcodec_receive_frame(pCodecCtx,pFrame);

            // 并不是decode一次就可解码出一帧
            if (frameFinished) {

                // lock native window buffer
                ANativeWindow_lock(nativeWindow, &windowBuffer, 0);

                // 格式转换
                sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);

                // 获取stride
                uint8_t *dst = (uint8_t *) windowBuffer.bits;
                int dstStride = windowBuffer.stride * 4;
                uint8_t *src = (pFrameRGBA->data[0]);
                int srcStride = pFrameRGBA->linesize[0];

                // 由于window的stride和帧的stride不同,因此需要逐行复制
                int h;
                for (h = 0; h < videoHeight; h++) {
                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                }

                ANativeWindow_unlockAndPost(nativeWindow);
            }

        }
        av_packet_unref(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGBA);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    env->ReleaseStringUTFChars(path_, file_name);
    return 0;

}






}
