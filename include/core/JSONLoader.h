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
        JSONLoader();
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
        class IVectorPtr {
            public:
                virtual ~IVectorPtr() = 0;
                virtual void* getPtr() = 0;
                virtual void* getPtr(size_t idx) = 0;
        };

        template <typename T> 
            class VectorPtr : IVectorPtr {
                public:
                    VectorPtr(std::vector<T>* vecPtr) : vecPtr(vecPtr) {}
                    virtual void* getPtr() override {
                        return static_cast<void*>(vecPtr);
                    }
                    virtual void* getPtr(size_t idx) override {
                        return static_cast<void*>(vecPtr[idx]);
                    }
                    ~VectorPtr() override {}

                private:
                    std::vector<T>* vecPtr;
            };

        class DataField {
            public:
                DataField(Json::Value jsonValue, void* dataPtr) :
                    jsonValue(jsonValue), dataPtr(dataPtr), vecPtr(nullptr) {}
                DataField(Json::Value jsonValue, void* dataPtr, IVectorPtr* vecPtr) :
                    jsonValue(jsonValue), dataPtr(dataPtr), vecPtr(vecPtr) {}
                ~DataField() { 
                    if (vecPtr != nullptr) {
                        delete vecPtr;
                    }
                }

                Json::Value jsonValue;
                void* dataPtr;
                IVectorPtr* vecPtr;
        };
        
        bool saveOnDestruct;
        std::string filePath;
        std::vector<DataField> dataFieldVec;

        Json::Value toJson(const DataField& dataField);
};
