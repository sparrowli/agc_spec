#include "wav_file.h"

#include <cstdio>
#include <sstream>

#include "wav_header.h"

namespace mixerengine {

// To-Do: build the wave header
static const WavFormat kWavFormat = kWavFormatPcm;
static const size_t kBytesPerSample = 2;

// Doesn't take ownership of the file handle and won't close it.
class ReadableWavFile : public ReadableWav {
 public:
  explicit ReadableWavFile(FILE* file) : file_(file) {}
  virtual size_t Read(void* buf, size_t num_bytes) {
    return fread(buf, 1, num_bytes, file_);
  }

 private:
  FILE* file_;
};

std::string WavFile::FormatAsString() const {
	std::ostringstream s;
	s << "Sample rate: "<< sample_rate() << " Hz, Channels: " << num_channels()
		<< ", Duration: "
		<< (1.f * num_samples()) / (num_channels() * sample_rate()) << " s";
	return s.str();
}

WavReader::WavReader(const std::string& filename)
    : file_handle_(fopen(filename.c_str(), "rb")) {
  //RTC_CHECK(file_handle_) << "Could not open wav file for reading.";

  ReadableWavFile readable(file_handle_);
  WavFormat format;
  size_t bytes_per_sample;
  /*RTC_CHECK(ReadWavHeader(&readable, &num_channels_, &sample_rate_, &format,
                          &bytes_per_sample, &num_samples_));*/
  ReadWavHeader(&readable, &num_channels_, &sample_rate_, &format,
                          &bytes_per_sample, &num_samples_);
  num_samples_remaining_ = num_samples_;
  //RTC_CHECK_EQ(kWavFormat, format);
  //RTC_CHECK_EQ(kBytesPerSample, bytes_per_sample);
}

WavReader::~WavReader() {
  Close();
}

int WavReader::sample_rate() const {
  return sample_rate_;
}

size_t WavReader::num_channels() const {
  return num_channels_;
}

size_t WavReader::num_samples() const {
  return num_samples_;
}

size_t WavReader::ReadSamples(size_t num_samples, int16_t* samples) {
/*
#ifndef WEBRTC_ARCH_LITTLE_ENDIAN
#error "Need to convert samples to big-endian when reading from WAV file"
#endif
*/
  // There could be metadata after the audio; ensure we don't read it.
  num_samples = std::min(num_samples, num_samples_remaining_);
  const size_t read =
      fread(samples, sizeof(*samples), num_samples, file_handle_);
  // If we didn't read what was requested, ensure we've reached the EOF.
  //RTC_CHECK(read == num_samples || feof(file_handle_));
  //RTC_CHECK_LE(read, num_samples_remaining_);
  num_samples_remaining_ -= read;
  return read;
}

void WavReader::Close() {
  //RTC_CHECK_EQ(0, fclose(file_handle_));
  file_handle_ = NULL;
}

WavWriter::WavWriter(const std::string& filename, int sample_rate,
		                 size_t num_channels)
    : sample_rate_(sample_rate),
		  num_channels_(num_channels),
			num_samples_(0),
			file_handle_(fopen(filename.c_str(), "wb")) {
  if (!file_handle_) {

		int* x;
		*x = 0;
	}

	bool error = CheckWavParameters(num_channels_, sample_rate_, kWavFormat,
			               kBytesPerSample, num_samples_);
	if (!error) {

		int* x;
		*x = 0;
	}

	static const uint8_t blank_header[kWavHeaderSize] = {0};
	fwrite(blank_header, kWavHeaderSize, 1, file_handle_);
}

WavWriter::~WavWriter() {
	Close();
}

int WavWriter::sample_rate() const {
	return sample_rate_;
}

size_t WavWriter::num_channels() const {
	return num_channels_;
}

size_t WavWriter::num_samples() const {
	return num_samples_;
}

void WavWriter::WriteSamples(const int16_t* samples, size_t num_samples) {

	const size_t written =
		  fwrite(samples, sizeof(*samples), num_samples, file_handle_);
	// To-Do: sanity check equal
	if (num_samples != written) {
		int* x;
		*x = 0;
	}

	num_samples_ += written;

	// To-Do: detect num_samples_ overflow
	if (num_samples_ < written) {
		int* x;
		*x = 0;
	}
}

void WavWriter::Close() {
	fseek(file_handle_, 0, SEEK_SET);
	uint8_t header[kWavHeaderSize];
	WriteWavHeader(header, num_channels_, sample_rate_, kWavFormat,
			           kBytesPerSample, num_samples_);

	fwrite(header, kWavHeaderSize, 1, file_handle_);
	fclose(file_handle_);
	file_handle_ = NULL;
}

}  // namespace mixerengine 

// To-Do: C wrappers for the WavWriter class
