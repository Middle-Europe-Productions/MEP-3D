#ifndef TWO_DIM_MESH_HPP
#define TWO_DIM_MESH_HPP

#include<GL/glew.h>
#include<vector>

#include<MEP-3D/drawable.hpp>
#include<MEP-3D/mesh_base.hpp>

class TwoDimMesh: public Drawable, public MeshBase {
public:
	virtual void Init(const std::vector<GLfloat>& vertices);
	virtual ~TwoDimMesh() = default;
};

#endif