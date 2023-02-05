#include <MEP-3D/file_reader_with_progress.hpp>

float FileReaderWithProgress::GetProgress() {
  last_pos_ = std::ifstream::tellg();
  if (last_pos_ == -1 || last_pos_ == file_length_) {
    return 1.f;
  }
  return static_cast<float>(last_pos_) / static_cast<float>(file_length_);
}

long long FileReaderWithProgress::GetSize() const {
  return file_length_;
}

void FileReaderWithProgress::Init() {
  std::ifstream::seekg(0, end);
  file_length_ = std::ifstream::tellg();
  std::ifstream::seekg(0, beg);
}
