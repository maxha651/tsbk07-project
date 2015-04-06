//
// Created by max on 2015-04-05.
//

#include <jeayeson/jeayeson.hpp>

#include "JSONLoader.h"

/* 
 * The idea behind this is to provide some very easy API to load data and save JSON.
 * Ideally, loading should be entirely in initialization list (see Transform(std::string) ctr).
 * It would be nice if saving was made in the same way in destructor.
 *
 *
 * TODO: throw exception on fail 
 * FIXME: Well, there's about zero chance for this to just work
 */

JSONLoader::JSONLoader() {
    // TODO: Either disallow further use or disallow this constructor
}

JSONLoader::JSONLoader(std::string path)
    : map( json_file{ "path" } ) {
    it = map.begin();
}

JSONLoader::~JSONLoader() {

}

template<typename T> T JSONLoader::GetNext() {
    if (it == map.end()) {
        return nullptr;
    }
    it++;
    if (typeid(*it) != typeid(T)) {
        return nullptr;
    }
    return *it;
}

template<typename T> std::vector<T> JSONLoader::GetNextArray() {
    json_array json_arr = GetNext<json_array>();
    std::vector<T> arr(json_arr.size());
    for (auto entry : json_arr) {
        arr.push_back(entry.as<T>());
    }
    return arr;

}
