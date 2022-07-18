#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include<MEP-3D/image.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/observer_list.hpp>
#include<MEP-3D/asset.hpp>
#include<GL/glew.h>

class Texture: public Asset {
public:
    Texture();
    void Create(Image& image);
    void Use();
    void Stop();
    void Clear();
    virtual ~Texture();

private:
    GLuint texture_id_;
};

#endif