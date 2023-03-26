
template <typename LightPtr>
LightController<LightPtr>::LightController(
    unsigned int max_point_light,
    const std::string& struct_name,
    GLint light_count_location,
    std::unordered_map<LightUniforms, std::string> uniform_map)
    : uniform_map_(uniform_map),
      struct_name_(struct_name),
      light_count_location_(light_count_location),
      max_size_(max_point_light) {}

template <typename LightPtr>
typename std::vector<LightPtr>::iterator LightController<LightPtr>::MakeAndBind(
    LightPtr light_ptr) {
  if (point_light_container_.size() >= max_size_) {
    LOG(INFO) << "Reached maximal number of point lights (" << max_size_ << ")";
    return point_light_container_.end();
  }
  if (!Get<Shader>()) {
    LOG(ERROR) << "You did not bind shader to " << __FUNCTION__;
    return point_light_container_.end();
  }
  point_light_container_.emplace_back(std::move(light_ptr));
  point_light_container_.back()->BindUniforms(
      *Get<Shader>(),
      static_cast<unsigned int>(point_light_container_.size() - 1),
      struct_name_, uniform_map_);
  return point_light_container_.end() - 1;
}

template <typename LightPtr>
bool LightController<LightPtr>::IsValid(
    const typename std::vector<LightPtr>::iterator& iter) const {
  return iter != point_light_container_.end();
}
template <typename LightPtr>
void LightController<LightPtr>::Remove(const Identity& id) {
  LOG(INFO) << __FUNCTION__ << ", " << id.ToString();
  point_light_container_.erase(
      std::remove_if(
          point_light_container_.begin(), point_light_container_.end(),
          [&id](const auto& in) { return in->GetId() == id.GetId(); }),
      point_light_container_.end());
}

template <typename LightPtr>
void LightController<LightPtr>::ForAll(
    std::function<void(LightPtr&)> function) {
  for (auto& light : point_light_container_) {
    function(light);
  }
}

template <typename LightPtr>
void LightController<LightPtr>::Use() {
  glUniform1i(light_count_location_,
              static_cast<GLint>(point_light_container_.size()));
  for (auto& light : point_light_container_) {
    light->Use();
  }
}

template <typename LightPtr>
const std::vector<LightPtr>& LightController<LightPtr>::GetContainer() const {
  return point_light_container_;
}

template <typename LightPtr>
LightPtr& LightController<LightPtr>::operator[](std::size_t element) {
  return point_light_container_[element];
}

template <typename LightPtr>
const LightPtr& LightController<LightPtr>::operator[](
    std::size_t element) const {
  return point_light_container_[element];
}