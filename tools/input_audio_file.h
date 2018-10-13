/*
 *  Copyright (c) 2018 sparrowli. All Rights Reserved.
 *
 */

#ifndef TOOLS_INPUT_AUDIO_FILE_H_
#define TOOLS_INPUT_AUDIO_FILE_H_

#include <stdio.h>

#include <string>

#include "base/constructormagic.h"

namespace webrtc {
namespace test {

// Class for handling a looping input audio file.
class InputAudioFile {
 public:
  explicit InputAudioFile(const std::string file_name, bool loop_at_end = true);

  virtual ~InputAudioFile();

  // Reads |samples| elements from source file to |destination|. Returns true
  // if the read was successful, otherwise false. If the file end is reached,
  // the file is rewound and reading continues from the beginning.
  // The output |destination| must have the capacity to hold |samples| elements.
  virtual bool Read(size_t samples, int16_t* destination);

  // Fast-forwards (|samples| > 0) or -backwards (|samples| < 0) the file by the
  // indicated number of samples. Just like Read(), Seek() starts over at the
  // beginning of the file if the end is reached. However, seeking backwards
  // past the beginning of the file is not possible.
  virtual bool Seek(int samples);

  // Creates a multi-channel signal from a mono signal. Each sample is repeated
  // |channels| times to create an interleaved multi-channel signal where all
  // channels are identical. The output |destination| must have the capacity to
  // hold samples * channels elements. Note that |source| and |destination| can
  // be the same array (i.e., point to the same address).
  static void DuplicateInterleaved(const int16_t* source,
                                   size_t samples,
                                   size_t channels,
                                   int16_t* destination);

 private:
  FILE* fp_;
  const bool loop_at_end_;
  RTC_DISALLOW_COPY_AND_ASSIGN(InputAudioFile);
};

}  // namespace test
}  // namespace webrtc
#endif  // TOOLS_INPUT_AUDIO_FILE_H_
