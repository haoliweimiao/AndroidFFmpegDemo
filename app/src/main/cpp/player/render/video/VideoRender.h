//
// Created by Von on 2023/5/13.
//

#ifndef FFMPEGDEMO_VIDEORENDER_H
#define FFMPEGDEMO_VIDEORENDER_H

#include "ImageDef.h"

class VideoRender {
public:
    virtual ~VideoRender() {}

    virtual void Init(int videoWidth, int videoHeight, int *dstSize) = 0;

    virtual void RenderVideoFrame(NativeImage *pImage) = 0;

    virtual void UnInit() = 0;
};

#endif //FFMPEGDEMO_VIDEORENDER_H
