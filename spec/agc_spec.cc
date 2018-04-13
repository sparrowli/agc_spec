#include "include/audio_processing.h"
#include "common_audio/wav_file.h"

#include <iostream>
using namespace std;

int main() {
	using namespace mixerengine; 
	AudioProcessing*  apm = AudioProcessing::Create();
	cout<<"============"<<endl;
	GainControl* gain_control = apm->gain_control();
	cout<<"------------"<<endl;

	gain_control->Enable(true);

	cout<<"++++++++++++"<<endl;
	cout<<"Read agc_unprocessed.wav ..."<<endl;
	WavReader r("/Users/sparrow/workspace/cpp/agc_spec/spec/release/agc_unprocessed.wav");
	static const int kNumSamples = 160;
	int16_t read_samples[kNumSamples]; 
	
	while (kNumSamples == r.ReadSamples(kNumSamples, read_samples)) {
		
		const int error = apm->ProcessStream(read_samples, kNumSamples);
		if (error != apm->kNoError) {
			return -1;
		}
	}

	AudioProcessing::Destory(apm);
	return 0;
}
