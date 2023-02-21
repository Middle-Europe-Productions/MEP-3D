#include <MEP-3D/features.hpp>

#include <vector>

namespace mep {
namespace feature {
#define MEP_FEATURE(var_name, var_value, default_val) \
  const char* var_name = var_value;
#include <MEP-3D/mep_features.inl>
#undef MEP_FEATURE

namespace {
struct FeatureBody {
  std::string_view name;
  bool state;
  bool default_state;
};

using FeatureBodyArr = std::vector<FeatureBody>;

FeatureBodyArr kFeaturesList = {
#define MEP_FEATURE(var_name, var_value, default_val) \
  {var_value, default_val, default_val},
#include <MEP-3D/mep_features.inl>
#undef MEP_FEATURE
};

FeatureBodyArr::iterator GetFeatureBodyIt(std::string_view feature) {
  return std::find_if(kFeaturesList.begin(), kFeaturesList.end(),
                      [feature](const auto& it) { return feature == it.name; });
}

bool Exists(FeatureBodyArr::iterator it) {
  return kFeaturesList.end() != it;
}

}  // namespace

bool IsFeatureEnabled(std::string_view feature) {
  auto feature_it = GetFeatureBodyIt(feature);
  if (Exists(feature_it)) {
    return feature_it->state;
  }
  return false;
}
void ForceFeature(std::string_view feature, bool status) {
  auto feature_it = GetFeatureBodyIt(feature);
  if (Exists(feature_it)) {
    feature_it->state = status;
  }
}

bool GetFeatureDefaultState(std::string_view feature) {
  auto feature_it = GetFeatureBodyIt(feature);
  if (Exists(feature_it)) {
    return feature_it->default_state;
  }
  return false;
}

};  // namespace feature
};  // namespace mep
