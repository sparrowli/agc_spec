#include "gain_control_impl.h"

#include "digital_agc/gain_control.h"

#include <stdio.h>
#include <iostream>
using namespace std;

namespace mixerengine {

typedef void Handle;

namespace {
	int16_t MapSetting(GainControl::Mode mode) {
		switch (mode) {
			case GainControl::kAdaptiveAnalog:
				return kAgcModeAdaptiveAnalog;
			case GainControl::kAdaptiveDigital:
				return kAgcModeAdaptiveDigital;
			case GainControl::kFixedDigital:
				return kAgcModeFixedDigital;
		}

		return -1;
	}
}  // namespace

class GainControlImpl::GainController {
 public:
  explicit GainController() {
		state_ = WebRtcAgc_Create();
		if (0 == state_)
		{
			printf("%s:%d Check failed\n", __FILE__, __LINE__);
		}
	}

	~GainController() {
		if (0 != state_)
			WebRtcAgc_Free(state_);
	}

	Handle* state() {
		if (0 == state_)
		{
			return NULL;
		}
		return state_;
	}

	void Initialize(int minimum_capture_level,
			            int maximum_capture_level,
									Mode mode,
									int sample_rate_hz,
									int capture_level) {
		if (0 == state_)
			return;

		cout<<" min_capture_level="<<minimum_capture_level;
		cout<<" max_capture_level="<<maximum_capture_level;
		cout<<" mode="<<mode;
		cout<<" sample_rate_hz="<<sample_rate_hz;
		cout<<" capture_level="<<capture_level;
		cout<<endl;
		int error = 
			WebRtcAgc_Init(state_, minimum_capture_level, maximum_capture_level,
					MapSetting(mode), sample_rate_hz);

		if (0 != error)
		{
			printf("%s:%d WebRtcAgc_Init failed.\n", __FILE__, __LINE__);
		}

		set_capture_level(capture_level);
	}

	void set_capture_level(int capture_level) {
		capture_level_ = capture_level;
	}

	int get_capture_level() {
		return capture_level_;
	}

