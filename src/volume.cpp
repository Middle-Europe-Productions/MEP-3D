#include <MEP-3D/volume.hpp>

#include <MEP-3D/file_reader_with_progress.hpp>
#include <MEP-3D/thread_pool.hpp>

namespace {
constexpr int kChunkSize = 1024;
constexpr long long kKiloByte = 1024;
};  // namespace

Volume::Volume() : Identity(kVolume) {
  active_.store(true, std::memory_order_release);
}

void Volume::LoadFromFile(const std::string_view file_path,
                          Vec3i size,
                          Texture3D::Type type) {
  type_ = type;
  volume_size_ = size;
  UpdateStatus(Status::Waiting);
  ThreadPool::PostTaskWithCallback(
      Executors::Resource,
      std::make_unique<TaskWithCallback>(
          [this, file_path]() -> bool {
            FileReaderWithProgress infile(file_path.data(), std::ios::binary);
            if (!infile.is_open()) {
              LOG(ERROR) << "Could not load the file " << file_path;
              return false;
            }
            DCHECK(infile.GetSize() > 0);
            std::vector<char> chunk(kChunkSize, 0);
            this->UpdateStatus(Status::Loading);
            try {
              data_.reserve(infile.GetSize());
            } catch (std::exception e) {
              LOG(ERROR) << "Unable to allocate memory: " << e.what();
              return false;
            }
            long long rea_c = 0;
            while (!infile.eof()) {
              if (!this->IsActive()) {
                return false;
              }
              infile.read(chunk.data(), kChunkSize);
              rea_c = infile.gcount();
              data_.insert(data_.end(), chunk.data(), chunk.data() + rea_c);
              this->SetProgress(infile.GetProgress());
              this->size_bytes_ += rea_c;
            }
            return true;
          },
          [this](bool status) {
            if (status) {
              this->UpdateStatus(Status::Uninitialized);
            } else {
              this->UpdateStatus(Status::Failed);
            }
          }));
}

void Volume::Draw(RenderTarget& render_target) {
  if (GetStatus() != Status::Avalible) {
    if (GetStatus() == Status::Uninitialized) {
      Init();
    } else {
      return;
    }
  }
  if (!Get<Shader>()) {
    return;
  }
  texture_->Use();
}

Uint8* Volume::GetData() {
  if (GetStatus() != Status::Uninitialized) {
    return nullptr;
  }
  return data_.data();
}

const Uint8* Volume::GetData() const {
  if (GetStatus() != Status::Uninitialized) {
    return nullptr;
  }
  return data_.data();
}

bool Volume::IsActive() const {
  return active_.load(std::memory_order_acquire);
}

const Vec3i& Volume::GetDimensions() const {
  return volume_size_;
}

long long Volume::GetSizeInKB() const {
  return size_bytes_.load() / kKiloByte;
}

const Texture3D::Type Volume::GetType() const {
  return type_;
}

void Volume::ClearVolume() {
  data_.clear();
}

void Volume::Init() {
  DCHECK(GetStatus() == Status::Uninitialized);
  DCHECK(data_.size() > 0);
  texture_ = std::make_unique<Texture3D>();
  texture_->Create(data_.data(), volume_size_, type_);
  DCHECK(texture_->IsValid());
}

Volume::~Volume() {
  active_.store(false, std::memory_order_release);
  WaitForResource();
}
