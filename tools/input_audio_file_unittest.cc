/*
 *  Copyright (c) 2018 sparrowli. All Rights Reserved.
 *
 */

// Unit tests for test InputAudioFile class.

#include "tools/input_audio_file.h"
#include "base/numerics/safe_conversions.h"

#include "gtest/gtest.h"

namespace webrtc {
namespace test {

TEST(TestInputAudioFile, DuplicateInterleaveSeparateSrcDst) {
  static const size_t kSamples = 10;
  static const size_t kChannels = 2;
  int16_t input[kSamples];
  for (size_t i = 0; i < kSamples; ++i) {
    input[i] = rtc::checked_cast<int16_t>(i);
  }
  int16_t output[kSamples * kChannels];
  InputAudioFile::DuplicateInterleaved(input, kSamples, kChannels, output);

  // Verify output
  int16_t* output_ptr = output;
  for (size_t i = 0; i < kSamples; ++i) {
    for (size_t j = 0; j < kChannels; ++j) {
      EXPECT_EQ(static_cast<int16_t>(i), *output_ptr++);
    }
  }
}

TEST(TestInputAudioFile, DuplicateInterleaveSameSrcDst) {
  static const size_t kSamples = 10;
  static const size_t kChannels = 5;
  int16_t input[kSamples * kChannels];
  for (size_t i = 0; i < kSamples; ++i) {
    input[i] = rtc::checked_cast<int16_t>(i);
  }
  InputAudioFile::DuplicateInterleaved(input, kSamples, kChannels, input);

  // Verify output
  int16_t* output_ptr = input;
  for (size_t i = 0; i < kSamples; ++i) {
    for (size_t j = 0; j < kChannels; ++j) {
      EXPECT_EQ(static_cast<int16_t>(i), *output_ptr++);
    }
  }
}

}  // namespace test
}  // namespace webrtc
