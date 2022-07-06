#ifndef TWO_DIM_MESH_HPP
#define TWO_DIM_MESH_HPP

#include <GL/glew.h>
#include <vector>

#include "drawable.hpp"
#include "mesh_base.hpp"

class TwoDimMesh: public Drawable, public MeshBase {
public:
	virtual void Init(const std::vector<GLfloat>& vertices) {
		glGenVertexArrays(1, &vertex_array_object_);
		glBindVertexArray(vertex_array_object_);

		glGenBuffers(1, &vertex_buffer_object_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	virtual ~TwoDimMesh() = default;
};

#endif