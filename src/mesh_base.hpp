#ifndef MESH_BASE_HPP
#define MESH_BASE_HPP

class MeshBase {
public:
	MeshBase() {
		vertex_buffer_object_ = 0;
		vertex_array_object_ = 0;
	}

	void Clear() {
		if (vertex_array_object_ != 0) {
			glDeleteVertexArrays(1, &vertex_array_object_);
			vertex_array_object_ = 0;
		}
		if (vertex_buffer_object_ != 0) {
			glDeleteBuffers(1, &vertex_buffer_object_);
			vertex_buffer_object_ = 0;
		}
	}
	virtual ~MeshBase() {
		Clear();
	}
protected:
	GLuint vertex_buffer_object_;
	GLuint vertex_array_object_;
};

#endif
