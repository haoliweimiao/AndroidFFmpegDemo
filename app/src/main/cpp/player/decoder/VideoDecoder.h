//
// Created by Von on 2020/6/17.
//

#ifndef FFMPEGDEMO_VIDEODECODER_H
#define FFMPEGDEMO_VIDEODECODER_H

extern "C" {
#define __STDC_CONSTANT_MACROS
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
};

#include <render/video/VideoRender.h>
#include "DecoderBase.h"

/**
 * Init -> ;
 * Start -> StartDecodingThread -> InitFFDecoder -> OnDecoderReady -> DecodingLoop(循环 DecodeOnePacket 解析)
 * -> UnInitDecoder -> OnDecoderDone
 * @param url
 */
class VideoDecoder : public DecoderBase {

public:
    VideoDecoder(char *url) {
        int ret = DecoderBase::Init(url, AVMEDIA_TYPE_VIDEO);
        LOGCATV("VideoDecoder constructor ret: %d", ret);
    }

    virtual ~VideoDecoder() {
        DecoderBase::UnInit();
    }

    int GetVideoWidth() {
        return m_VideoWidth;
    }

    int GetVideoHeight() {
        return m_VideoHeight;
    }

    void SetVideoRender(VideoRender *videoRender) {
        m_VideoRender = videoRender;
    }

private:
    virtual void OnDecoderReady();

    virtual void OnDecoderDone();

    virtual void OnFrameAvailable(AVFrame *frame);

    const AVPixelFormat DST_PIXEL_FORMAT = AV_PIX_FMT_RGBA;

    int m_VideoWidth = 0;
    int m_VideoHeight = 0;

    int m_RenderWidth = 0;
    int m_RenderHeight = 0;

    AVFrame *m_RGBAFrame = nullptr;
    uint8_t *m_FrameBuffer = nullptr;

    VideoRender *m_VideoRender = nullptr;

    SwsContext *m_SwsContext = nullptr;

};


#endif //FFMPEGDEMO_VIDEODECODER_H
