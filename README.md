# simple3d
This library aims to be a simple tool to let you visualise simple 3D primitives. 
To provide maximum flexibility the library is using ANGLE (https://github.com/google/angle)
as graphics backend.
C++ namespace - Simpl3D.
Currently there is no aim to support multiple windows, and contexts, but it might be provided in the future.

## Development.
Coding standard - Google C++ Style Guide. (https://github.com/cpplint/cpplint)

## Components.
1. (graphics) Objects related to 3D rendering.
2. (context) Window manager: creating windows, GLES contexts, main loop, etc.
3. ImGui support (?)
4. Texture manager
5. Tools for optimizing visualisation of massive amounts of data.

## Dependencies.
- glad (https://github.com/Dav1dde/glad)
- GLFW (https://www.glfw.org/).
- ANGLE (https://github.com/google/angle)
- OpenGL Mathematics (GLM) (https://github.com/g-truc/glm)
- assimp (https://github.com/assimp/assimp) 
- Dear ImGui (https://github.com/ocornut/imgui)
- stb_image (https://github.com/nothings/stb)

## Dependencies for building examples:
- vcpkg (https://github.com/microsoft/vcpkg)

## Limitations.
- Context can be initialized only once, otherwise you can expect undefined
behaviour.
- Only one instance of shader per type. Makes shaders less flexible, but
increases overall performance as it decreases switches between shader
programs.
- Only one instance of renderer per scene. Couldn't come up with a good
way to allow that, but also provide easy library interface.

## TODO:
### Overall.
- [x] Create CMake config of library
- [x] Organize components in folders.
- [x] Create documentation (doxygen).
- [ ] Add support for non-desktop devices: Android, IOS.

### 1. Objects related to 3D rendering.
- [x] Create basic API/interface of library:
    - [x] Scene object (relative position of objects).
    - [x] Camera object (render capabilites).
    - [ ] Misc (additional 3D primitives).
- [ ] Implement API/interface:
    - [x] Simple lighting.
    - [x] Scene object (relative position of objects).
    - [x] Camera object (render capabilites).
    - [x] Controls for Camera object.
    - [ ] Misc (additional 3D primitives).
- [ ] 3D primitives:
    - [x] Cuboid.
    - [ ] Triangle.
    - [ ] Spehere: basic, icosphere, cubesphere.
    - [ ] Particle.


### 3. Window manager: creating windows, GLES contexts, main loop, etc.
- [x] Initialization, and termination.
- [x] Context management.
- [x] Input management.

### 4. ImGui support (?)
- [x] Add ImGui support.
### 5. Model manager
- [x] Create simple model loading.
- [ ] Add support for embedded textures (assimp).
### 6. Tools for optimizing visualisation of massive amounts of data.
- [ ] Optimized ParticleRenderer.