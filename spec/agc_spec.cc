#include "include/audio_processing.h"
#include "common_audio/wav_file.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {


  if (argc != 3) {
    printf("\n\nWrong number of arguments or flag values.\n\n");

    printf("\n");
    printf("\nagc_spec test application\n\n");
    printf("Usage:\n\n");
    printf("./agc_spec unprocessed.wav processed.wav\n\n");
    exit(0);

  }





	using namespace mixerengine; 
	AudioProcessing*  apm = AudioProcessing::Create();
	cout<<"============"<<endl;
	GainControl* gain_control = apm->gain_control();
	cout<<"------------"<<endl;

	gain_control->Enable(true);

	
	char in[256];
	char out[256];


  sscanf(argv[1], "%s", in);
  sscanf(argv[2], "%s", out);

	WavWriter* out_wav_ = new WavWriter(out, 8000, 1);

	cout<<"++++++++++++"<<endl;
	cout<<"Read  "<<in<<"..."<<endl;
	//WavReader r("/Users/sparrow/workspace/cpp/agc_spec/spec/release/agc_unprocessed.wav");
	WavReader r(in);
	static const int kNumSamples = 160;
	int16_t read_samples[kNumSamples]; 
	
	while (kNumSamples == r.ReadSamples(kNumSamples, read_samples)) {
		
		const int error = apm->ProcessStream(read_samples, kNumSamples);
		if (error != apm->kNoError) {
			return -1;
		}
		out_wav_->WriteSamples(read_samples, kNumSamples);
	}

	delete out_wav_;
  out_wav_ = NULL;
	AudioProcessing::Destory(apm);
	return 0;
}
