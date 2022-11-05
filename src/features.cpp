#include <glog/logging.h>
#include <MEP-3D/features.hpp>

#include <unordered_set>

namespace {
constexpr char kFeatureFlag[] = "feature";
constexpr char kFeatureSeparator = ':';
constexpr char kFeatureSign = '=';
const std::unordered_set<std::string_view> kTrueValues = {"true", "TRUE", "T",
                                                          "True"};
const std::unordered_set<std::string_view> kFalseValues = {"false", "FALSE",
                                                           "F", "False"};
};  // namespace

bool Features::Init(int argc, char* argv[]) {
#ifdef MEP_ENGINE_USE_IMGUI
  ForceFeature(feature::UseEngineDataMonitor, true);
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
      ForceFeature(feature_name, true);
    } else if (kFalseValues.count(feature_sign)) {
      ForceFeature(feature_name, false);
    } else {
      LOG(WARNING) << "Could not parse " << temp << ", invalid sign!";
      continue;
    }
    VLOG(1) << "Feature parsed " << feature_name;
  }
}

bool Features::IsFeatureEnabled(std::string_view feature) {
  if (Get().features_.find(feature) != Get().features_.end()) {
    return Get().features_[feature];
  }
  return false;
}
void Features::ForceFeature(std::string_view feature, bool status) {
  Get().features_[feature] = status;
}

Features& Features::Get() {
  static Features feature_;
  return feature_;
}
