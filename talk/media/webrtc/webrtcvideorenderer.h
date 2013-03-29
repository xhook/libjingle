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

#include "modules/video_render/include/video_render.h"

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/media/base/videocapturer.h"

namespace webrtc {

class WebRtcVideoRenderer
	: public VideoRendererInterface
	, public sigslot::has_slots<>
{
public:

  WebRtcVideoRenderer(uint32_t id,
                      void* window,
                      const bool fullscreen,
                      cricket::VideoCapturer * capturer = NULL,
                      bool mirror_x = false,
                      bool mirror_y = false);

  virtual ~WebRtcVideoRenderer();

  void OnFrameCaptured(cricket::VideoCapturer* capturer,
                       const webrtc::I420VideoFrame * frame);

  // Called when the video has changed size.
  virtual void SetSize(int width, int height);

  void RenderFrame(const webrtc::I420VideoFrame * frame);

  bool SetCropping(float left, float right, float bottom, float top);

  bool TakeScreenshotRGB24(const uint8_t ** buffer, uint32_t * width, uint32_t * height);

protected:

  bool RenderFrame();

private:

  uint32_t stream_id_;
  webrtc::I420VideoFrame frame_;
  webrtc::I420VideoFrame frame_copy_;
  webrtc::VideoRender * video_render_;
  webrtc::VideoRenderCallback * video_render_cb_;
  cricket::VideoCapturer * capturer_;
  std::vector<uint8_t> screenshot_;
  talk_base::CriticalSection cs_scr_;
};


}  // namespace webrtc

