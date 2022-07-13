#include <MEP-3D/mesh_base.hpp>

MeshBase::MeshBase() {
	vertex_buffer_object_ = 0;
	vertex_array_object_ = 0;
	index_buffer_boject_ = 0;
}

void MeshBase::Clear() {
	if (vertex_array_object_ != 0) {
		glDeleteVertexArrays(1, &vertex_array_object_);
		vertex_array_object_ = 0;
	}
	if (vertex_buffer_object_ != 0) {
		glDeleteBuffers(1, &vertex_buffer_object_);
		vertex_buffer_object_ = 0;
	}
	if (index_buffer_boject_ != 0) {
		glDeleteBuffers(1, &index_buffer_boject_);
		index_buffer_boject_ = 0;
	}
}
MeshBase::~MeshBase() {
	Clear();
}