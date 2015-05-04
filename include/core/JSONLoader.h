#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>

#include <json/json.h>
#include <json/value.h>
#include <assert.h>

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

    template <typename T>
    void AddDataField(const std::string& name, T* dataPtr);
    template <typename T>
    void AddArrayField(const std::string& name, std::vector<T>* dataPtr);
    template <typename T>
    void AddArrayField(const std::string& name, T dataPtr[], size_t len);
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
        virtual size_t Size() = 0;
        virtual void Resize(size_t size) = 0;
    };

    template <typename T>
    class VectorPtr : public IVectorPtr {
    public:
        VectorPtr(std::vector<T>* vecPtr) : vecPtr(vecPtr) {}
        virtual ~VectorPtr() {}

        void* getPtr() override {
            return static_cast<void*>(vecPtr);
        }
        void* getPtr(size_t idx) override {
            return static_cast<void*>(&(vecPtr->at(idx)));
        }
        size_t Size() override { return vecPtr->size(); }
        void Resize(size_t size) override { vecPtr->resize(size); }
    private:
        std::vector<T>* vecPtr;
    };

    template <typename T>
    class ArrayPtr : public IVectorPtr {
    public:
        ArrayPtr(T array[], size_t len) : array(array), len(len) {}
        virtual ~ArrayPtr() {}

        void* getPtr() override {
            return static_cast<void*>(array);
        }
        void* getPtr(size_t idx) override {
            assert(idx < len);
            return static_cast<void*>(&array[idx]);
        }
        size_t Size() override { return len; }
        void Resize(size_t size) override { }

    private:
        T* array;
        size_t len;
    };

    class DataField {
    public:
        DataField(const std::string& name, Json::Value jsonValue, void* dataPtr) :
                name(name), jsonValue(jsonValue), dataPtr(dataPtr), vecPtr(nullptr) {}
        DataField(const std::string& name, Json::Value jsonValue, void* dataPtr, IVectorPtr* vecPtr) :
                name(name), jsonValue(jsonValue), dataPtr(dataPtr), vecPtr(vecPtr) {}
        ~DataField() { }

        std::string name;
        Json::Value jsonValue;
        void* dataPtr;
        std::shared_ptr<IVectorPtr> vecPtr;
    };

    bool saveOnDestruct;
    std::string filePath;
    std::vector<DataField> dataFieldVec;

    void Init(const Json::Value& root);
    void FromJson(void* dataPtr, Json::ValueType type, const Json::Value& jsonValue);
    Json::Value ToJson(const DataField& dataField);
};
