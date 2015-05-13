//
// Created by max on 2015-05-13.
//

#include <ShaderManager.h>
#include <Input.h>
#include <Context.h>

ShaderManager::ShaderManager() {}

ShaderManager::ShaderManager(const std::string &jsonPath) : jsonLoader(jsonPath) {
    jsonLoader.AddArrayField<std::string>("shaders", &shaders);
    jsonLoader.Read();

    ShaderLoader shaderLoader;

    for (auto& shader : shaders) {
        std::string vertShader = TSBK07_SHADERS_PATH;
        vertShader += "/" + shader + ".vert";
        std::string fragShader = TSBK07_SHADERS_PATH;
        fragShader += "/" + shader + ".frag";
        programs.push_back(shaderLoader.CreateProgram(vertShader.c_str(), fragShader.c_str()));
    }

    currentProgram = 0;
    Context::Instance().program = programs.empty() ? 0 : programs[0];
}

void ShaderManager::Update() {
    BaseComponent::Update();

    if (Input::IsKeyDown('p')) {
        if (!hasSwitched) {
            Context::Instance().program = programs[++currentProgram % programs.size()];
            hasSwitched = true;
        }
    }
    else {
        hasSwitched = false;
    }
}
