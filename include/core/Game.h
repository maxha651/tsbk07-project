#pragma once

//
// Created by max on 2015-04-05.
//

#include <string>
#include <vector>

#include <GameObject.h>

class GameObject;

/**
 * \brief Main class that handles big stuff
 *
 * Is initialized by passing path to dir which 
 * contains the JSON files to be loaded.
 *
 * Added GameObjects will be written to init 
 * directory on close unless setSaveOnExit() 
 * is called with false.
 *
 */
class Game {
    public:
        Game(const std::string& path);
        ~Game();

        GameObject& GetGameObject(const std::string& name);
        void AddComponent(const std::string& name);

        void Update();

        void setSaveOnExit(bool value);
    private:
        bool saveOnExit;
        std::vector<GameObject> gameObjects;

};

