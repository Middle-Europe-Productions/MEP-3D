#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <glog/logging.h>

class Drawable {
public:
	virtual void Draw() = 0;
	virtual ~Drawable() = default;
};

#endif