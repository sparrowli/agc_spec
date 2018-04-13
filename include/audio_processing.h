#ifndef INCLUDE_AUDIO_PROCESSING_INCLUDE_AUDIO_PROCESSING_H_
#define INCLUDE_AUDIO_PROCESSING_INCLUDE_AUDIO_PROCESSING_H_

#include <stdint.h>

namespace mixerengine {

class GainControl;

class AudioProcessing {

 public:
  enum Error {
    // Fatal errors.
    kNoError = 0,
    kUnspecifiedError = -1,
    kCreationFailedError = -2,
    kUnsupportedComponentError = -3,
    kUnsupportedFunctionError = -4,
    kNullPointerError = -5,
    kBadParameterError = -6,
    kBadSampleRateError = -7,
    kBadDataLengthError = -8,
    kBadNumberChannelsError = -9,
    kFileError = -10,
    kStreamParameterNotSetError = -11,
    kNotEnabledError = -12,

    // Warnings are non-fatal.
    // This results when a set_stream_ parameter is out of range. Processing
    // will continue, but the parameter may have been truncated.
    kBadStreamParameterWarning = -13
  };

	virtual int ProcessStream(int16_t* data, int num_frames) = 0;

	virtual GainControl* gain_control() const = 0;

	static AudioProcessing* Create();
	static void Destory(AudioProcessing* audio_processing);
	
	~AudioProcessing() {}
};

class GainControl {
 public:
  virtual int Enable(bool enable) = 0;
  virtual bool is_enabled() const = 0;

  virtual int set_stream_analog_level(int level) = 0;

  virtual int stream_analog_level() = 0;

  enum Mode {
    kAdaptiveAnalog,

    kAdaptiveDigital,

    kFixedDigital
  };

  virtual int set_mode(Mode mode) = 0;
  virtual Mode mode() const = 0;

  virtual int set_target_level_dbfs(int level) = 0;
  virtual int target_level_dbfs() const = 0;

  virtual int set_compression_gain_db(int gain) = 0;
  virtual int compression_gain_db() const = 0;

  virtual int enable_limiter(bool enable) = 0;
  virtual bool is_limiter_enabled() const = 0;

  virtual int set_analog_level_limits(int minimum,
                                      int maximum) = 0;
  virtual int analog_level_minimum() const = 0;
  virtual int analog_level_maximum() const = 0;

  virtual bool stream_is_saturated() const = 0;

 protected:
  virtual ~GainControl() {}
};


}  // namespace mixerengine

#endif  // INCLUDE_AUDIO_PROCESSING_INCLUDE_AUDIO_PROCESSING_H_
