#pragma once

//
// Created by max on 2015-04-05.
//

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>

#include <GameObject.h>

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
    void AddGameObject(const std::string& name);
    void AddGameObject(GameObject* gameObject);

    void Update();
    void Render();
    void Awake();
	void Start();
    void SetSaveOnExit(bool value);
private:
    std::string path;
    std::vector<GameObject*> gameObjects;

};

