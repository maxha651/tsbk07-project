#include <JSONLoader.h>

static const bool SAVE_ON_DESTRUCT_DEFAULT = true;
static const size_t DYN_DATA_MAX_LEN = 1;

// No initialization => nothing to save
JSONLoader::JSONLoader() : saveOnDestruct(false) {

}

// No path to save to when using this one
JSONLoader::JSONLoader(const Json::Value& root) :saveOnDestruct(false) {
    init(root);
}

JSONLoader::JSONLoader(const std::string& path) 
    : saveOnDestruct(SAVE_ON_DESTRUCT_DEFAULT), filePath(path) {
        read();
    }


JSONLoader::~JSONLoader() {
    if (saveOnDestruct) {
        write();
    }
}

template <typename T> void JSONLoader::addDataField(T* dataPtr) {
    DataField dataField(Json::Value(*dataPtr), static_cast<void*>(dataPtr));
    dataFieldVec.push_back(dataField);
}

template <typename T> void JSONLoader::addArrayField(std::vector<T>* vector) {
    IVectorPtr* vecPtr = dynamic_cast<IVectorPtr*>(new VectorPtr<T>(vector));
    DataField dataField{ Json::arrayValue, static_cast<void*>(vector), vecPtr};

    dataField.jsonValue.resize(vector->size());
    for(int idx = 0; idx < vector->size(); ++idx) {
        dataField.jsonValue[idx] = Json::Value(vector->at(idx));
    }

    dataFieldVec.push_back(dataField);
}

void JSONLoader::setSaveOnDestruct(bool value) {
    saveOnDestruct = value;
}

void JSONLoader::init(const Json::Value& root) {
    // If we haven't added enough data fields then that data will not (can not) be
    // updated on write.
    DataField defaultValue{ nullptr, nullptr };
    dataFieldVec.resize(std::max<size_t>(root.size(), dataFieldVec.size()), defaultValue);

    // Update fields from JSON file
    for (int idx = 0; idx < root.size(); ++idx) {
        dataFieldVec[idx].jsonValue = std::move(root[idx]);
    }
}

void JSONLoader::read() {
    dataFieldVec.clear();

    std::ifstream inStream( filePath, std::ifstream::binary );
    Json::Value root(Json::objectValue);

    inStream >> root;

    init(root);
}

void JSONLoader::write() {
    Json::Value root = toJson();

    std::ofstream outStream(filePath);
    // Overwrite old file contents
    outStream.seekp(0);
    outStream << root;
}

Json::Value JSONLoader::toJson() {
    Json::Value root(Json::objectValue);

    for (DataField dataField : dataFieldVec) {
        root.append(toJson(dataField));
    }

    return root;
}

Json::Value JSONLoader::toJson(const DataField& dataField) {
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
                        jsonValue[idx] = toJson(DataField{ jsonValue[idx], vecPtr->getPtr(idx) });
                    }
                    break;
                }
            case Json::objectValue:
                {
                    JSONLoader* value = static_cast<JSONLoader*>(dataField.dataPtr);
                    jsonValue = value->toJson();
                    break;
                }
            default:
                break;
        }
    }

    jsonValue.setComment(dataField.jsonValue.getComment(Json::CommentPlacement::commentBefore),
            Json::CommentPlacement::commentBefore);

    return jsonValue;
}

// Explicitly define valid template instances
// Others will yield undefined reference

template void JSONLoader::addDataField<double>(double* dataPtr);
template void JSONLoader::addDataField<std::string>(std::string* dataPtr);
template void JSONLoader::addDataField<bool>(bool* dataPtr);
template void JSONLoader::addArrayField<double>(std::vector<double>* vector);
template void JSONLoader::addArrayField<std::string>(std::vector<std::string>* vector);
// TODO: vector bool specialization fucks up my cool/hacky VectorPtr class
//template void JSONLoader::addArrayField<bool>(std::vector<bool>* vector);


