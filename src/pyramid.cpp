#include <MEP-3D/pyramid.hpp>

Pyramid::Pyramid(const std::vector<GLfloat>& vertices) {
	ThreeDimMesh::Init(vertices, {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
		});
}

void Pyramid::Draw(RenderTarget& render_target) {
	if (!Get()) {
		return;
	}
	ShaderBase* shader = Get();
	Update();
	glUniformMatrix4fv(shader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(GetModel()));
	if (render_target.GetView())
		glUniformMatrix4fv(shader->GetProjectionLocation(), 1, GL_FALSE,
			glm::value_ptr(render_target.GetView()->GetProjection()));
	if (render_target.GetCamera())
		glUniformMatrix4fv(shader->GetViewLocation(), 1, GL_FALSE,
			glm::value_ptr(render_target.GetCamera()->GetViewMatrix()));

	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
