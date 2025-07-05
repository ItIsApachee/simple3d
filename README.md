# simple3d

Project uses C++20.

## Integration

```cmake
include(FetchContent)

FetchContent_Declare(
    simple3d
    GIT_REPOSITORY https://github.com/ItIsApachee/simple3d.git # or git@github.com:ItIsApachee/simple3d.git
    GIT_TAG master # FIXME(apachee): Replace with tag or commit hash.
    )
FetchContent_MakeAvailable(simple3d)

target_link_libraries(foo PRIVATE simple3d)
```

## Build instructions (for development purposes)
1. Install dependencies:
- CMake (3.20 and up)
- C++20-compatible compiler.
- Emscripten SDK.

2. Build:
```bash
$ cmake -S <simple3d-dir> -B <simple3d-dir>/build -DSIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE=<emsdk-dir>/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE:STRING=<cfg> ...
$ cmake --build build
```

## Credits

Many project design choices were inspired by [YTsaurus](https://ytsaurus.tech/). Here is short list of some of those choices:
- Monorepo-like file structure.
- simple3d/core library.
- [Code style](https://github.com/ytsaurus/ytsaurus/blob/main/yt/styleguide/cpp.md).
