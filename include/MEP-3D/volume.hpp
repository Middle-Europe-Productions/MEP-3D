#ifndef VOLUME_HPP
#define VOLUME_HPP

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/async_progress.hpp>
#include <MEP-3D/drawable.hpp>
#include <MEP-3D/identity.hpp>
#include <MEP-3D/resource_base.hpp>
#include <MEP-3D/texture_3d.hpp>

class Volume : public Identity,
               public Drawable,
               public ResourceBase,
               public AsyncProgress,
               public AssetController {
 public:
  Volume() = default;
  void LoadFromFile(const std::string_view file_path,
                    Vec3i size,
                    Texture3D::Type type = Texture3D::Type::BYTE_8);
  void Draw(RenderTarget& render_target);
  Uint8* GetData();
  const Uint8* GetData() const;
  const Vec3i& GetSize() const;
  const Texture3D::Type GetType() const;
  void ClearVolume();

 private:
  void Init();
  Texture3D::Type type_;
  Vec3i volume_size_;
  std::vector<Uint8> data_;
  std::unique_ptr<Texture3D> texture_;
};

#endif
