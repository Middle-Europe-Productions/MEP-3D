
namespace mep {
template <typename What>
What* AssetController::Get() const {
  if (std::is_same<TextureBase, What>::value) {
    return dynamic_cast<What*>(texture_);
  } else if (std::is_same<Shader, What>::value) {
    return dynamic_cast<What*>(shader_);
  } else if (std::is_same<Material, What>::value) {
    return dynamic_cast<What*>(material_);
  }
  LOG(INFO) << "Unknown asset!";
  return nullptr;
}
}  // namespace mep
