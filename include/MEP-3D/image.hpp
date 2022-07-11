#ifndef IMAGE_HPP
#define IMAGE_HPP

#include<string>
#include<vector>

using Uint8 = unsigned char;
using Uint32 = unsigned int;

class Image {
public:
    Image();
    bool LoadFromFile(const std::string& name);
    Uint8* GetPixels();

private:
    std::vector<Uint8> image_data_;
    Uint32 width_;
    Uint32 height_;
    Uint32 bit_depth_;

};
#endif