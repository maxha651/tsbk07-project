//
// Created by max on 2015-04-05.
//

#include <Game.h>

static const std::string GAMEOBJECTS_PATH(TSBK07_GAMEOBJECTS_PATH);

Game::Game(const std::string& path) {
    DIR *dir;
    struct dirent *ent;

    // All directories in path should be a GameObject instance
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
                AddGameObject(ent->d_name);
            }
            std::cout << "Game: Added GameObject: " << ent->d_name << std::endl;
        }
        closedir(dir);
    } else {
        /* could not open directory */
        std::cerr << "Game: Couldn't open directory" << std::endl;
    }
}

Game::~Game() {
}

GameObject& Game::GetGameObject(const std::string& name) {
    for (auto& go : gameObjects) {
        if (name.compare(go.GetName())) {
            return go;
        }
    }
    throw new std::exception();
}

void Game::AddGameObject(const std::string& name) {
    std::string path(GAMEOBJECTS_PATH);
    path.append(name);

    gameObjects.push_back(GameObject(path));
}

void Game::Update() {
    for (GameObject go : gameObjects) {
        go.Update();
    }
}

void Game::SetSaveOnExit(bool value) {
    for (GameObject go : gameObjects) {
        go.SetSaveOnExit(value);
    }
}


