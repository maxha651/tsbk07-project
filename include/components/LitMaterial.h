#pragma once
//
// Created by max on 2015-05-10.
//

#include <BaseComponent.h>
#include <PointLight.h>

class PointLight;

class LitMaterial : public BaseComponent {
public:
    LitMaterial();
    LitMaterial(const std::string& jsonPath);
    virtual ~LitMaterial();

    void Update();
    void Render();

    unsigned int GetTextureUnit();

private:
    void Init();

    std::vector<std::unique_ptr<PointLight>> pointLights;
    float xSize, ySize;

    // Keep this last
    JSONLoader jsonLoader;
};


