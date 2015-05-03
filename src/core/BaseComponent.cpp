//
// Created by max on 2015-04-20.
//

#include <BaseComponent.h>

#include <cassert>

#include <GameObject.h>


BaseComponent::BaseComponent(const std::string &jsonPath) {
    std::cerr << "Component doesn't implement JSON constructor :(" << std::endl;
    assert(false);
}

BaseComponent::BaseComponent() {

}

BaseComponent::~BaseComponent() {

}

void BaseComponent::Update(){
    std::cout << "Base to the B" << std::endl;
}

void BaseComponent::Render() {

}

GOTransform& BaseComponent::GetTransform() {
    return this->gameObject->transform;
}
