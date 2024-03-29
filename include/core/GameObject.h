#pragma once 

//
// Created by max on 2015-04-05.
//

#include <vector>
#include <typeinfo>
#include <memory>

#include <JSONLoader.h>

#include <GOTransform.h>
#include <Component.h>
#include <ComponentFactory.h>

/**
 * @brief Class for any object in game, contains Components
 */
class GameObject {

public:
    GameObject();
    GameObject(const std::string& path);
    GameObject(const std::string& name, const std::string& path);
    virtual ~GameObject();

    template<class T> T* GetComponent() const {
        for (auto c : components) {
            if (typeid(*c) == typeid(T)) {
                return dynamic_cast<T*>(c);
            }
        }
        return nullptr;
    }
    void AddComponent(Component*);

    const std::string& GetName() const;
    void SetSaveOnExit(bool value);

    GOTransform transform;

    void Update();
    void Render();
    void Awake();
	void Start();

private:
    ComponentFactory componentFactory;
    std::string name;

    std::vector<std::string> componentNames;
    std::vector<Component*> components;

    // Keep all dependent variables before this
    JSONLoader jsonLoader;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
};

