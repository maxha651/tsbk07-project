//
// Created by max on 2015-05-23.
//

#include <ComponentFactory.h>
#include <Camera.h>
#include <Model.h>
#include <ShaderManager.h>
#include <Mirror.h>
#include <LineRenderer.h>
#include <MirrorContainer.h>

ComponentFactory::ComponentFactory() {}

ComponentFactory::~ComponentFactory() {}

template<typename... Args>
Component *ComponentFactory::GetComponent(const std::string& name, Args... args) {
    return ComponentMap(name, args...);
}

template<typename... Args>
Component *ComponentFactory::NewComponent(const std::string& name, Args... args) {
    Component* newComp = ComponentMap(name, args...);
    createdComponents.emplace_back(newComp);
    return newComp;
}

template<typename... Args>
Component *ComponentFactory::ComponentMap(const std::string& name, Args... args) {
    if (name.compare("Camera") == 0) {
        return _NewComponent<Camera>(name, args...);
    }
    else if (name.compare("Model") == 0) {
        return _NewComponent<Model>(name, args...);
    }
    else if (name.compare("GOTransform") == 0) {
        return _NewComponent<GOTransform>(name, args...);
    }
    else if (name.compare("ShaderManager") == 0) {
        return _NewComponent<ShaderManager>(name, args...);
    }
    else if (name.compare("Mirror") == 0) {
        return _NewComponent<Mirror>(name, args...);
    }
    else if (name.compare("LineRenderer") == 0) {
        return _NewComponent<LineRenderer>(name, args...);
    }
    else if (name.compare("MirrorContainer") == 0) {
        return _NewComponent<MirrorContainer>(name, args...);
    }
        /* ADD NEW CLASSES HERE */
    else {
        std::cerr << "ComponentFactory: Couldn't find class " << name << std::endl;
        assert(false);
        return nullptr;
    }
}

template<class C, typename... Args>
C *ComponentFactory::_NewComponent(const std::string& name, Args... args) {
    return new C(args...);
}

template Component* ComponentFactory::NewComponent<const std::string&>(const std::string &name, const std::string& jsonPath);
