#ifndef TALK_APP_WEBRTC_DEVICEFACTORY_H_
#define TALK_APP_WEBRTC_DEVICEFACTORY_H_

#include "stddef.h"

#include "talk/app/webrtc/config.h"
#include "talk/base/scoped_ref_ptr.h"

#include "modules/audio_device/include/audio_device.h"
#include "modules/video_capture/include/video_capture.h"

namespace webrtc
{

typedef talk_base::scoped_refptr<webrtc::AudioDeviceModule> AudioDeviceModulePtr;
typedef talk_base::scoped_refptr<webrtc::VideoCaptureModule> VideoCaptureModulePtr;

class JINGLE_API DeviceFactory
{
public:
	static void InitForAndroid(void* javaVM, void* env, void* context);

	static AudioDeviceModulePtr CreateAudioDeviceModule(
		int id,
		AudioDeviceModule::AudioLayer audioLayer);

	// Create a video capture module object
	// id - unique identifier of this video capture module object.
	// deviceUniqueIdUTF8 - name of the device.
	//                      Available names can be found by using GetDeviceName
	static VideoCaptureModulePtr CreateVideoCaptureModule(
		const int32_t id,
		const char* deviceUniqueIdUTF8);

	// Create a video capture module object used for external capture.
	// id - unique identifier of this video capture module object
	// externalCapture - [out] interface to call when a new frame is captured.
	static VideoCaptureModulePtr CreateVideoCaptureModule(
		const int32_t id,
		VideoCaptureExternal*& externalCapture);

	static VideoCaptureModule::DeviceInfo* CreateVideoCaptureDeviceInfo(
		const int32_t id);

};

} // namespace webrtc

#endif // TALK_APP_WEBRTC_DEVICEFACTORY_H_

