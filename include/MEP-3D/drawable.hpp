#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include<glog/logging.h>

#include<MEP-3D/render_target.hpp>

class Drawable {
public:
	virtual void Draw(RenderTarget& render_target) = 0;
	virtual ~Drawable() = default;
};

#endif