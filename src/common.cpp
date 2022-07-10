#include<MEP-3D/common.hpp>

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