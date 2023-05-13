package com.von.ffmpeg.demo;

import android.view.Surface;

public class FFmpegPlayer {

    static {
        System.loadLibrary("learn-ffmpeg");
    }

    public static final int MSG_DECODER_INIT_ERROR = 0;
    public static final int MSG_DECODER_READY = 1;
    public static final int MSG_DECODER_DONE = 2;
    public static final int MSG_REQUEST_RENDER = 3;
    public static final int MSG_DECODING_TIME = 4;

    public static final int MEDIA_PARAM_VIDEO_WIDTH = 0x0001;
    public static final int MEDIA_PARAM_VIDEO_HEIGHT = 0x0002;
    public static final int MEDIA_PARAM_VIDEO_DURATION = 0x0003;

    private long mNativePlayerHandle = 0;

    private EventCallback mEventCallback = null;

    public static String getFFmpegVersion() {
        return nativeGetFFmpegVersion();
    }

    public void init(String url, Surface surface) {
        mNativePlayerHandle = nativeInit(url, surface);
    }

    public void play() {
        nativePlay(mNativePlayerHandle);
    }

    public void pause() {
        nativePause(mNativePlayerHandle);
    }

    public void seekToPosition(float position) {
        nativeSeekToPosition(mNativePlayerHandle, position);
    }

    public void stop() {
        nativeStop(mNativePlayerHandle);
    }

    public void unInit() {
        nativeUnInit(mNativePlayerHandle);
    }

    public void addEventCallback(EventCallback callback) {
        mEventCallback = callback;
    }

    public long getMediaParams(int paramType) {
        return nativeGetMediaParams(mNativePlayerHandle, paramType);
    }

    private void playerEventCallback(int msgType, float msgValue) {
        if (mEventCallback != null)
            mEventCallback.onPlayerEvent(msgType, msgValue);

    }

    private static native String nativeGetFFmpegVersion();

    private native long nativeInit(String url, Object surface);

    private native void nativePlay(long playerHandle);

    private native void nativeSeekToPosition(long playerHandle, float position);

    private native void nativePause(long playerHandle);

    private native void nativeStop(long playerHandle);

    private native void nativeUnInit(long playerHandle);

    private native long nativeGetMediaParams(long playerHandle, int paramType);

    public interface EventCallback {
        void onPlayerEvent(int msgType, float msgValue);
    }
}
