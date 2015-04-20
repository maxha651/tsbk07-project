//
// Created by max on 2015-04-05.
//

#ifndef TSBK07_PROJECT_GAMEOBJECT_H
#define TSBK07_PROJECT_GAMEOBJECT_H

#include <vector>

#include <JSONLoader.h>
#include <GOTransform.h>

class Component;

/**
 * @brief Class for any object in game, contains Components
 */
class GameObject {

    public:
        GameObject();
        GameObject(const std::string& path);
        virtual ~GameObject();

        template<class T> T GetComponent();
        void AddComponent(Component);
        void Update();
        const std::string& GetName() const;

        GOTransform transform; 
    private:
        JSONLoader jsonLoader;
        std::string name;
        std::vector<Component> components;

};


#endif //TSBK07_PROJECT_GAMEOBJECT_H
