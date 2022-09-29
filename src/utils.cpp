#include <GL/glew.h>
#include <MEP-3D/utils.hpp>

#include <algorithm>
#include <unordered_map>

#include <glog/logging.h>

namespace {
constexpr char kSeparator = '=';
constexpr char kPrefix = '-';
int RepeatingPrefixIndex(const std::string &name, const char prefix) {
  for (int i = 0; i < name.size(); ++i) {
    if (name[i] != prefix) {
      return i;
    }
  }
  return 0;
}
void InitDefaultGlogLogging() { FLAGS_logtostderr = 1; }
bool InitGlogFlag(const std::string &l_val, const std::string &r_val) {
  if (l_val == "logtostderr") {
    int val = atoi(r_val.c_str());
    if (val != 0 && val != 1) {
      return false;
    }
    FLAGS_logtostderr = val;
    LOG(INFO) << "Overriding logtostderr " << val;
    return true;
  }
  if (l_val == "stderrthreshold") {
    int val = atoi(r_val.c_str());
    FLAGS_logtostderr = val;
    LOG(INFO) << "Overriding stderrthreshold " << val;
    return true;
  }
  if (l_val == "minloglevel") {
    int val = atoi(r_val.c_str());
    FLAGS_minloglevel = val;
    LOG(INFO) << "Overriding minloglevel " << val;
    return true;
  }
  if (l_val == "log_dir") {
    FLAGS_log_dir = r_val;
    LOG(INFO) << "Overriding log_dir " << r_val;
    return true;
  }
  if (l_val == "v") {
    int val = atoi(r_val.c_str());
    FLAGS_v = val;
    LOG(INFO) << "Overriding v " << val;
    return true;
  }
  if (l_val == "vmodule") {
    FLAGS_vmodule = r_val;
    LOG(INFO) << "Overriding vmodule " << r_val;
    return true;
  }
  return false;
}
} // namespace

void utils::InitLogging(int argc, char *argv[]) {
  InitDefaultGlogLogging();
  google::InitGoogleLogging(argv[0]);
  for (int i = 1; i < argc; i++) {
    std::string temp(argv[i]);
    LOG(INFO) << temp;
    auto prefix = RepeatingPrefixIndex(temp, kPrefix);
    if (prefix == 0) {
      LOG(WARNING) << "Input should start from '-'" << temp;
      continue;
    }
    temp = temp.substr(prefix, temp.size());
    auto separator_index = temp.find(kSeparator);
    if (!InitGlogFlag(temp.substr(0, separator_index),
                      temp.substr(separator_index + 1, temp.size() - 1))) {
      LOG(ERROR) << "Could not parse " << temp;
    }
  }
}

glm::vec3 utils::AxisToGlm(Axis axis) {
  switch (axis) {
    case Axis::X:
      return glm::vec3(1.0f, 0.0f, 0.0f);
    case Axis::Y:
      return glm::vec3(0.0f, 1.0f, 0.0f);
    case Axis::Z:
      return glm::vec3(0.0f, 0.0f, 1.0f);
  }
  LOG(FATAL) << "Invalid axis!";
  return glm::vec3(-1.0, -1.0, -1.0);
}

float utils::DegToRadians(float degress) {
  return degress * 3.14159265f / 180.0f;
}

float utils::RadiansToDeg(float radians) {
  return radians * 180.0f / 3.14159265f;
}
