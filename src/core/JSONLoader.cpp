#include <JSONLoader.h>

#include <ComponentFactory.h>

static const bool SAVE_ON_DESTRUCT_DEFAULT = false;

// No initialization => nothing to save
JSONLoader::JSONLoader() : saveOnDestruct(false) {

}

// No path to save to when using this one
JSONLoader::JSONLoader(const Json::Value& root) :saveOnDestruct(false) {
    Init(root);
}

JSONLoader::JSONLoader(const std::string& path) 
    : saveOnDestruct(SAVE_ON_DESTRUCT_DEFAULT), filePath(path) {
    }


JSONLoader::~JSONLoader() {
    if (saveOnDestruct) {
        Write();
    }
}

template <typename T> void JSONLoader::AddDataField(const std::string& name, T* dataPtr) {
    DataField dataField(Json::Value(*dataPtr), static_cast<void*>(dataPtr));
    dataFieldVec.push_back(dataField);
}

template <typename T> void JSONLoader::AddArrayField(const std::string& name, std::vector<T>* vector) {
    IVectorPtr* vecPtr = dynamic_cast<IVectorPtr*>(new VectorPtr<T>(vector));
    DataField dataField{ Json::arrayValue, static_cast<void*>(vector), vecPtr};

    dataField.jsonValue.resize(vector->size());
    for(int idx = 0; idx < vector->size(); ++idx) {
        dataField.jsonValue[idx] = Json::Value(vector->at(idx));
    }

    dataFieldVec.push_back(dataField);
}

void JSONLoader::SetSaveOnDestruct(bool value) {
    saveOnDestruct = value;
}

void JSONLoader::Init(const Json::Value& root) {
    // If we haven't added enough data fields then that data will not (can not) be
    // updated on write.
    DataField defaultValue{ Json::Value(), nullptr };
    dataFieldVec.resize(std::max<size_t>(root.size(), dataFieldVec.size()), defaultValue);

    // Update fields from JSON file
    for (int idx = 0; idx < root.size(); ++idx) {
        dataFieldVec[idx].jsonValue = std::move(root[dataFieldVec[idx].name]);
    }

    // TODO: Set values from json...
}

void JSONLoader::Read() {
    std::ifstream inStream( filePath, std::ifstream::binary );
    Json::Value root(Json::objectValue);

    inStream >> root;

    Init(root);
}

void JSONLoader::Write() {
    Json::Value root = ToJson();

    std::ofstream outStream(filePath);
    // Overwrite old file contents
    outStream.seekp(0);
    outStream << root;
}

Json::Value JSONLoader::ToJson() {
    Json::Value root(Json::objectValue);

    for (auto dataField : dataFieldVec) {
        root.append(ToJson(dataField));
    }

    return root;
}

Json::Value JSONLoader::ToJson(const DataField& dataField) {
    // Default to old in case something goes wrong
    Json::Value jsonValue(dataField.jsonValue);

    if (dataField.dataPtr != nullptr) {
        switch (dataField.jsonValue.type()) {
            case Json::nullValue:
                {
                    std::cerr << "JSONLoader: Can't parse null value" << std::endl;
                    break;
                }
            case Json::realValue:
                {
                    double value = *static_cast<double*>(dataField.dataPtr);
                    jsonValue = Json::Value(value);
                    break;
                }
            case Json::stringValue:
                {
                    std::string value = *static_cast<std::string*>(dataField.dataPtr);
                    jsonValue = Json::Value(value);
                    break;
                }
            case Json::booleanValue:
                {
                    bool value = *static_cast<bool*>(dataField.dataPtr);
                    jsonValue = Json::Value(value);
                    break;
                }
            case Json::arrayValue:
                {
                    // XXX: Somewhat of a hack. Encapsulates std::vector in class so we
                    // don't have to deduce the type quite yet.
                    IVectorPtr* vecPtr = static_cast<IVectorPtr*>(dataField.dataPtr);
                    for (int idx = 0; idx < jsonValue.size(); ++idx) {
                        jsonValue[idx] = ToJson(DataField{ jsonValue[idx], vecPtr->getPtr(idx) });
                    }
                    break;
                }
            case Json::objectValue:
                {
                    std::cout << "JSONLoader: Something saved as object value, probably wrong" << std::endl;
                    jsonValue = dataField.jsonValue;
                    break;
                }
            default:
                {
                    std::cerr << "JSONLoader: Unrecognized type: " << dataField.jsonValue.type() << std::endl;
                break;
                }
        }
    }

    jsonValue.setComment(dataField.jsonValue.getComment(Json::CommentPlacement::commentBefore),
            Json::CommentPlacement::commentBefore);

    return jsonValue;
}

// Explicitly define valid template instances
// Others will yield undefined reference
template void JSONLoader::AddDataField<double>(const std::string& name, double* dataPtr);
template void JSONLoader::AddDataField<std::string>(const std::string& name, std::string* dataPtr);
template void JSONLoader::AddDataField<bool>(const std::string& name, bool* dataPtr);
template void JSONLoader::AddArrayField<double>(const std::string& name, std::vector<double>* vector);
template void JSONLoader::AddArrayField<std::string>(const std::string& name, std::vector<std::string>* vector);
// TODO: vector bool specialization fucks up my cool/hacky VectorPtr class
// Fix this if needed, might be to see bools as int, implicit conversion all the way
//template void JSONLoader::addArrayField<bool>(std::vector<bool>* vector);


