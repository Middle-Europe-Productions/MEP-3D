#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

enum class Axis { X, Y, Z };

std::ostream& operator<<(std::ostream& out, Axis axis) {
	switch (axis) {
	case Axis::X:
		out << "X";
	case Axis::Y:
		out << "Y";
	case Axis::Z:
		out << "Z";
	}
	return out;
}

#endif