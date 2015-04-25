#pragma once

#include <memory>

#include <GOTransform.h>
#include <Camera.h>
#include <ModelAndShader.h>

/**
 * Used for creating Component instances providing name as string.
 * \note Header only because we'll probable get undefined template specializations otherwise.
 */
class ComponentFactory {

public:
    ComponentFactory() {}

    ~ComponentFactory() {}

    template<typename... Args>
    Component GetComponent(const std::string& name, Args... args) {
        std::unique_ptr<Component> ptr = ComponentMap(name, args...);
        return *ptr;
    }

    template<typename... Args>
    Component* NewComponent(const std::string& name, Args... args) {
        createdComponents.push_back(
                std::unique_ptr<Component>(ComponentMap(name, args...)));
    }

private:
    std::vector<std::unique_ptr<Component>> createdComponents;

    template<typename... Args>
    Component* ComponentMap(const std::string& name, Args... args) {
        if (name.compare("Camera") == 0) {
            return _NewComponent<Camera>(name, args...);
        }
        else if (name.compare("ModelAndShader") == 0) {
            return _NewComponent<ModelAndShader>(name, args...);
        }
        else if (name.compare("GOTransform") == 0) {
            return _NewComponent<GOTransform>(name, args...);
        }
        /* ADD NEW CLASSES HERE */
        else {
            std::cerr << "ComponentFactory: Couldn't find class" << std::endl;
            return nullptr;
        }
    }

    template<class C, typename... Args>
    C* _NewComponent(const std::string& name, Args... args) {
        return new C(args...);
    }
};

