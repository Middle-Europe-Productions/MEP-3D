#include <iostream>
#ifdef _WIN32
#define WINPAUSE system("pause")
#endif

#include <MEP-3D/mep-3d.hpp>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  // Creating reader with display of length 100 (100 #'s)

  Volume volume;
  volume.LoadFromFile("beechnut_1024x1024x1546_uint16.raw", {256, 256, 256});
  int progress = 0;
  RenderTarget target;
  while (true) {
    if (progress != (int)(volume.GetProgress() * 100)) {
      LOG(INFO) << "Status:" << ToString(volume.GetStatus())
                << ", progress: " << volume.GetProgress();
      progress = volume.GetProgress() * 100;
      volume.Draw(target);
    }
  }
  WINPAUSE;
}