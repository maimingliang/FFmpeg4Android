#include <jni.h>
#include <string>


extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>

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


}
