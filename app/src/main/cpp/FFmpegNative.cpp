#include <cstdio>
#include <cstring>
#include "jni.h"

//由于 FFmpeg 库是 C 语言实现的，告诉编译器按照 C 的规则进行编译
extern "C" {
#include <libavcodec/version.h>
#include <libavcodec/avcodec.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libavfilter/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>
};

#include "FFMediaPlayer.h"

static jstring getFFmpegPlayerVersion(JNIEnv *env, jclass instance
) {
    char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "libavcodec : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(strBuffer, "\nlibavformat : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(strBuffer, "\nlibavutil : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVUTIL_VERSION));
    strcat(strBuffer, "\nlibavfilter : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFILTER_VERSION));
    strcat(strBuffer, "\nlibswresample : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    strcat(strBuffer, "\nlibswscale : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWSCALE_VERSION));
    strcat(strBuffer, "\navcodec_configure : \n");
    strcat(strBuffer, avcodec_configuration());
    strcat(strBuffer, "\navcodec_license : ");
    strcat(strBuffer, avcodec_license());
    return env->NewStringUTF(strBuffer);
}

static jlong ffmpegPlayerInit(JNIEnv *env, jobject obj, jstring jurl, jobject surface) {
    const char *url = env->GetStringUTFChars(jurl, nullptr);
    auto *player = new FFMediaPlayer();
    player->Init(env, obj, const_cast<char *>(url), surface);
    env->ReleaseStringUTFChars(jurl, url);
    return reinterpret_cast<jlong>(player);
}

static void ffmpegPlayerPlay(JNIEnv *env, jobject obj, jlong player_handle) {
    if (player_handle != 0) {
        auto *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        ffMediaPlayer->Play();
    }
}

static void
ffmpegPlayerSeekToPosition(JNIEnv *env, jobject obj, jlong player_handle, jfloat position) {
    if (player_handle != 0) {
        auto *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        ffMediaPlayer->SeekToPosition(position);
    }
}


static jlong
getFFmpegPlayerMediaParams(JNIEnv *env, jobject thiz, jlong player_handle, jint param_type) {
    long value = 0;
    if (player_handle != 0) {
        auto *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        value = ffMediaPlayer->GetMediaParams(param_type);
    }
    return value;
}

static void ffmpegPlayerPause(JNIEnv *env, jobject obj, jlong player_handle) {
    if (player_handle != 0) {
        auto *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        ffMediaPlayer->Pause();
    }
}

static void ffmpegPlayerStop(JNIEnv *env, jobject obj, jlong player_handle) {
    if (player_handle != 0) {
        auto *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        ffMediaPlayer->Stop();
    }
}

static void ffmpegPlayerUnInit(JNIEnv *env, jobject obj, jlong player_handle) {
    if (player_handle != 0) {
        auto *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        ffMediaPlayer->UnInit();
    }
}

static JNINativeMethod method_table[] = {
        {"nativeGetFFmpegVersion", "()Ljava/lang/String;",                    (void *) getFFmpegPlayerVersion},
        {"nativeInit",             "(Ljava/lang/String;Ljava/lang/Object;)J", (void *) ffmpegPlayerInit},
        {"nativePlay",             "(J)V",                                    (void *) ffmpegPlayerPlay},
        {"nativeSeekToPosition",   "(JF)V",                                   (void *) ffmpegPlayerSeekToPosition},
        {"nativeGetMediaParams",   "(JI)J",                                   (void *) getFFmpegPlayerMediaParams},
        {"nativePause",            "(J)V",                                    (void *) ffmpegPlayerPause},
        {"nativeStop",             "(J)V",                                    (void *) ffmpegPlayerStop},
        {"nativeUnInit",           "(J)V",                                    (void *) ffmpegPlayerUnInit},
};


extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    } else {
        jclass clazz = env->FindClass("com/von/ffmpeg/demo/media/FFmpegPlayer");
        if (clazz) {
            jint ret = env->RegisterNatives(clazz, method_table,
                                            sizeof(method_table) / sizeof(method_table[0]));
            env->DeleteLocalRef(clazz);
            return ret == 0 ? JNI_VERSION_1_6 : JNI_ERR;
        } else {
            return JNI_ERR;
        }
    }
}
