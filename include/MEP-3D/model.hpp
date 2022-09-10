#ifndef MODEL_HPP
#define MODEL_HPP

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/drawable.hpp>
#include <MEP-3D/mesh_base.hpp>
#include <MEP-3D/model_controller.hpp>
#include <MEP-3D/resource_base.hpp>
#include <MEP-3D/texture.hpp>

class Model : public Identity,
              public Drawable,
              public ModelController,
              public AssetController,
              public ResourceBase {
 public:
  Model();
  Model(const std::string& name);
  void Load(const std::string& file_path);
  void Init();
  virtual void Draw(RenderTarget& render_target) override;
  virtual std::string ToString() const override;
  void Clear();

 private:
  std::vector<MeshBaseFactoryPtr> master_mesh_factory_;
  std::vector<MeshBasePtr> mesh_container_;
  std::vector<TexturePtr> textures_container_;
  std::vector<unsigned int> mesh_to_texture_;
  bool status;
};

using ModelPtr = std::unique_ptr<Model>;

#endif