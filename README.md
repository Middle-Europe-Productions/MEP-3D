<a href="url"><img src="https://github.com/Middle-Europe-Productions/MEP/blob/master/docs/mepdoc.png" align="center" height="159" width="392" ></a>

## MEP-3D

#### CMake
Goto project directory.
```sh
cmake -S . -B build/
```

#### Building scripts
* Using Unix build script
```sh
./build/examples/run_mep_unix.sh -t/--target <target> <flags>
```
or from the appropriate directory
```sh
run_mep_unix.sh <target> <flags>
```

* Using MSVC build script
Open the visual studio developer console or set environment variables.
```sh
cd <build>/examples
run_mep_msvc.bat <target> <mode=Debug>
```
##### Flags
Mep engine allows default chromium flags, for example:
```sh
-v=2
```
For more info please check this link [link](https://github.com/google/glog).

#### Available tools
* Window creation using GLFW
* Projection view
* Mesh handler with UV
* Key input observer integrated to window
* Image loading using STB
* Texture handler integrated with mesh
* Camera with build-in key input handling

### Avalible lightning

#### Phong lightning
The supported lighting model is Phong lighting. It consists of three types of lightning applied to a given object.
* Ambient light - simulation of a light bouncing from another object.
* Diffuse light - simulates from off of light following the angle of light.
* Specular light - relays on a user's position. It is a simulation of a direct reflection of a light coming to the viewer's eye.

#### Type of light supperted in the engine:
    * Directional light
    * Point light
    * Stop light

#### Model loading
The engine currently supports assimp library as its main object loader.

#### Build in thread pool
Objects are always loaded in the dedicated resource thread pool.
