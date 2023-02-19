#include <MEP-3D/mesh.hpp>

Mesh::Mesh() {
  VLOG(3) << __FUNCTION__;
  vertices_count_ = 0;
}

void Mesh::Clear() {
  MeshBase::Clear();
  vertices_count_ = 0;
}

void Mesh::Draw(RenderTarget& render_target) {
  if (!Get<Shader>()) {
    return;
  }
  Shader* shader = Get<Shader>();
  shader->SetUniform(static_cast<unsigned int>(CommonUniform::Model),
                     GetModel());
  if (render_target.GetView())
    shader->SetUniform(static_cast<unsigned int>(CommonUniform::Projection),
                       render_target.GetView()->GetProjection());
  if (render_target.GetCamera()) {
    shader->SetUniform(static_cast<unsigned int>(CommonUniform::View),
                       render_target.GetCamera()->GetViewMatrix());
    shader->SetUniform(static_cast<unsigned int>(CommonUniform::Position),
                       render_target.GetCamera()->GetPosition());
  }

  TextureBase* texture = nullptr;
  if ((texture = Get<TextureBase>()))
    texture->Use();

  MeshBase::Draw(render_target);

  if (texture)
    texture->Stop();
}

std::string Mesh::ToString() const {
  return "\"Mesh\": {\n" + AssetController::ToString() + ",\n" +
         ModelController::ToString() + "\n}";
}
