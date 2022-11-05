#ifndef FEATURES_HPP
#define FEATURES_HPP

#include <unordered_map>
#include <string>

#include <MEP-3D/mep_features.hpp>

class Features {
public:
    static bool Init(int argc, char* argv[]);
    static bool IsFeatureEnabled(std::string_view feature);
    static void ForceFeature(std::string_view feature, bool status);
private:
    Features() = default;
    static Features& Get();
    std::unordered_map<std::string_view, bool> features_;
};

#endif 