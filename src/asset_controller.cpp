#include<MEP-3D/asset_controller.hpp>
#include<glog/logging.h>

AssetController::AssetController() : texture_(nullptr), shader_(nullptr) {}

void AssetController::Bind(Texture* object) {
    if (texture_) {
        texture_->RemoveObserver(this);
    }
    texture_ = object;
    texture_->AddObserver(this);
}

void AssetController::Bind(Shader* object) {
    if (shader_) {
        shader_->RemoveObserver(this);
    }
    shader_ = object;
    shader_->AddObserver(this);
}

void AssetController::OnDelete(Identity& caller_identity) {
    LOG(INFO) << __FUNCTION__ << ", caller_id: " << caller_identity.ToString();
    if (texture_ && texture_->GetGlobalId() == caller_identity.GetGlobalId()) {
        texture_ = nullptr;
    }
    else if (shader_ && shader_->GetGlobalId() == caller_identity.GetGlobalId()) {
        shader_ = nullptr;
    }
}

void AssetController::RemoveAssets() {
    LOG(INFO) << __FUNCTION__ << ", observer_id: " << GetObsserverId();
    if (texture_) {
        LOG(INFO) << texture_->ToString();
        texture_->RemoveObserver(this);
        texture_ = nullptr;
    }
    if (shader_) {
        LOG(INFO) << shader_->ToString();
        shader_->RemoveObserver(this);
        shader_ = nullptr;
    }
}

std::string AssetController::ToString() const
{
    std::string tex_temp = (texture_ ? "texture_id: " + texture_->ToString() : "");
    std::string sh_temp = (texture_ ? "shader_id: " + texture_->ToString() : "");
    return "AssetController: { \n" + tex_temp +
        (tex_temp.size() != 0 ? ", " + sh_temp : sh_temp) +
        "\n}";
}

AssetController::~AssetController() {
    RemoveAssets();
}