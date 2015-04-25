//
// Created by max on 2015-04-05.
//

#include <GameObject.h>
#include <ComponentFactory.h>

GameObject::GameObject() {
}

GameObject::GameObject(const std::string& path) :
    jsonLoader(path + "/GameObject.json") {

    jsonLoader.AddArrayField<std::string>("components", &componentNames);
    jsonLoader.Read();

    for (auto component : componentNames) {
        std::string componentPath(path);
        componentPath.append(component + ".json");
        components.push_back(std::move(ComponentFactory::GetComponent<std::string>(component, componentPath)));
        std::cout << "GameObject " << name << " added Component: " << component << std::endl;
    }
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

void GameObject::AddComponent(Component& component) {
	component.SetGameObject(this);
    components.push_back(component);
}

void GameObject::Update(){

}

const std::string& GameObject::GetName() const {
    return name;
}

void GameObject::SetSaveOnExit(bool value) {
    jsonLoader.SetSaveOnDestruct(value);
}

