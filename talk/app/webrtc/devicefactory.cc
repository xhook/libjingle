#include "talk/app/webrtc/devicefactory.h"

#include "modules/video_capture/include/video_capture_factory.h"
#include "modules/audio_device/include/audio_device.h"
#include "voice_engine/include/voe_base.h"

namespace webrtc
{

void DeviceFactory::InitForAndroid(void* javaVM, void* env, void* context)
{
	webrtc::VoiceEngine::SetAndroidObjects(javaVM, env, context);
}

AudioDeviceModulePtr DeviceFactory::CreateAudioDeviceModule(
	int id,
	AudioDeviceModule::AudioLayer audioLayer)
{
	return webrtc::CreateAudioDeviceModule(id, audioLayer);
}

VideoCaptureModulePtr DeviceFactory::CreateVideoCaptureModule(const WebRtc_Word32 id,
									    const char* deviceUniqueIdUTF8)
{
	return VideoCaptureFactory::Create(id, deviceUniqueIdUTF8);
}

VideoCaptureModulePtr DeviceFactory::CreateVideoCaptureModule(const WebRtc_Word32 id,
									    VideoCaptureExternal*& externalCapture)
{
	return VideoCaptureFactory::Create(id, externalCapture);
}

VideoCaptureModule::DeviceInfo* DeviceFactory::CreateVideoCaptureDeviceInfo(
		const WebRtc_Word32 id)
{
	return VideoCaptureFactory::CreateDeviceInfo(id);
}

} // namespace webrtc

