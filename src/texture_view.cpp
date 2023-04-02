
#include <MEP-3D/texture_view.hpp>

#include <GL/glew.h>

namespace mep {

TextureView::TextureView() : handler_(0) {}

TextureView::TextureView(unsigned int handler) : handler_(handler) {}

void TextureView::Use(unsigned int slot) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, handler_);
}
void TextureView::Stop() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureView::Clear() {
  handler_ = 0;
}

void TextureView::SetHandler(unsigned int handler) {
  handler_ = handler;
}

unsigned int TextureView::GetHandler() {
  return handler_;
}

TextureView::~TextureView() {
  Clear();
}

};  // namespace mep
