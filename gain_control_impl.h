#ifndef INCLUDE_AUDIO_PROCESSING_GAIN_CONTROL_IMPL_H_
#define INCLUDE_AUDIO_PROCESSING_GAIN_CONTROL_IMPL_H_

#include <memory>
#include <vector>
#include <stdint.h>

#include "include/audio_processing.h"

namespace webrtc {

//class ApmDataDumper;
//class AudioBuffer;

class GainControlImpl : public GainControl {
 public:
 GainControlImpl();
  ~GainControlImpl();

  int AnalyzeCaptureAudio(int16_t* audio);
  int ProcessCaptureAudio(int16_t* audio);
	/*
  int AnalyzeCaptureAudio(AudioBuffer* audio);
  int ProcessCaptureAudio(AudioBuffer* audio, bool stream_has_echo);
	*/

  void Initialize(size_t proc_channels_num, int sample_rate_hz);

	/*
  static void PackRenderAudioBuffer(AudioBuffer* audio,
                                    std::vector<int16_t>* packed_buffer);
	*/

  // GainControl implementation.
  bool is_enabled() const;
  int stream_analog_level();
  bool is_limiter_enabled() const;
  Mode mode() const;

  int compression_gain_db() const;

 private:
 class GainController;

  // GainControl implementation.
  int Enable(bool enable);
  int set_stream_analog_level(int level);
  int set_mode(Mode mode);
  int set_target_level_dbfs(int level);
  int target_level_dbfs() const;
  int set_compression_gain_db(int gain);
  int enable_limiter(bool enable);
  int set_analog_level_limits(int minimum, int maximum);
  int analog_level_minimum() const;
  int analog_level_maximum() const;
  bool stream_is_saturated() const;

  int Configure();

  //ApmDataDumper* data_dumper_;

  bool enabled_;

  Mode mode_;
  int minimum_capture_level_;
  int maximum_capture_level_;
  bool limiter_enabled_;
  int target_level_dbfs_;
  int compression_gain_db_;
  int analog_capture_level_;
  bool was_analog_level_set_;
  bool stream_is_saturated_;

  // here *may appear exception 
  GainController* gain_controller_;

  size_t num_proc_channels_;
  int sample_rate_hz_;

  static int instance_counter_;
};

}  // namespace webrtc
#endif  // INCLUDE_AUDIO_PROCESSING_GAIN_CONTROL_IMPL_H_
