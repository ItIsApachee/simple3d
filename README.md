# simple3d
This library aims to be a simple tool to let you visualise simple 3D primitives. 
To provide maximum flexibility the library is using ANGLE (https://github.com/google/angle)
as graphics backend.

## Development.
Coding standard - Google C++ Style Guide. (https://github.com/cpplint/cpplint)

## Components.
1. Objects related to 3D rendering.
2. (context) Window manager: creating windows, GLES contexts, main loop, etc.
3. ImGui support (?)
4. Texture manager
5. Tools for optimizing visualisation of massive amounts of data.

## Dependencies.
- GLAD (https://github.com/Dav1dde/glad)
- GLFW (https://www.glfw.org/).
- ANGLE (https://github.com/google/angle).

## TODO:
### Overall.
- [ ] Create CMake config of library
- [ ] Organize components in folders.
- [ ] Create documentation (doxygen).

### 1. Objects related to 3D rendering.
- [ ] Create basic API/interface of library:
    - [ ] Basic 3D primitives.
    - [ ] Simple lighting.
    - [ ] Scene object (relative position of objects).
    - [ ] Camera object (render capabilites).
    - [ ] Controls for Camera object.
    - [ ] Misc (additional 3D primitives).
- [ ] Implement API/interface:
    - [ ] Basic 3D primitives.
    - [ ] Simple lighting.
    - [ ] Scene object (relative position of objects).
    - [ ] Camera object (render capabilites).
    - [ ] Controls for Camera object.
    - [ ] Misc (additional 3D primitives).

### 2. Window manager: creating windows, GLES contexts, main loop, etc.
- [ ] Initialization, and termination.
- [ ] Main loop.
- [ ] Window creation.
- [ ] Context management.

### 3. ImGui support (?)
- ...
### 4. Texture manager
- ...
### 5. Tools for optimizing visualisation of massive amounts of data.
- ...