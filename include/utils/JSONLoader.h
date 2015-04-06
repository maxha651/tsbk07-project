//
// Created by max on 2015-04-05.
//

#ifndef TSBK07_PROJECT_JSONLOADER_H
#define TSBK07_PROJECT_JSONLOADER_H

#include <string>

/**
 * @brief Utility class for loading JSON
 */
class JSONLoader {

public:
    JSONLoader(std::string path);
    virtual ~JSONLoader();

    template<typename T> T getNext();

private:
    json_map map;
    json_map::iterator it;

};


#endif //TSBK07_PROJECT_JSONLOADER_H
