#include<MEP-3D/utils.hpp>

glm::vec3 utils::AxisToGlm(Axis axis) {
	switch (axis) {
	case Axis::X:
		return glm::vec3(1.0f, 0.0f, 0.0f);
	case Axis::Y:
		return glm::vec3(0.0f, 1.0f, 0.0f);
	case Axis::Z:
		return glm::vec3(0.0f, 0.0f, 1.0f);
	}
}
