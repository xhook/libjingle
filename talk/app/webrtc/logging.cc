#include <cassert>

#include <talk/base/logging.h>
#include <talk/base/stream.h>

#include "logging.h"

namespace webrtc {

static talk_base::StreamResult ConvertResult(LoggingStreamInterafce::Result res)
{
	talk_base::StreamResult ret;
	switch(res)
	{
	case LoggingStreamInterafce::RESULT_ERROR:
		ret = talk_base::SR_ERROR;
		break;
	case LoggingStreamInterafce::RESULT_SUCCESS:
		ret = talk_base::SR_SUCCESS;
		break;
	case LoggingStreamInterafce::RESULT_BLOCK:
		ret = talk_base::SR_BLOCK;
		break;
	case LoggingStreamInterafce::RESULT_EOS:
		ret = talk_base::SR_EOS;
		break;
	default:
		assert(false);
		break;
	}
	return ret;
}

static talk_base::StreamState ConvertState(LoggingStreamInterafce::State state)
{
	talk_base::StreamState ret;
	switch(state)
	{
	case LoggingStreamInterafce::STATE_CLOSED:
		ret = talk_base::SS_CLOSED;
		break;
	case LoggingStreamInterafce::STATE_OPENING:
		ret = talk_base::SS_OPENING;
		break;
	case LoggingStreamInterafce::STATE_OPEN:
		ret = talk_base::SS_OPEN;
		break;
	default:
		assert(false);
		break;
	}
	return ret;
}

static talk_base::LoggingSeverity ConvertSeverity(LoggingSeverity sev)
{
	talk_base::LoggingSeverity ret;
	switch(sev)
	{
	case LOGGING_SEVERITY_SENSITIVE:
		ret = talk_base::LS_SENSITIVE;
		break;
	case LOGGING_SEVERITY_VERBOSE:
		ret = talk_base::LS_VERBOSE;
		break;
	case LOGGING_SEVERITY_INFO:
		ret = talk_base::LS_INFO;
		break;
	case LOGGING_SEVERITY_WARNING:
		ret = talk_base::LS_WARNING;
		break;
	case LOGGING_SEVERITY_ERROR:
		ret = talk_base::LS_ERROR;
		break;
	default:
		assert(false);
		break;
	}
	return ret;
}

class LoggingStreamImpl : public talk_base::StreamInterface
{
public:

	LoggingStreamImpl(LoggingStreamInterafce* loggingStream)
		: loggingStream_(loggingStream)
	{
	}

	virtual ~LoggingStreamImpl()
	{
		loggingStream_->Destroy();
	}

	virtual talk_base::StreamState GetState() const
	{
		return ConvertState(loggingStream_->GetState());
	}

	virtual talk_base::StreamResult Read(void* buffer, size_t buffer_len,
						size_t* read, int* error)
	{
		return ConvertResult(loggingStream_->Read(buffer, buffer_len, read, error));
	}

	virtual talk_base::StreamResult Write(const void* data, size_t data_len,
							size_t* written, int* error)
	{
		return ConvertResult(loggingStream_->Write(data, data_len, written, error));
	}

	virtual void Close()
	{
	}

private:
	LoggingStreamInterafce* loggingStream_;
};

void InstallLoggingStream(	LoggingStreamInterafce* loggingStream,
							LoggingSeverity severity)
{
	if (loggingStream)
	{
		talk_base::LogMessage::LogToStream(new LoggingStreamImpl(loggingStream), ConvertSeverity(severity));
	}
	else
	{
		talk_base::LogMessage::LogToStream(NULL, 0);
	}
}

} // webrtc

