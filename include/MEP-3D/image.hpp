#ifndef IMAGE_HPP
#define IMAGE_HPP

#include<string>
#include<vector>
#include<MEP-3D/vector.hpp>
#include<MEP-3D/color.hpp>

class Image {
public:
    enum Type {
        RGBA,
        RGB,
        Unknown
    };
    Image();
    bool LoadFromFile(const std::string& name);
    Uint8* GetPixels();
    Type GetType() const;
    const Vec2i& GetSize() const;
    void Clear();

private:
    std::vector<Uint8> image_data_;
    Vec2i size_;
    int bit_depth_;
    Type image_type_;

};
#endif