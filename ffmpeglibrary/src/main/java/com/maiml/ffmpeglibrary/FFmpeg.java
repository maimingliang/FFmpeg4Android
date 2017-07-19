package com.maiml.ffmpeglibrary;

/**
 * Created by maimingliang on 2017/7/18.
 */

public class FFmpeg {


    private static FFmpeg sInstance ;

    public static FFmpeg getsInstance(){
        if(sInstance == null) {
            sInstance = new FFmpeg();
        }
        return sInstance;
    }

    private FFmpeg(){}


    public String getValue(){
        return avcodecinfo();
    }

    /**
     * 执行ffmpeg命令
     * @param commands
     * @return
     */
    public int executeCommand(String[] commands){
        return execute(commands);
    }



    private native String avcodecinfo();
    private native int execute(String[] commands);

    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("swscale-4");
        System.loadLibrary("avformat-57");

    }

}