 private:
  Handle* state_;	
	int capture_level_;
};

int GainControlImpl::instance_counter_ = 0; 

GainControlImpl::GainControlImpl()
    : //data_dumper_(NULL/*new ApmDataDumper(instance_counter_)*/),
			enabled_(false),
		  mode_(kAdaptiveDigital),
			minimum_capture_level_(0),
			maximum_capture_level_(255),
			limiter_enabled_(true),
			target_level_dbfs_(4),
			compression_gain_db_(9),
			analog_capture_level_(0),
			was_analog_level_set_(false),
			stream_is_saturated_(false) {
}


GainControlImpl::~GainControlImpl() {
	if (gain_controller_ != NULL) {
		delete gain_controller_;
	}
}

int GainControlImpl::AnalyzeCaptureAudio(int16_t* audio) {

	if (!enabled_) {
		return AudioProcessing::kNoError;
	}

	if (mode_ == kAdaptiveDigital) {
		int32_t level_out = 0;
		int err = WebRtcAgc_VirtualMic(
				gain_controller_->state(), &audio,
				1, 80, 0, &level_out);

		gain_controller_->set_capture_level(level_out);	

		if (err != AudioProcessing::kNoError) {
			return AudioProcessing::kUnspecifiedError; 
		}
	} else if (mode_ == kAdaptiveAnalog) {
		return AudioProcessing::kNotEnabledError; 
	}

	return AudioProcessing::kNoError;
}

int GainControlImpl::ProcessCaptureAudio(int16_t* audio) {

	if (!enabled_) {
		return AudioProcessing::kNoError;
	}

	// To-Do: only support kAdaptiveDigital
	if (mode_ == kAdaptiveAnalog || mode_ == kFixedDigital) {
		return AudioProcessing::kUnsupportedFunctionError;
	}

	stream_is_saturated_ = false;
	int32_t capture_level_out = 0;
	uint8_t saturation_warning = 0;
	const int16_t* const* in_near = &audio;
	int16_t* const* out = &audio;
	
	int err = WebRtcAgc_Process(
			gain_controller_->state(), in_near,
			1, 80, 
			out,
			gain_controller_->get_capture_level(), &capture_level_out,
			false, &saturation_warning);

	if (err != AudioProcessing::kNoError) {
		return AudioProcessing::kUnspecifiedError; 
	}
	
	gain_controller_->set_capture_level(capture_level_out);
	if (saturation_warning == 1) {
		stream_is_saturated_ = true;
	}

	return AudioProcessing::kNoError;
}

/*
int GainControlImpl::AnalyzeCaptureAudio(AudioBuffer* audio) {
		return AudioProcessing::kNoError;
	}

	if (mode_ == kAdaptiveAnalog) {
		// Don't support adaptive analog on server side as
		// an analog volume control in unavailable
		return AudioProcessing::kNoError;
	} else if (mode_ == kAdaptiveDigital) {

		int capture_channel = 0;
		int32_t capture_level_out = 0;
		int err = WebRtcAgc_VirtualMic(
				gain_controller_->state(), audio->split_bands(capture_channel),
				audio->num_bands(), audio->num_frames_per_band(),
				analog_capture_level_, &capture_level_out);
		if (err != AudioProcessing::kNoError) {
			return AudioProcesing::kUnspecifiedError;
		}
		++capture_channel;
		
		for (std::vector<GainController*>::iterator gain_controller = gain_controllers_.begin();
				gain_controller != gain_controllers_.end(); ++gain_controller) {
			int32_t capture_level_out = 0;
			int err = WebRtcAgc_VirtualMic(
					gain_controller->state(), audio->split_bands(capture_channel),
					audio->num_bands(), audio->num_frames_per_band(),
					analog_capture_level_, &capture_level_out);

			gain_controller->set_capture_level(capture_level_out);

			if (err != AudioProcessing::kNoError) {
				return AudioProcesing::kUnspecifiedError;
			}
			++capture_channel;
		}
	}

  return AudioProcessing::kNoError;
}
*/
/*
int GainControlImpl::ProcessCaptureAudio(AudioBuffer* audio,
		                                     bool stream_has_echo) {
	if (!enabled_) {
		return AudioProcessing::kNoError;
	}

	if (0 >= num_proc_channels_) {
		return AudioProcessing::kBadNumberChannelsError; 
	}

	if (160 >= audio->num_frames_per_band()) {
		return AudioProcessing::kBadDataLengthError; 
	}

	if (audio->num_channels() != num_proc_channels_) {
		return AudioProcessing::kBadNumberChannelsError; 
	}

	stream_is_saturated_ = false;
	int capture_channel = 0;
	int32_t capture_level_out = 0;
	uint8_t saturation_warning = 0;

	int err = WebRtcAgc_Process(
			gain_controller_->state(), audio->split_bands_const(capture_channel),
			audio->num_bands(), audio->num_frames_per_band(),
			audio->split_bands(capture_channel),
			gain_controller_->get_capture_level(), &capture_level_out,
			false, &saturation_warning);
	if (err != AudioProcessing::kNoError) {
		return AudioProcesing::kUnspecifiedError;
	}

	gain_controller->set_capture_level(capture_level_out);
	if (1 == saturation_warning) {
		stream_is_saturated_ = true;
	}
	
	++capture_channel;
	
	for (std::vector<GainController>::iterator gain_controller = gain_controllers_.begin();
			gain_controller != gain_controlers_.end(); ++gain_controller) {
		int32_t capture_level_out = 0;
		uint8_t saturation_warning = 0;

		int err = WebRtcAgc_Process(
				gain_controller->state(), audio->split_bands_const(capture_channel),
				audio->num_bands(), audio->num_frames_per_band(),
				audio->split_bands(capture_channel),
				gain_controller->get_capture_level(), &capture_level_out,
				false/stream_has_echo/, &saturation_warning);

		if (err != AudioProcessing::kNoError) {
			return AudioProcesing::kUnspecifiedError;
		}

		gain_controller->set_capture_level(capture_level_out);
		if (1 == saturation_warning) {
			stream_is_saturated_ = true;
		}
		
		++capture_channel;
	}

	was_analog_level_set_ = false;
	return AudioProcessing::kNoError;
}
*/

int GainControlImpl::compression_gain_db() const {
	return compression_gain_db_;
}

int GainControlImpl::set_stream_analog_level(int level) {
	//data_dumper_->DumpRaw("gain_control_set_stream_analog_level", 1, &level);

	was_analog_level_set_ = true;
	if (level < minimum_capture_level_ || level > maximum_capture_level_) {
		return AudioProcessing::kBadParameterError;
	}
	analog_capture_level_ = level;

	return AudioProcessing::kNoError;
}


int GainControlImpl::stream_analog_level() {
	return analog_capture_level_; 
}

int GainControlImpl::Enable(bool enable) {
	if (enable && !enabled_) {
		enabled_ = enable;

		Initialize(num_proc_channels_, sample_rate_hz_);
	} else {
		enabled_ = enable;
	}
	return AudioProcessing::kNoError;
}

bool GainControlImpl::is_enabled() const {
	return enabled_;
}

int GainControlImpl::set_mode(Mode mode) {
	if (MapSetting(mode) == -1) {
		return AudioProcessing::kBadParameterError;
	}

	mode_ = mode;
	Initialize(num_proc_channels_, sample_rate_hz_);
	return AudioProcessing::kNoError;
}

GainControl::Mode GainControlImpl::mode() const {
	return mode_;
}

int GainControlImpl::set_analog_level_limits(int minimum,
		                                         int maximum) {
	if (minimum < 0) {
		return AudioProcessing::kBadParameterError;
	}

	if (maximum < 0) {
		return AudioProcessing::kBadParameterError;
	}

	if (maximum < minimum) {
		return AudioProcessing::kBadParameterError;
	}

	size_t num_proc_channels_local = 0u;
	int sample_rate_hz_local = 0;

	minimum_capture_level_ = minimum;
	maximum_capture_level_ = maximum;

	num_proc_channels_local =  num_proc_channels_;
	sample_rate_hz_local = sample_rate_hz_;

	Initialize(num_proc_channels_local, sample_rate_hz_local);
	return AudioProcessing::kNoError;
}

int GainControlImpl::analog_level_minimum() const {
	return minimum_capture_level_;
}

int GainControlImpl::analog_level_maximum() const {
	return maximum_capture_level_;
}

bool GainControlImpl::stream_is_saturated() const {
	return stream_is_saturated_;
}

int GainControlImpl::set_target_level_dbfs(int level) {
	if (level > 31 || level < 0) {
		return AudioProcessing::kBadParameterError; 
	}

	target_level_dbfs_ = level;

	return Configure();
}

int GainControlImpl::target_level_dbfs() const {
	return target_level_dbfs_;
}

int GainControlImpl::set_compression_gain_db(int gain) {
	if (gain < 0 || gain > 90) {
		return AudioProcessing::kBadParameterError; 
	}

  compression_gain_db_ = gain;

	return Configure();
}

int GainControlImpl::enable_limiter(bool enable) {
  limiter_enabled_ = enable;

	return Configure();
}

bool GainControlImpl::is_limiter_enabled() const {
	return limiter_enabled_;
}

void GainControlImpl::Initialize(size_t num_proc_channels, int sample_rate_hz) {
	num_proc_channels_ = num_proc_channels;
	sample_rate_hz_    = sample_rate_hz;

	if (!enabled_) {
		return;
	}

	gain_controller_ = new GainController();

	gain_controller_->Initialize(minimum_capture_level_, maximum_capture_level_,
			                        mode_, sample_rate_hz_, analog_capture_level_);

	cout<<"gain_control_impl initialize(0, 255, digital, 8000, 0)"<<endl;
	Configure();
}


int GainControlImpl::Configure() {
  WebRtcAgcConfig config;

	config.targetLevelDbfs = static_cast<int16_t>(target_level_dbfs_);
	config.compressionGaindB =
		  static_cast<int16_t>(compression_gain_db_);
	config.limiterEnable = limiter_enabled_;

	int error = AudioProcessing ::kNoError;
	const int handle_error = 
		  WebRtcAgc_set_config(gain_controller_->state(), config);
	if (handle_error != AudioProcessing::kNoError) {
		error = handle_error;
	}

	return error;
}

}  // namespace mixerengine 
