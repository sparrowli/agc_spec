#ifndef INCLUDE_AUDIO_PROCESSING_COMMON_AUDIO_WAV_FILE_H_
#define INCLUDE_AUDIO_PROCESSING_COMMON_AUDIO_WAV_FILE_H_

#ifdef __cplusplus

#include <stdint.h>
#include <stddef.h>
#include <string>

namespace mixerengine {

class WavFile {
 public:
  virtual ~WavFile() {}

	virtual int sample_rate() const = 0;
	virtual size_t num_channels() const = 0;
	virtual size_t num_samples() const = 0;

	std::string FormatAsString() const;
};

class WavWriter : public WavFile {
 public:
  WavWriter(const std::string& filename, int sample_rate, size_t num_channels);

	~WavWriter();

	void WriteSamples(const int16_t* samples, size_t num_samles);

	int sample_rate() const;
	size_t num_channels() const;
	size_t num_samples() const;

 private:
  void Close();
	const int sample_rate_;
	const size_t num_channels_;
	size_t num_samples_;
	FILE* file_handle_;

	// To-Do: disallow copy and assign
};

// Follows the conventions of WavWriter.
class WavReader : public WavFile {
 public:
  // Opens an existing WAV file for reading.
  explicit WavReader(const std::string& filename);

  // Close the WAV file.
  ~WavReader();

  // Returns the number of samples read. If this is less than requested,
  // verifies that the end of the file was reached.
  size_t ReadSamples(size_t num_samples, float* samples);
  size_t ReadSamples(size_t num_samples, int16_t* samples);

  int sample_rate() const;
  size_t num_channels() const;
  size_t num_samples() const;

 private:
  void Close();
  int sample_rate_;
  size_t num_channels_;
  size_t num_samples_;  // Total number of samples in the file.
  size_t num_samples_remaining_;
  FILE* file_handle_;  // Input file, owned by this class.
};

}  // namespace mixerengine

extern "C" {
#endif  // __cplusplus

// To-Do: C wrappers for the WavWriter class.
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // INCLUDE_AUDIO_PROCESSING_COMMON_AUDIO_WAV_FILE_H_
