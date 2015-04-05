//
// Created by max on 2015-04-05.
//

#ifndef TSBK07_PROJECT_ABSTRACTLOADER_H
#define TSBK07_PROJECT_ABSTRACTLOADER_H

#include <string>

/**
 * @brief Abstract class for loading JSON
 */
class AbstractLoader {

public:
    AbstractLoader(std::string path);
    virtual ~AbstractLoader();

    template<typename T> T getNext();

private:


};


#endif //TSBK07_PROJECT_ABSTRACTLOADER_H
