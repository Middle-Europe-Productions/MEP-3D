#ifndef MESH_BASE_HPP
#define MESH_BASE_HPP

#include<GL/glew.h>

class MeshBase {
public:
	MeshBase();
	void Clear();
	virtual ~MeshBase();

protected:
	GLuint vertex_buffer_object_;
	GLuint vertex_array_object_;
	GLuint index_buffer_boject_;

};

#endif
