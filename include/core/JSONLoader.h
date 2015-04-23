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
        JSONLoader(const Json::Value& root);
        JSONLoader(const std::string& path);
        ~JSONLoader();

        template <typename T> void AddDataField(T* dataPtr);
        template <typename T> void AddArrayField(std::vector<T>* dataPtr);
        void SetSaveOnDestruct(bool value);
        void Read();
        void Write();

    protected:
        Json::Value ToJson();

    private:
        class IVectorPtr {
            public:
                virtual ~IVectorPtr() {}
                virtual void* getPtr() = 0;
                virtual void* getPtr(size_t idx) = 0;
        };

        template <typename T> 
            class VectorPtr : public IVectorPtr {
                public:
                    VectorPtr(std::vector<T>* vecPtr) : vecPtr(vecPtr) {}
                    virtual void* getPtr() override {
                        return static_cast<void*>(vecPtr);
                    }
                    virtual void* getPtr(size_t idx) override {
                        return static_cast<void*>(&(vecPtr->at(idx)));
                    }
                    virtual ~VectorPtr() override {}

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

        void Init(const Json::Value& root);
        Json::Value ToJson(const DataField& dataField);
};
