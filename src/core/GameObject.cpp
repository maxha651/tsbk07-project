//
// Created by max on 2015-04-05.
//

#include <GameObject.h>

GameObject::GameObject() {
}

GameObject::GameObject(const std::string& path) :
    jsonLoader(path + "/GameObject.json"), transform(path + "/GOTransform.json") {

    jsonLoader.AddArrayField<std::string>("components", &componentNames);
    jsonLoader.Read();

    for (auto component : componentNames) {
        std::string componentPath(path);
        componentPath.append("/" + component + ".json");
        AddComponent(componentFactory.NewComponent<const std::string&>(component, componentPath));
        std::cout << "GameObject: " << name << " added Component: " << component << std::endl;
    }
}

GameObject::GameObject(const std::string &name, const std::string &path) : GameObject(path) {
    this->name = name;
}

GameObject::~GameObject() {
    std::cout << "GameObject: Deleting gameObject: " << name << std::endl;
}

void GameObject::AddComponent(Component* component) {
	component->SetGameObject(this);
    components.push_back(component);
}

void GameObject::Update() {
    for (auto* component : components) {
        component->Update();
    }
}

const std::string& GameObject::GetName() const {
    return name;
}

void GameObject::SetSaveOnExit(bool value) {
    jsonLoader.SetSaveOnDestruct(value);
}

void GameObject::Render() {
    for (auto* component : components) {
        component->Render();
    }
}
