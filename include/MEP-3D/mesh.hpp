#ifndef THREE_DIM_MESH_HPP
#define THREE_DIM_MESH_HPP

#include<GL/glew.h>
#include<glog/logging.h>

#include<MEP-3D/mesh_base.hpp>
#include<MEP-3D/drawable.hpp>
#include<MEP-3D/asset_controller.hpp>
#include<MEP-3D/model_controller.hpp>

class Mesh : public MeshBase, 
			 public Drawable, 
			 public AssetController, 
			 public ModelController {
public:
	Mesh();
	virtual void Clear();
	virtual void Init(const std::vector<GLfloat>& vertices, const std::vector<unsigned int> indices);
	virtual void Draw(RenderTarget& render_target);
	unsigned int GetVerticesCount() const;
	std::string ToString() const;
	virtual ~Mesh() = default;

protected:
	unsigned int vertices_count_;

};

#endif