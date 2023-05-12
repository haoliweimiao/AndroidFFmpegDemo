package com.von.ffmpeg.demo;

public class FFmpegNative {

    static {
        System.loadLibrary("learn-ffmpeg");
    }

    public static native String getFFmpegVersion();
}
