#include <MEP-3D/pyramid.hpp>
#include <MEP-3D/common.hpp>
#include<glm/gtc/type_ptr.hpp>

Pyramid::Pyramid(const std::vector<GLfloat>& vertices) {
	Mesh::Init(vertices, {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
		});
}

void Pyramid::Draw(RenderTarget& render_target) {
	if (!Get<Shader>()) {
		return;
	}
	Shader* shader = Get<Shader>();
	Update();
	shader->SetUniformFromMemory(CommonUniform::Model, GetModel());
	if (render_target.GetView())
		shader->SetUniformFromMemory(CommonUniform::Projection, render_target.GetView()->GetProjection());
	if (render_target.GetCamera())
		shader->SetUniformFromMemory(CommonUniform::View, render_target.GetCamera()->GetViewMatrix());
	
	Texture* texture = nullptr;
	if ((texture = Get<Texture>()))
		texture->Use();

	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
	glDrawElements(GL_TRIANGLES, GetVerticesCount(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	if (texture)
		texture->StopUsing();
}

std::string Pyramid::ToString() const
{
	return AssetController::ToString() + ",\n" + ObjectActionController::ToString();
}
