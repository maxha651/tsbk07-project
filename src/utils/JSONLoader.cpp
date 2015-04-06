//
// Created by max on 2015-04-05.
//

#include <jeayeson/jeayeson.hpp>

#include "JSONLoader.h"

JSONLoader::JSONLoader(std::string path)
    : map( json_file{ "path" } )
{
    it = map.begin();
}

JSONLoader::~JSONLoader() {

}

/* TODO: throw exception on fail */
template<typename T> T JSONLoader::getNext() {
    if (it == map.end()) {
        return nullptr;
    }
    it++;
    if (typeid(*it) != typeid(T)) {
        return nullptr;
    }
    return *it;
}
