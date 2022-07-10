#ifndef COMMON_HPP
#define COMMON_HPP

#include<iostream>

enum class Axis { X, Y, Z };

std::ostream& operator<<(std::ostream& out, Axis axis);

#endif