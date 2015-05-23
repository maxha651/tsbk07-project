#pragma once
//
// Created by max on 2015-05-13.
//

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <BaseComponent.h>
#include <ShaderLoader.h>
#include <JSONLoader.h>

class ShaderManager : public BaseComponent {

public:
    ShaderManager();
    ShaderManager(GameObject* gameObject, const std::string& jsonPath);

    void Update() override;

private:
    std::vector<std::string> shaders;
    std::vector<GLuint> programs;
    bool hasSwitched;
    int currentProgram;

    // Keep last
    JSONLoader jsonLoader;
};


