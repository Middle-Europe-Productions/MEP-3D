#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include<MEP-3D/image.hpp>
#include<GL/glew.h>

class Texture {
public:
    Texture();
    void Create(Image& image);
    void Use();
    void Clear();
    ~Texture();

private:
    GLuint texture_id_;
};

#endif