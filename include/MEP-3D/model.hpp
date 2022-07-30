#ifndef MODEL_HPP
#define MODEL_HPP

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/drawable.hpp>
#include <MEP-3D/mesh_base.hpp>
#include <MEP-3D/model_controller.hpp>
#include <MEP-3D/texture.hpp>

class Model : public Drawable, public ModelController, public AssetController {
 public:
  Model();
  void Load(const std::string& file_path);
  void Draw(RenderTarget& render_target) override;
  void Clear();

 private:
  std::vector<MeshBasePtr> mesh_container_;
  std::vector<TexturePtr> textures_container_;
  std::vector<unsigned int> mesh_to_texture_;
  bool status;
};

#endif