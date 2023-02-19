#ifndef ASSET_OBSERVER_HPP
#define ASSET_OBSERVER_HPP

#include <MEP-3D/identity.hpp>
#include <MEP-3D/observer.hpp>

class AssetObserver : public Observer {
 public:
  virtual void OnDelete(Identity& caller_identity) = 0;
  virtual ~AssetObserver() = default;
};

#endif
