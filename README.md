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
- GLAD (https://github.com/Dav1dde/glad)
- GLFW (https://www.glfw.org/).
- ANGLE (https://github.com/google/angle).

## Limitations.
- Context can be initialized only once, otherwise you can expect undefined
behaviour.

## TODO:
### Overall.
- [x] Create CMake config of library
- [x] Organize components in folders.
- [x] Create documentation (doxygen).
- [ ] Add support for non-desktop devices: Android, IOS.

### 1. Objects related to 3D rendering.
- [x] Create basic API/interface of library:
    - [x] Scene object (relative position of objects).
    - [ ] Camera object (render capabilites).
    - [ ] Misc (additional 3D primitives).
- [ ] Implement API/interface:
    - [ ] Simple lighting.
    - [ ] Scene object (relative position of objects).
    - [ ] Camera object (render capabilites).
    - [ ] Controls for Camera object.
    - [ ] Misc (additional 3D primitives).
- [ ] 3D primitives:
    - [ ] Cuboid.
    - [ ] Triangle.
    - [ ] Spehere: basic, icosphere, cubesphere.
    - [ ] Particle.
- [ ] Simple lighting.
- [ ] Controls for Camera object.


### 3. Window manager: creating windows, GLES contexts, main loop, etc.
- [x] Initialization, and termination.
- [x] Context management.
- [ ] Input management.

### 4. ImGui support (?)
- ...
### 5. Texture manager
- ...
### 6. Tools for optimizing visualisation of massive amounts of data.
- [ ] Optimized ParticleRenderer.