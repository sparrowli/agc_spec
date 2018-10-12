#ifndef INCLUDE_AUDIO_PROCESSING_AUDIO_PROCESSING_IMPL_H_
#define INCLUDE_AUDIO_PROCESSING_AUDIO_PROCESSING_IMPL_H_

#include "include/audio_processing.h"

#include "gain_control_impl.h"
#include "common_audio/wav_file.h"

// #define AGC_DEBUG_DUMP
#ifdef AGC_DEBUG_DUMP
#include <stdio.h>
#endif
namespace mixerengine {

static __inline short stoshort(float x){short y;y = (short)x;	if(x>32767)y=32767;if(x<-32768)y = -32768;return y;}

class AudioProcessingImpl : public AudioProcessing {
 public:
	AudioProcessingImpl();
	~AudioProcessingImpl();

	int Initialize();
	GainControl* gain_control() const;

	int RemoveDCBias(int16_t* data, int num_frames);
	int ProcessStream(int16_t* data, int num_frames);
 private:
	GainControlImpl* gain_control_;
  float average_state_;
#ifdef AGC_DEBUG_DUMP
	FILE* fpt_unprocessed_;
	FILE* fpt_processed_;
	WavWriter* unprocessed_wav_file_;
	WavWriter* processed_wav_file_;
#endif
};

}  // namespace mixerengine 
#endif  // INCLUDE_AUDIO_PROCESSING_AUDIO_PROCESSING_IMPL_H_
