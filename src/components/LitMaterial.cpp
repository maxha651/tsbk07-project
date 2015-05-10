//
// Created by max on 2015-05-10.
//

#include <LitMaterial.h>

static const float resolution = 0.1f;

LitMaterial::LitMaterial() {

}

LitMaterial::LitMaterial(const std::string &jsonPath) : jsonLoader(jsonPath) {
    jsonLoader.AddDataField<float>("xSize", &xSize);
    jsonLoader.AddDataField<float>("ySize", &ySize);
    jsonLoader.Read();

    Init();
}

LitMaterial::~LitMaterial() {

}

void LitMaterial::Update() {
    BaseComponent::Update();

}

void LitMaterial::Render() {
    BaseComponent::Render();

}

unsigned int LitMaterial::GetTextureUnit() {
    return 0;
}

void LitMaterial::Init() {
    Vector3f xDir = GetTransform()->Rotate(GOTransform::right);
    Vector3f yDir = GetTransform()->Rotate(GOTransform::up);

    for (float y = 0.0f; y < ySize; y += resolution) {
        for (float x = 0.0f; x < xSize; x += resolution) {
            pointLights.emplace_back(x * xDir, y * yDir);
        }
    }
}
