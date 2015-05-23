#pragma once
//
// Created by max on 2015-05-11.
//

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <map>

#include <GL/glew.h>

class FBOManager
{
public:

    static FBOManager& Instance()
    {
        static FBOManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    unsigned int AddFBO(unsigned int width, unsigned int height);
    bool ActivateFBO(unsigned int fbo);
    void DeactivateFBO();
    unsigned int getColorTexture();
    unsigned int getDepthTexture();
    unsigned int getColorTexture(unsigned int fbo);
    unsigned int getDepthTexture(unsigned int fbo);

    static GLuint GenerateColorTexture(unsigned int width, unsigned int height);
    static GLuint GenerateDepthTexture(unsigned int width, unsigned int height);

private:
    struct FBOData {
        GLuint fbo, colorTex, depthTex, width, height;
    };

    FBOManager();

    std::map<unsigned int, struct FBOData> fboMap;
    GLuint current_fbo;

    // Make sure no copy constructors are implemented
    // Compiler will create these for us otherwise
    FBOManager(FBOManager const&) = delete;
    void operator=(FBOManager const&) = delete;
};
