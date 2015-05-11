//
// Created by max on 2015-05-10.
//

#include <RadiosityLight.h>
#include <Context.h>
#include <GameObject.h>

using Eigen::Vector3f;
using Eigen::Vector4f;

RadiosityLight::RadiosityLight() : PointLight() {

}

RadiosityLight::~RadiosityLight() {

}

void RadiosityLight::Update() {
    PointLight::Update();

    for (auto& light : Context::Instance().pointLights) {
        if (light.second->reference != reference) {
            AddIncomingLight(light.second);
        }
    }
}

void RadiosityLight::Render() {
    PointLight::Render();


}

void RadiosityLight::AddIncomingLight(PointLight* light) {
    Vector3f wPosition = light->GetGameObject()->transform.GetPosition() + light->position;
    Vector3f toDir = wPosition - (position + GetGameObject()->transform.GetPosition());

    float dot = toDir.dot(normal);

    if (dot <= 0.0f) {
        return;
    }

    //color += dot * light->intensity * light->color / toDir.len;
}
