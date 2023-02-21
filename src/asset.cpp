#include <MEP-3D/asset.hpp>

namespace mep {
Asset::~Asset() {
  LOG(INFO) << __FUNCTION__ << ", " << Identity::ToString();
  ForAllObservers(
      [this](AssetObserver* observer) { observer->OnDelete(*this); });
}
}  // namespace mep
