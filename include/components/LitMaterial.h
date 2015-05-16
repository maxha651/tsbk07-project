#pragma once
//
// Created by max on 2015-05-10.
//

#include <BaseComponent.h>

class PointLight;

class LitMaterial : public BaseComponent {
public:
    LitMaterial();
    LitMaterial(const std::string& jsonPath);
    virtual ~LitMaterial();

    void Update();
    void Render();
	void Start();

private:
    void Init();

    float xSize, ySize;
    unsigned int fboRef;

    // Keep this last
    JSONLoader jsonLoader;
};


