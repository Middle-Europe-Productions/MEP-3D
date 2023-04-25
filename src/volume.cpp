#include <MEP-3D/volume.hpp>

#include <MEP-3D/file_reader_with_progress.hpp>
#include <MEP-3D/thread_pool.hpp>

namespace mep {
namespace {
constexpr int kChunkSize = 1024;
constexpr long long kKiloByte = 1024;

// unit cube indices
const std::vector<unsigned int> cubeIndices = {
    0, 5, 4, 5, 0, 1, 3, 7, 6, 3, 6, 2, 7, 4, 6, 6, 4, 5,
    2, 1, 3, 3, 1, 0, 3, 0, 7, 7, 0, 4, 6, 5, 2, 2, 5, 1};

Vec3f BringToBase(const Vec3i& value) {
  return Vec3f(static_cast<float>(value.x_) / static_cast<float>(value.Max()),
               static_cast<float>(value.y_) / static_cast<float>(value.Max()),
               static_cast<float>(value.z_) / static_cast<float>(value.Max()));
}
};  // namespace

Volume::Volume(Vec3i size) : Identity(kVolume), volume_size_(size) {
  active_.store(true, std::memory_order_release);
  auto base = BringToBase(size);
  const std::vector<float> vertices2 = {
      -base.x_ / 2, -base.y_ / 2, -base.z_ / 2, base.x_ / 2,  -base.y_ / 2,
      -base.z_ / 2, base.x_ / 2,  base.y_ / 2,  -base.z_ / 2, -base.x_ / 2,
      base.y_ / 2,  -base.z_ / 2, -base.x_ / 2, -base.y_ / 2, base.z_ / 2,
      base.x_ / 2,  -base.y_ / 2, base.z_ / 2,  base.x_ / 2,  base.y_ / 2,
      base.z_ / 2,  -base.x_ / 2, base.y_ / 2,  base.z_ / 2};
  Mesh::Init(vertices2, cubeIndices, true);
}

Volume::Volume(Vec3i size, std::string_view name)
    : Identity(kVolume, name), volume_size_(size) {
  active_.store(true, std::memory_order_release);
  auto base = BringToBase(size);
  const std::vector<float> vertices2 = {
      -base.x_ / 2, -base.y_ / 2, -base.z_ / 2, base.x_ / 2,  -base.y_ / 2,
      -base.z_ / 2, base.x_ / 2,  base.y_ / 2,  -base.z_ / 2, -base.x_ / 2,
      base.y_ / 2,  -base.z_ / 2, -base.x_ / 2, -base.y_ / 2, base.z_ / 2,
      base.x_ / 2,  -base.y_ / 2, base.z_ / 2,  base.x_ / 2,  base.y_ / 2,
      base.z_ / 2,  -base.x_ / 2, base.y_ / 2,  base.z_ / 2};
  Mesh::Init(vertices2, cubeIndices, true);
}

void Volume::LoadFromFile(std::filesystem::path file_path,
                          Texture3D::Type type) {
  type_ = type;
  UpdateStatus(Status::Waiting);
  ThreadPool::PostTaskWithCallback(
      Executors::Resource,
      std::make_unique<TaskWithCallback>(
          [this, file_path]() -> bool {
            FileReaderWithProgress infile(file_path, std::ios::binary);
            if (!infile.is_open()) {
              LOG(ERROR) << "Could not load the file " << file_path;
              return false;
            }
            DCHECK(infile.GetSize() > 0);
            std::vector<Uint8> chunk(kChunkSize, 0);
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
              infile.read(reinterpret_cast<char*>(chunk.data()), kChunkSize);
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
      TransformToTexture();
    } else {
      return;
    }
  }
  Mesh::Draw(render_target);
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

Texture3D::Type Volume::GetType() const {
  return type_;
}

std::string Volume::ToString() const {
  return "\"volume\": {" + Identity::ToString() + ", \n" +
         AssetController::ToString() + ", \n" + ModelController::ToString() +
         "}";
}

void Volume::ClearVolume() {
  data_.clear();
}

void Volume::TransformToTexture() {
  DCHECK(GetStatus() == Status::Uninitialized);
  DCHECK(data_.size() > 0);
  texture_ = std::make_unique<Texture3D>();
  texture_->Create(data_.data(), volume_size_, type_);
  Bind(texture_.get());
  DCHECK(texture_->IsValid());
  ClearVolume();
  UpdateStatus(Status::Avalible);
}

Volume::~Volume() {
  active_.store(false, std::memory_order_release);
  WaitForResource();
}
}  // namespace mep
