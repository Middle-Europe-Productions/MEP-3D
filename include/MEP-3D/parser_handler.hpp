#ifndef PARSER_HANDLER_HPP
#define PARSER_HANDLER_HPP

#include <MEP-3D/scene.hpp>

namespace mep {
class ParserHandler {
 public:
  virtual void SetUp(mep::Scene* scene) = 0;
  virtual void TearDown() = 0;
  virtual void Draw() = 0;
  virtual ~ParserHandler() = default;
};
}  // namespace mep

#endif