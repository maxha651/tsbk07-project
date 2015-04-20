//
// Created by max on 2015-04-05.
//

#include <vector>
#include <typeinfo>

#include <Component.h>
#include <GameObject.h>

GameObject::GameObject() {
}

GameObject::GameObject(const std::string& path) :
    jsonLoader(path) {

}

GameObject::~GameObject() {
}

template<class T> T GameObject::GetComponent() {
    for (auto c : components) {
        if (typeid(c) == typeid(T)) {
            return c;
        }
    }
    return nullptr;
}

void GameObject::AddComponent(Component component) {
	component.SetGameObject(*this);
    components.push_back(component);
}

void GameObject::Update(){

}

const std::string& GameObject::GetName() const {
    return name;
}

