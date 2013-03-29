/*
 * libjingle
 * Copyright 2004--2011, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "common_video/libyuv/include/webrtc_libyuv.h"

#include "webrtcvideorenderer.h"
#include "webrtcvideoframe.h"

#include "talk/media/base/videoframe.h"

namespace webrtc {

WebRtcVideoRenderer::WebRtcVideoRenderer(uint32_t id,
                                         void* window,
                                         const bool fullscreen,
                                         cricket::VideoCapturer * capturer,
                                         bool mirror_x,
                                         bool mirror_y)
    : stream_id_(12345) // NOTE: This value shuold be less than 0xffff. TODO: Change id to something meaningful
    , video_render_(0)
    , video_render_cb_(0)
    , capturer_(capturer)
{
  video_render_ = webrtc::VideoRender::CreateVideoRender(id, window, fullscreen);
  if (video_render_)
  {
    video_render_cb_ = video_render_->AddIncomingRenderStream(stream_id_, 0, 0, 0, 1, 1);
    if (mirror_x || mirror_y)
      video_render_->MirrorRenderStream(stream_id_, true, mirror_x, mirror_y);
    video_render_->StartRender(stream_id_);
  }
  if (capturer_ != NULL)
  {
    capturer_->SignalI420FrameCaptured.connect(this, &WebRtcVideoRenderer::OnFrameCaptured);
  }
}

WebRtcVideoRenderer::~WebRtcVideoRenderer()
{
  if (video_render_)
  {
    video_render_->StopRender(stream_id_);
    video_render_->DeleteIncomingRenderStream(stream_id_);
    webrtc::VideoRender::DestroyVideoRender(video_render_);
    video_render_ = 0;
  }
}

void WebRtcVideoRenderer::OnFrameCaptured(cricket::VideoCapturer* capturer,
                                          const webrtc::I420VideoFrame * frame)
{
  RenderFrame(frame);
}

// Called when the video has changed size.
void WebRtcVideoRenderer::SetSize(int width, int height)
{
  return;
}

void WebRtcVideoRenderer::RenderFrame(const webrtc::I420VideoFrame * frame)
{
  talk_base::CritScope lock(&cs_scr_);
  frame_.CopyFrame(*frame);
  RenderFrame();
}

bool WebRtcVideoRenderer::SetCropping(float left, float right, float bottom, float top)
{
  talk_base::CritScope lock(&cs_scr_);
  return (video_render_->SetStreamCropping(stream_id_, left, top, right, bottom) == 0);
}

bool WebRtcVideoRenderer::TakeScreenshotRGB24(const uint8_t ** buffer, uint32_t * width, uint32_t * height)
{
  talk_base::CritScope lock(&cs_scr_);
  size_t ss_size = frame_.width()*frame_.height()*4;
  if (ss_size == 0) {
    return false;
  }
  screenshot_.resize(ss_size);
  if (screenshot_.size() != ss_size) {
    return false;
  }
  if (ConvertFromI420(frame_, kARGB, 0, &screenshot_[0]) != 0) {
    return false;
  }
  *buffer = &screenshot_.front();
  *width = frame_.width();
  *height = frame_.height();
  return true;
}

bool WebRtcVideoRenderer::RenderFrame()
{
  if (!video_render_cb_) {
    return false;
  }
  if (video_render_cb_->RenderFrame(stream_id_, frame_) != 0) {
    return false;
  }
  return true;
}

}  // namespace webrtc
