#ifndef FEATURES_HPP
#define FEATURES_HPP

#include <string>

namespace feature {
#define MEP_FEATURE(var_name, var_value, default_val) \
extern const char* var_name;
#include <MEP-3D/mep_features.inl>
#undef MEP_FEATURE

bool IsFeatureEnabled(std::string_view feature);
void ForceFeature(std::string_view feature, bool status);
};

#endif