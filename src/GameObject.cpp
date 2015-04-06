//
// Created by max on 2015-04-05.
//

#include <GameObject.h>

GameObject::GameObject() {
}

GameObject::~GameObject() {

}

template<class T> T GameObject::GetComponent() {
    for (auto it : components) {
        if (typeid(*it) == typeid(T)) {
            return *it;
        }
    }
    return nullptr;
}

void GameObject::AddComponent(Component component) {
    components.push_back(component);
}

