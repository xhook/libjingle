#ifndef TALK_APP_WEBRTC_LOGGING_H_
#define TALK_APP_WEBRTC_LOGGING_H_

#include <cstddef>

#include "config.h"

namespace webrtc {

// Note that the non-standard LoggingSeverity aliases exist because they are
// still in broad use.  The meanings of the levels are:
//  LOGGING_SEVERITY_SENSITIVE: Information which should only be logged with the consent
//   of the user, due to privacy concerns.
//  LOGGING_SEVERITY_VERBOSE: This level is for data which we do not want to appear in the
//   normal debug log, but should appear in diagnostic logs.
//  LOGGING_SEVERITY_INFO: Chatty level used in debugging for all sorts of things, the default
//   in debug builds.
//  LOGGING_SEVERITY_WARNING: Something that may warrant investigation.
//  LOGGING_SEVERITY_ERROR: Something that should not have occurred.
enum LoggingSeverity
{
	LOGGING_SEVERITY_SENSITIVE,
	LOGGING_SEVERITY_VERBOSE,
	LOGGING_SEVERITY_INFO,
	LOGGING_SEVERITY_WARNING,
	LOGGING_SEVERITY_ERROR
};

class LoggingStreamInterafce
{
public:

	// The SS_OPENING state indicates that the stream will signal open or closed
	// in the future.
	enum State { STATE_CLOSED, STATE_OPENING, STATE_OPEN };

	// Stream read/write methods return this value to indicate various success
	// and failure conditions described below.
	enum Result { RESULT_ERROR, RESULT_SUCCESS, RESULT_BLOCK, RESULT_EOS };

public:

	virtual State GetState() const = 0;

	// Read attempts to fill buffer of size buffer_len.  Write attempts to send
	// data_len bytes stored in data.  The variables read and write are set only
	// on RESULT_SUCCESS (see below).  Likewise, error is only set on RESULT_ERROR.
	// Read and Write return a value indicating:
	//  RESULT_ERROR: an error occurred, which is returned in a non-null error
	//    argument.  Interpretation of the error requires knowledge of the
	//    stream's concrete type, which limits its usefulness.
	//  RESULT_SUCCESS: some number of bytes were successfully written, which is
	//    returned in a non-null read/write argument.
	//  RESULT_BLOCK: the stream is in non-blocking mode, and the operation would
	//    block, or the stream is in STATE_OPENING state.
	//  RESULT_EOS: the end-of-stream has been reached, or the stream is in the
	//    STATE_CLOSED state.
	virtual Result Read(void* buffer, size_t buffer_len,
						size_t* read, int* error) = 0;
	virtual Result Write(const void* data, size_t data_len,
						 size_t* written, int* error) = 0;

	virtual void Destroy() = 0;

	virtual ~LoggingStreamInterafce() {}
};

// Install any non-blocking stream interface. Takes ownership of the stream.
JINGLE_API void InstallLoggingStream(	LoggingStreamInterafce* loggingStream,
										LoggingSeverity severity);

} // webrtc

#endif // TALK_APP_WEBRTC_LOGGING_H_
