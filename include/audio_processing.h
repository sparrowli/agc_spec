#ifndef INCLUDE_AUDIO_PROCESSING_INCLUDE_AUDIO_PROCESSING_H_
#define INCLUDE_AUDIO_PROCESSING_INCLUDE_AUDIO_PROCESSING_H_

#include <stdint.h>
#include <stddef.h>

namespace webrtc {

class GainControl;

class AudioProcessing {

 public:

  // TODO(mgraczyk): Remove once all methods that use ChannelLayout are gone.
  enum ChannelLayout {
    kMono,
    // Left, right.
    kStereo,
    // Mono, keyboard, and mic.
    kMonoAndKeyboard,
    // Left, right, keyboard, and mic.
    kStereoAndKeyboard
  };

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
  static const int kChunkSizeMs = 10;
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

class StreamConfig {
 public:
  // sample_rate_hz: The sampling rate of the stream.
  //
  // num_channels: The number of audio channels in the stream, excluding the
  //               keyboard channel if it is present. When passing a
  //               StreamConfig with an array of arrays T*[N],
  //
  //                N == {num_channels + 1  if  has_keyboard
  //                     {num_channels      if  !has_keyboard
  //
  // has_keyboard: True if the stream has a keyboard channel. When has_keyboard
  //               is true, the last channel in any corresponding list of
  //               channels is the keyboard channel.
  StreamConfig(int sample_rate_hz = 0,
               size_t num_channels = 0,
               bool has_keyboard = false)
      : sample_rate_hz_(sample_rate_hz),
        num_channels_(num_channels),
        has_keyboard_(has_keyboard),
        num_frames_(calculate_frames(sample_rate_hz)) {}

  void set_sample_rate_hz(int value) {
    sample_rate_hz_ = value;
    num_frames_ = calculate_frames(value);
  }
  void set_num_channels(size_t value) { num_channels_ = value; }
  void set_has_keyboard(bool value) { has_keyboard_ = value; }

  int sample_rate_hz() const { return sample_rate_hz_; }

  // The number of channels in the stream, not including the keyboard channel if
  // present.
  size_t num_channels() const { return num_channels_; }

  bool has_keyboard() const { return has_keyboard_; }
  size_t num_frames() const { return num_frames_; }
  size_t num_samples() const { return num_channels_ * num_frames_; }

  bool operator==(const StreamConfig& other) const {
    return sample_rate_hz_ == other.sample_rate_hz_ &&
           num_channels_ == other.num_channels_ &&
           has_keyboard_ == other.has_keyboard_;
  }

  bool operator!=(const StreamConfig& other) const { return !(*this == other); }

 private:
  static size_t calculate_frames(int sample_rate_hz) {
    return static_cast<size_t>(AudioProcessing::kChunkSizeMs * sample_rate_hz /
                               1000);
  }

  int sample_rate_hz_;
  size_t num_channels_;
  bool has_keyboard_;
  size_t num_frames_;
};

}  // namespace webrtc

#endif  // INCLUDE_AUDIO_PROCESSING_INCLUDE_AUDIO_PROCESSING_H_
