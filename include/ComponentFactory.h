#pragma once

#include <memory>
#include <vector>

#include <Component.h>

/**
 * Used for creating Component instances providing name as string.
 * \note Header only because we'll probable get undefined template specializations otherwise.
 */
class ComponentFactory {

public:
    ComponentFactory();

    ~ComponentFactory();

    template<typename... Args> static
    Component * GetComponent(const std::string& name, Args... args);

    template<typename... Args>
    Component * NewComponent(const std::string& name, Args... args);

private:
    std::vector<std::shared_ptr<Component>> createdComponents;

    template<typename... Args> static
    Component * ComponentMap(const std::string& name, Args... args);

    template<class C, typename... Args> static
    C* _NewComponent(const std::string& name, Args... args);
};

