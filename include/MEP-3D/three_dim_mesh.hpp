#ifndef THREE_DIM_MESH_HPP
#define THREE_DIM_MESH_HPP

#include<GL/glew.h>
#include<glog/logging.h>

#include<MEP-3D/two_dim_mesh.hpp>

class ThreeDimMesh : public TwoDimMesh {
public:
	ThreeDimMesh();
	virtual void Clear();
	virtual void Init(const std::vector<GLfloat>& vertices, const std::vector<unsigned int> indices);
	virtual ~ThreeDimMesh() = default;

protected:
	GLuint index_buffer_boject_;

};

#endif