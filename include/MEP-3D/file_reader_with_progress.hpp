#ifndef FILE_READER_WITH_PROGRESS_HPP
#define FILE_READER_WITH_PROGRESS_HPP

#include <fstream>

namespace mep {
class FileReaderWithProgress : public std::ifstream {
 public:
  template <class... Args>
  inline FileReaderWithProgress(Args&&... args) : std::ifstream(args...) {
    if (std::ifstream::is_open())
      Init();
  }

  template <class... Args>
  inline auto open(Args&&... args) -> decltype(std::ifstream::open(args...)) {
    auto rvalue(std::ifstream::open(args...));
    if (std::ifstream::is_open())
      Init();
    return rvalue;
  }

  float GetProgress();
  long long GetSize() const;

 private:
  long long last_pos_ = 0;
  long long file_length_ = 0;

  void Init();
};
}  // namespace mep

#endif