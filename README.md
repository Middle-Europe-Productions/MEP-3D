# Volume-Rendering:
#### MSVC (Windows):
Creating project:
```sh
// Debug:
// Creating project:
cmake . --preset volume-rendering-msvc-d
// Building:
cmake --build --preset "volume-rendering-msvc-d"

// Release:
// Creating project:
cmake . --preset volume-rendering-msvc
// Building:
cmake --build --preset "volume-rendering-msvc"
```

#### Ninja (cross-platform):
Creating project:
```sh
// Debug:
// Creating project:
cmake . --preset volume-rendering-ninja-d
// Building:
cmake --build --preset "volume-rendering-ninja-d"

// Release:
// Creating project:
cmake . --preset volume-rendering-ninja
// Building:
cmake --build --preset "volume-rendering-ninja"
```
