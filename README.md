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

TODO(apachee): Complete instructions and README as a whole

## Credits

Many project design choices were inspired by [YTsaurus](https://ytsaurus.tech/). Here is short list of some of those choices:
- Monorepo-like file structure.
- simple3d/core library.
- [Code style](https://github.com/ytsaurus/ytsaurus/blob/main/yt/styleguide/cpp.md).
