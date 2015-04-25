#pragma once 

//
// Created by max on 2015-04-05.
//

#include <vector>
#include <typeinfo>

#include <JSONLoader.h>

#include <GOTransform.h>
#include <Component.h>

/**
 * @brief Class for any object in game, contains Components
 */
class GameObject {

public:
    GameObject();
    GameObject(const std::string& path);
    virtual ~GameObject();

    template<class T> T GetComponent();
    void AddComponent(Component&);

    void Update();
    const std::string& GetName() const;
    void SetSaveOnExit(bool value);

    GOTransform transform;
private:
    JSONLoader jsonLoader;
    std::string name;

    std::vector<std::string> componentNames;
    std::vector<Component> components;
};

