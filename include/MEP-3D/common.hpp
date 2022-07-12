#ifndef COMMON_HPP
#define COMMON_HPP

#include<iostream>

using Uint8 = unsigned char;

enum class Axis { X, Y, Z };

std::ostream& operator<<(std::ostream& out, Axis axis);

#endif