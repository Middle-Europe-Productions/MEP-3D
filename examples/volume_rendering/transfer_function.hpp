#ifndef TRANSFER_FUNCTION_HPP
#define TRANSFER_FUNCTION_HPP

#include <MEP-3D/template/ui_element.hpp>

namespace tf {
enum VolumeRenderingHandlers : int { TransferFunction = UI_ELEMENT_COUNT + 1 };
void Register();
}  // namespace tf

#endif