#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include<MEP-3D/image.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/observer_list.hpp>
#include<MEP-3D/identity.hpp>
#include<GL/glew.h>

class Texture: public Identity, public ObserverList<AssetObserver> {
public:
    Texture();
    void Create(Image& image);
    void Use();
    void StopUsing();
    void Clear();
    ~Texture();

private:
    GLuint texture_id_;
};

#endif