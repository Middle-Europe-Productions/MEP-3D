#ifndef THREE_DIM_MESH_HPP
#define THREE_DIM_MESH_HPP

#include <GL/glew.h>
#include <glog/logging.h>

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/drawable.hpp>
#include <MEP-3D/mesh_base.hpp>
#include <MEP-3D/model_controller.hpp>

class Mesh : public MeshBase,
             public Drawable,
             public AssetController,
             public ModelController {
 public:
  Mesh();
  virtual void Clear();
  virtual void Draw(RenderTarget& render_target);
  std::string ToString() const;
};

using MeshPtr = std::unique_ptr<Mesh>;

#endif