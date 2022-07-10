#include<MEP-3D/three_dim_mesh.hpp>


ThreeDimMesh::ThreeDimMesh() {
	index_buffer_boject_ = 0;
}

void ThreeDimMesh::Clear() {
	MeshBase::Clear();
	if (index_buffer_boject_ != 0) {
		glDeleteBuffers(1, &index_buffer_boject_);
		index_buffer_boject_ = 0;
	}
}

void ThreeDimMesh::Init(const std::vector<GLfloat>& vertices, const std::vector<unsigned int> indices) {
	TwoDimMesh::Init(vertices);
	glGenBuffers(1, &index_buffer_boject_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}