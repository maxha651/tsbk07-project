#pragma once

//
// Created by max on 2015-05-03.
//
// Keeps pointers to global stuff
//

#include <mutex>

// Forward declarations (no need to include headers when
// only using non-dereferenced pointers)
class Camera;
class PointLight;

class Context
{
public:
    Camera* camera;
	unsigned int program;
    std::map<size_t, PointLight*> pointLights;
    size_t pointLightsRefCntr;

    static Context& Instance()
    {
        static Context instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
private:
    Context() : camera(nullptr), pointLightsRefCntr(0) {};

    // Make sure no copy constructors are implemented
    // Compiler will create these for us otherwise
    Context(Context const&) = delete;
    void operator=(Context const&) = delete;
};