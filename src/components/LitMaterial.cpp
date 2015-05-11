//
// Created by max on 2015-05-10.
//

#include <LitMaterial.h>
#include <FBOManager.h>

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

    // Clear FBO
    // Use our camera to draw to our FBO
    // Probably use our own shader as well

}

void LitMaterial::Render() {
    BaseComponent::Render();

    GLuint colorTexture = FBOManager::Instance().getColorTexture(fboRef);
    GLuint depthTexture = FBOManager::Instance().getColorTexture(fboRef);

    // Draw normally using FBO texture from Update

}

void LitMaterial::Init() {
    Vector3f xDir = GetTransform()->Rotate(GOTransform::right);
    Vector3f yDir = GetTransform()->Rotate(GOTransform::up);

    fboRef = FBOManager::Instance().AddFBO((unsigned int)xSize, (unsigned int)ySize);
}
