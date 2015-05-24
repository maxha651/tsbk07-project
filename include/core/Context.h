#pragma once

//
// Created by max on 2015-05-03.
//
// Keeps pointers to global stuff
//

// Forward declarations (no need to include headers when
// only using non-dereferenced pointers)
class Game;
class Camera;
class PointLight;

class Context
{
public:
    Game* game;
    Camera* camera;
	unsigned int program;
    unsigned int renderer;
	bool preBaked;

    static Context& Instance()
    {
        static Context instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
private:
    Context() : game(nullptr), camera(nullptr), renderer(0), preBaked(false) {};

    // Make sure no copy constructors are implemented
    // Compiler will create these for us otherwise
    Context(Context const&) = delete;
    void operator=(Context const&) = delete;
};