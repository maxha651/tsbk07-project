//
// Created by max on 2015-04-05.
//

#include <Game.h>
#include <Context.h>

Game::Game(const std::string& path) : path(path) {
    DIR *dir;
    struct dirent *ent;

    // All directories in path should be a GameObject instance
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strncmp(ent->d_name, ".", 1) != 0) {
                // "." and ".." are this and parent folder
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

    if (Context::Instance().game == nullptr) {
        Context::Instance().game = this;
        std::cout << "Game: Added ourselves to Context" << std::endl;
    }
}

Game::~Game() {
}

GameObject& Game::GetGameObject(const std::string& name) {
    for (auto& go : gameObjects) {
        if (name.compare(go->GetName()) == 0) {
            return *go;
        }
    }
    throw new std::exception();
}

void Game::AddGameObject(const std::string& name) {
    std::string goPath(path + "/" + name);
    gameObjects.emplace_back(new GameObject(name, goPath));
}

void Game::AddGameObject(GameObject* gameObject) {
    //gameObjects.emplace_back(gameObject);
}

void Game::Update() {
    for (auto& go : gameObjects) {
        go->Update();
    }
}

void Game::SetSaveOnExit(bool value) {
    for (auto& go : gameObjects) {
        go->SetSaveOnExit(value);
    }
}


void Game::Render() {
    for (auto& go : gameObjects) {
        if (InsideCulling(go))
            go->Render();
    }
}


void Game::Start() {
	for (auto& go : gameObjects) {
		go->Start();
	}
}

void Game::Awake() {
	for (auto& go : gameObjects) {
		go->Awake();
	}

}

bool Game::InsideCulling(GameObject* gameObject) {
    Vector3f cameraPos = Context::Instance().camera->GetTransform()->GetPosition();
    Vector3f gameObjectPos = gameObject->transform.GetPosition();
    Vector3f relPos = gameObjectPos - cameraPos;
    Vector3f cullFwd = Context::Instance().camera->cullFwd;
    Vector3f cullWidth = Context::Instance().camera->cullWidth;
    // Behind camera
    if (relPos.dot(cullFwd) < 0.0f) {
        return false;
    }
    // Too far away
    if (relPos.dot(cullFwd.normalized()) > cullFwd.norm()) {
        return false;
    }
    // Not within width
    if (std::abs(relPos.dot(cullWidth.normalized())) > cullWidth.norm() * 0.5f) {
        return false;
    }
    return true;
}
