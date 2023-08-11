# Volume-Rendering:
#### MSVC (Windows):
Creating project:
```sh
// Listing all of the presets:
cmake --list-presets
// Debug:
cmake . --preset volume-rendering-msvc-d
// Release:
cmake . --preset volume-rendering-msvc
```
Building project:
```sh
// Listing all of the build presets:
cmake --build --list-presets
// Debug:
cmake --build --preset "volume-rendering-msvc-d"
// Release:
cmake --build --preset "volume-rendering-msvc"
```
#### Ninja (cross-platform):
Creating project:
```sh
// Listing all of the presets:
cmake --list-presets
// Debug:
cmake . --preset volume-rendering-ninja-d
// Release:
cmake . --preset volume-rendering-ninja
```
Building project:
```sh
// Listing all of the build presets:
cmake --build --list-presets
// Debug:
cmake --build --preset "volume-rendering-ninja-d"
// Release:
cmake --build --preset "volume-rendering-ninja"
```
