#include <GL/glew.h>
#include <MEP-3D/features.hpp>
#include <MEP-3D/utils.hpp>

#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include <glog/logging.h>

namespace {
constexpr char kSeparator = '=';
constexpr char kPrefix = '-';
constexpr char kFeatureFlag[] = "feature";
constexpr char kFeatureSeparator = ':';
constexpr char kFeatureSign = '=';
const std::unordered_set<std::string_view> kTrueValues = {"true", "TRUE", "T",
                                                          "True"};
const std::unordered_set<std::string_view> kFalseValues = {"false", "FALSE",
                                                           "F", "False"};

int RepeatingPrefixIndex(const std::string& name, const char prefix) {
  for (int i = 0; i < name.size(); ++i) {
    if (name[i] != prefix) {
      return i;
    }
  }
  return 0;
}

void InitDefaultGlogLogging() {
  FLAGS_logtostderr = 1;
}

bool InitGlogFlag(const std::string& l_val, const std::string& r_val) {
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
}  // namespace

void utils::Init(int argc, char* argv[]) {
  InitLogging(argc, argv);
  InitFeatures(argc, argv);
}

void utils::InitLogging(int argc, char* argv[]) {
  InitDefaultGlogLogging();
  google::InitGoogleLogging(argv[0]);
  for (int i = 1; i < argc; i++) {
    std::string temp(argv[i]);
    auto prefix = RepeatingPrefixIndex(temp, kPrefix);
    if (prefix == 0) {
      continue;
    }
    temp = temp.substr(prefix, temp.size());
    auto separator_index = temp.find(kSeparator);
    InitGlogFlag(temp.substr(0, separator_index),
                 temp.substr(separator_index + 1, temp.size() - 1));
  }
}

void utils::InitFeatures(int argc, char* argv[]) {
#ifdef MEP_ENGINE_USE_IMGUI
  feature::ForceFeature(feature::kUseEngineDataMonitor, true);
#endif
  for (int i = 0; i < argc; ++i) {
    std::string temp(argv[i]);
    int value = temp.find(kFeatureFlag);
    int separator = temp.find(kFeatureSeparator);
    int sign = temp.find(kFeatureSign);
    if (value == -1 || separator == -1 || sign == -1) {
      continue;
    }
    if (sign < separator || separator < value) {
      continue;
    }
    std::string feature_name(temp.substr(separator + 1, sign - separator - 1));
    std::string feature_sign(temp.substr(sign + 1, temp.size() - 1));
    if (kTrueValues.count(feature_sign)) {
      feature::ForceFeature(feature_name, true);
    } else if (kFalseValues.count(feature_sign)) {
      feature::ForceFeature(feature_name, false);
    } else {
      LOG(WARNING) << "Could not parse " << temp << ", invalid sign!";
      continue;
    }
    VLOG(1) << "Feature parsed " << feature_name;
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

std::string utils::LoadFromFile(const std::string& file_path) {
  std::ifstream input_file(file_path);
  if (!input_file.is_open()) {
    LOG(ERROR) << "Unable to open file " << file_path;
    return "";
  }
  return std::string((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>());
}
