#ifndef COMMON_HPP
#define COMMON_HPP

#include<iostream>

using Uint8 = unsigned char;

enum class Axis { X, Y, Z };

enum CommonUniform : unsigned int {
	Projection = 10000,
	Model = 10001,
	View
};

std::ostream& operator<<(std::ostream& out, Axis axis);

#endif