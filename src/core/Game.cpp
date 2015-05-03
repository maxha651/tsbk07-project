//
// Created by max on 2015-04-05.
//

#include <Game.h>

Game::Game(const std::string& path) : path(path) {
    DIR *dir;
    struct dirent *ent;

    // All directories in path should be a GameObject instance
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strncmp(ent->d_name, ".", 1) != 0) {
                AddGameObject(ent->d_name);
                std::cout << "Game: Added GameObject: " << ent->d_name << std::endl;
            }
            else {
                std::cout << "Game: Didn't add: " << ent->d_name << std::endl;
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        std::cerr << "Game: Couldn't open directory " << path << std::endl;
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
    std::string goPath(path + "/" + name);
    gameObjects.push_back(std::move(GameObject(goPath)));
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


void Game::Render() {
    for (GameObject go : gameObjects) {
        go.Render();
    }
}
