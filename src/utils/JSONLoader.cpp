//
// Created by max on 2015-04-05.
//

#include <jeayeson/jeayeson.hpp>

#include "JSONLoader.h"

AbstractLoader::AbstractLoader(std::string path) {

}

AbstractLoader::~AbstractLoader() {

}

template<typename T> T AbstractLoader::getNext() {

    return NULL;
}
