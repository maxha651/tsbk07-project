//
// Created by max on 2015-05-10.
//

#include <PointLight.h>

#include <numeric>

#include "Context.h"

using Eigen::Vector3f;

PointLight::PointLight() : intensity(1.0f), radius(std::numeric_limits<float>::max()) {
    reference = ++Context::Instance().pointLightsRefCntr;
    Context::Instance().pointLights[reference] = this;
}

PointLight::PointLight(const Vector3f& position) : PointLight() {
    this->position = position;
}

PointLight::~PointLight() {
    Context::Instance().pointLights.erase(reference);
}

void PointLight::Update() {
    BaseComponent::Update();
}

void PointLight::Render() {
    BaseComponent::Render();
}
