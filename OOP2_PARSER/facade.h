#ifndef FACADE_H
#define FACADE_H
#include <iostream>
#include <memory>
#include "logic.h"


class Facade {
private:
    FileWorker* fileWorker;
    JsonChecker* jsonCheker;
public:
    Facade(FileWorker* file = nullptr, JsonChecker* checker = nullptr):fileWorker(file), jsonCheker(checker){}
    ~Facade(){}
    std::string filePath;
    std::string readOperation();
    void checkOperation();
};

#endif // FACADE_H
