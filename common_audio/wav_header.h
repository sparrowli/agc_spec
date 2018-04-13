#ifndef INCLUDE_AUDIO_PROCESSING_COMMON_AUDIO_WAV_HEAD_H_
#define INCLUDE_AUDIO_PROCESSING_COMMON_AUDIO_WAV_HEAD_H_

#include <stddef.h>
#include <stdint.h>

namespace mixerengine {

static const size_t kWavHeaderSize = 44;

class ReadableWav {
 public:
  // Returns the number of bytes read.
  size_t virtual Read(void* buf, size_t num_bytes) = 0;
  virtual ~ReadableWav() {}
};

enum WavFormat {
	kWavFormatPcm   = 1,  // PCM
  kWavFormatALaw  = 6,  // 8-bit ITU-T G.711 A-law
	kWavFormatMuLaw = 7,  // 8-bit ITU-T G.711 mu-law
};


bool CheckWavParameters(size_t num_channels,
		                    int sample_rate,
												WavFormat format,
												size_t bytets_per_sample,
												size_t num_samples);

void WriteWavHeader(uint8_t* buf,
		                size_t num_channels,
										int sample_rate,
										WavFormat format,
										size_t bytes_per_sample,
										size_t num_samples);

// Read a WAV header from an implemented ReadableWav and parse the values into
// the provided output parameters. ReadableWav is used because the header can
// be variably sized. Returns false if the header is invalid.
bool ReadWavHeader(ReadableWav* readable,
                   size_t* num_channels,
                   int* sample_rate,
                   WavFormat* format,
                   size_t* bytes_per_sample,
                   size_t* num_samples);

}  // namespace mixerengine 

#endif  // INCLUDE_AUDIO_PROCESSING_COMMON_AUDIO_WAV_HEAD_H_
