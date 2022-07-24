#ifndef ASSET_HPP
#define ASSET_HPP

#include <MEP-3D/asset_observer.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/observer_list.hpp>

class Asset : public Identity, public ObserverList<AssetObserver> {
 public:
  Asset(const char* identity) : Identity(identity) {}
  virtual void Use() = 0;
  virtual void Stop() = 0;
  virtual ~Asset();
};

#endif