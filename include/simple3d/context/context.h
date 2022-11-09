#ifndef SIMPLE3D_CONTEXT_CONTEXT_H
#define SIMPLE3D_CONTEXT_CONTEXT_H

#include <simple3d/misc/error.h>

namespace Simple3D {

// Singleton for managing windows, inputs, etc.
class MainLoop {
public:
    static MainLoop& GetInstance();

    MainLoop(const MainLoop&) = delete;
    MainLoop(MainLoop&&) = delete;
    MainLoop& operator=(const MainLoop&) = delete;
    MainLoop& operator=(MainLoop&&) = delete;

    ~MainLoop() = default;

    Error Init();
    // only in main thread
    Error Start();
    void Destroy();
private:
    MainLoop();
};

}

#endif