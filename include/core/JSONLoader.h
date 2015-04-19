#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <json/json.h>
#include <json/value.h>

/**
 * \brief JSON loader utility class.
 *
 * Usage: Initialize in class' ctr by passing appropriate 
 * data fields to initDataField(). Values are automatically 
 * updated if saveOnDestruct is true. Default is saveOnDestruct=true.
 *
 */

class JSONLoader {
    public:
        JSONLoader(const std::string& path);
        ~JSONLoader();

        template <typename T> void addDataField(T* dataPtr);
        template <typename T> void addArrayField(std::vector<T>* dataPtr);
        void setSaveOnDestruct(bool value);
        void read();
        void write();

    protected:
        Json::Value toJson();

    private:
        class DataField;
        class IVectorPtr;
        template <typename T> class VectorPtr;

        bool saveOnDestruct;
        std::string filePath;
        std::vector<DataField> dataFieldVec;

        Json::Value toJson(const DataField& dataField);
};
