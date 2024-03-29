#ifndef VOLUME_HPP
#define VOLUME_HPP

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/async_progress.hpp>
#include <MEP-3D/drawable.hpp>
#include <MEP-3D/figures.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/resource_base.hpp>
#include <MEP-3D/texture_3d.hpp>

#include <filesystem>

namespace mep {
class Volume : public Identity,
               public Mesh,
               public ResourceBase,
               public AsyncProgress {
 public:
  Volume(Vec3i size, bool keep_volume_cached = false);
  Volume(Vec3i size, std::string_view name, bool keep_volume_cached = false);
  void UpdateVolumeFromMemory();
  void LoadFromMemory(std::vector<Uint8> data,
                      Texture3D::Type type = Texture3D::Type::BYTE_8);
  void LoadFromFile(std::filesystem::path file_path,
                    Texture3D::Type type = Texture3D::Type::BYTE_8);
  void Draw(RenderTarget& render_target) override;
  std::vector<Uint8>& GetData();
  const std::vector<Uint8>& GetData() const;
  bool IsActive() const;
  const Vec3i& GetDimensions() const;
  long long GetSizeInKB() const;
  Texture3D::Type GetType() const;
  virtual std::string ToString() const override;
  void ClearVolume();
  ~Volume();

 private:
  void TransformToTexture();
  Texture3D::Type type_;
  Vec3i volume_size_;
  bool keep_cached_;
  std::vector<Uint8> data_;
  std::unique_ptr<Texture3D> texture_;
  std::atomic<bool> active_;
  std::atomic<long long> size_bytes_;
};
}  // namespace mep

#endif
