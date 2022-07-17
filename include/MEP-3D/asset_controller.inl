
template<typename What>
What* AssetController::Get() const {
    if (std::is_same<Texture, What>::value) {
        return dynamic_cast<What*>(texture_);
    } else if (std::is_same<Shader, What>::value) {
        return dynamic_cast<What*>(shader_);
    }
    LOG(INFO) << "Unknown asset!";
    return nullptr;
}
