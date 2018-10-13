#include "audio_processing_impl.h"

namespace webrtc {

AudioProcessing* AudioProcessing::Create() {
	AudioProcessingImpl* apm = new AudioProcessingImpl();

	if (apm->Initialize() != kNoError) {
		delete apm;
		apm = NULL;
	}

	return apm;
}

void AudioProcessing::Destory(AudioProcessing* audio_processing) {
	if (audio_processing != NULL) {
		AudioProcessingImpl* apm = (AudioProcessingImpl*)audio_processing;
		delete apm;
		apm = NULL;
	}

}

AudioProcessingImpl::AudioProcessingImpl() {
	gain_control_ = new GainControlImpl();
  average_state_ = 0.0;
#ifdef AGC_DEBUG_DUMP
	/*
	static const char log_path[] = "/home/oicq/AudioMixer/log";

	char file_path[64];

	sprintf(file_path, "%s/processed_agc.pcm", log_path);
	fpt_processed_ = fopen(file_path, "wb");
	*/
	//processed_wav_file_ = new WavWriter("/home/oicq/AudioMixer/log/agc_processed.wav", 8000, 1);
	processed_wav_file_ = new WavWriter("/Users/sparrow/workspace/cpp/agc_spec/spec/release/agc_processed.wav", 8000, 1);

	/*
	sprintf(file_path, "%s/unprocessed_agc.pcm", log_path);
	fpt_unprocessed_ = fopen(file_path, "wb");
	*/
	//unprocessed_wav_file_ = new WavWriter("/home/oicq/AudioMixer/log/agc_unprocessed.wav", 8000, 1);
	unprocessed_wav_file_ = new WavWriter("/Users/sparrow/workspace/cpp/agc_spec/spec/release/read_unprocessed.wav", 8000, 1);

#endif
}

AudioProcessingImpl::~AudioProcessingImpl() {
	if (gain_control_ != NULL) {
		delete gain_control_;
		gain_control_ = NULL;
	}

  average_state_ = 0.0;
#ifdef AGC_DEBUG_DUMP
	/*
	fclose(fpt_unprocessed_);
	fclose(fpt_processed_);
	*/

	if (processed_wav_file_ != NULL) {
		delete processed_wav_file_; 
		processed_wav_file_ = NULL;
	}
	if (unprocessed_wav_file_ != NULL) {
		delete unprocessed_wav_file_; 
		unprocessed_wav_file_ = NULL;
	}
#endif
}

int AudioProcessingImpl::Initialize() {
	gain_control_->Initialize(1, 8000);

	return kNoError;
}

GainControl* AudioProcessingImpl::gain_control() const {
	return gain_control_;
}

int AudioProcessingImpl::RemoveDCBias(int16_t* data, int num_frames) {
	float amplitude_sum = 0;
	float average_value = 0;

	for (int i = 0; i < num_frames; ++i) {
		amplitude_sum += data[i];
	}

	// Q8
	//amplitude_sum <<= 8;
	average_value = amplitude_sum / num_frames;
  average_state_ = 0.15 * average_value  + 0.85 * average_state_; 
	for (int i = 0; i < num_frames; ++i) {
		data[i] = stoshort(data[i] - average_state_);
	}

	return 0;
}

int AudioProcessingImpl::ProcessStream(int16_t* data, int num_frames) {

	if (!data) {
		return kNullPointerError; 
	}

	// To-Do: only support 8k mono 20ms
	if (num_frames != 160) {
		return kBadParameterError; 
	}

	// To-Do: dump the unprocesssed stream
#ifdef AGC_DEBUG_DUMP	
	//fwrite(data, sizeof(int16_t), num_frames, fpt_unprocessed_);
	unprocessed_wav_file_->WriteSamples(data, num_frames);
#endif
	int error = kNoError;
	size_t L = 80;

	//RemoveDCBias(data, num_frames);
	// To-Do: only support 10ms frames
	for (int i = 0; i < 2; ++i) {
		error = gain_control_->AnalyzeCaptureAudio(data + i*L);
		if (error != kNoError) {
			return error;
		}

		error = gain_control_->ProcessCaptureAudio(data + i*L);
		if (error != kNoError) {
			return error;
		}
	}

	//RemoveDCBias(data, num_frames);
	// To-Do: dump the processed stream
#ifdef AGC_DEBUG_DUMP	
	//fwrite(data, sizeof(int16_t), num_frames, fpt_processed_);
	processed_wav_file_->WriteSamples(data, num_frames);
#endif

	return error;
}

}  // namespace webrtc 
