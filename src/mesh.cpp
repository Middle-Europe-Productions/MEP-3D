#include <MEP-3D/mesh.hpp>

Mesh::Mesh() {
  LOG(INFO) << __FUNCTION__;
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
  Update();
  shader->SetUniformFromMemory(static_cast<unsigned int>(CommonUniform::Model),
                               GetModel());
  if (render_target.GetView())
    shader->SetUniformFromMemory(
        static_cast<unsigned int>(CommonUniform::Projection),
        render_target.GetView()->GetProjection());
  if (render_target.GetCamera()) {
    shader->SetUniformFromMemory(static_cast<unsigned int>(CommonUniform::View),
                                 render_target.GetCamera()->GetViewMatrix());
    shader->SetUniformFromMemory(
        static_cast<unsigned int>(CommonUniform::Position),
        render_target.GetCamera()->GetPosition());
  }

  Texture* texture = nullptr;
  if ((texture = Get<Texture>()))
    texture->Use();

  MeshBase::Draw(render_target);

  if (texture)
    texture->Stop();
}

std::string Mesh::ToString() const {
  return "Mesh: {\n" + AssetController::ToString() + ",\n" +
         ModelController::ToString() + "\n}";
}
