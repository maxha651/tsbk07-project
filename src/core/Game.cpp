//
// Created by max on 2015-04-05.
//

#include <Game.h>

Game::Game(const std::string& path) {

}

Game::~Game() {
    if (saveOnExit) {
        // TODO Write JSON   
    }
}

GameObject& Game::GetGameObject(const std::string& name) {
    for (auto& go : gameObjects) {
        if (name.compare(go.Name())) {
            return go;
        }
    }
    throw new NoSuchObjectException();
}

void Game::AddGameObject(const std::string& name) {
    // TODO calc path and send that instead
    gameObjects.push_back(GameObject::GameObject(name));
}

void Game::Update() {
    for (GameObject go : gameObjects) {
        go.Update();
    }
}

void Game::SetSaveExit(bool value) {
    saveOnExit = value;
}


