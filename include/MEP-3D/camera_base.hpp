#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include <glm/glm.hpp>

#include <MEP-3D/common_names.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/observable_map.hpp>
#include <MEP-3D/updatable.hpp>
#include <MEP-3D/utils.hpp>
#include <MEP-3D/window_observer.hpp>

#include <array>

enum class CameraVariables { Position, Direction };

std::string ToString(const CameraVariables& camera_variable);

using CameraObserver = MapObserver<glm::vec3, CameraVariables>;

class CameraBase : public Identity,
                   public Updatable,
                   public WindowObserver,
                   public ObservableMap<glm::vec3, CameraVariables> {
 public:
  CameraBase(std::string_view identity);
  CameraBase(std::string_view identity, std::string_view name);
  void OnKeyEvent(KeyEvent event) override;
  void DisableEvents();
  void EnableEvents();
  bool IsDisabled() const;
  virtual glm::mat4 GetViewMatrix() const = 0;
  virtual glm::vec3 GetPosition() const;
  virtual glm::vec3 GetNormalizedDirection() const;

 protected:
  bool ContainsKey(Key key) const;
  void ResetCameraControls();

  template <typename Actions>
  bool IsActive(Actions pca);

  template <typename Actions, std::size_t SIZE>
  void InitCameraControls(Actions terminate_action,
                          const std::array<Key, SIZE>& controls);
  std::unordered_map<Key, bool> key_status_;
  std::vector<Key> controls_;

 private:
  bool disabled_ = true;
};

template <typename Actions>
bool CameraBase::IsActive(Actions pca) {
  DCHECK(!controls_.empty());
  return key_status_[controls_[static_cast<int>(pca)]];
}

template <typename Actions, std::size_t SIZE>
void CameraBase::InitCameraControls(Actions terminate_action,
                                    const std::array<Key, SIZE>& controls) {
  DCHECK(!controls.empty());
  controls_.resize(controls.size());
  for (int i = 0; i < controls.size(); i++) {
    controls_[i] = controls[i];
  }
  if (controls_.size() != static_cast<int>(terminate_action)) {
    LOG(WARNING) << "Not all camera controls match camera actions";
  }
  for (Actions it = static_cast<Actions>(0); it != terminate_action;
       it = utils::IncEnum(it)) {
    key_status_[controls_[static_cast<int>(it)]] = false;
  }
}

#endif