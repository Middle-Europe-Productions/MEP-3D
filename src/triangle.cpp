#include<MEP-3D/triangle.hpp>

Triangle::Triangle(const std::vector<GLfloat>& vertices) {
	Init(vertices, {});
}

void Triangle::Draw(RenderTarget& render_target) {
	if (!Get()) {
		return;
	}
	ShaderBase* shader = Get();
	//TODO: Implement caching
	glm::mat4 model(1.0f);
	Update();
	shader->SetUniformExt(shader->GetModelLocation(), model);
	if (render_target.GetView())
		shader->SetUniformExt(shader->GetProjectionLocation(), render_target.GetView()->GetProjection());
	if (render_target.GetCamera())
		shader->SetUniformExt(shader->GetViewLocation(), render_target.GetCamera()->GetViewMatrix());

	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
