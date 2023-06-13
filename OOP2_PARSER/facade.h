#ifndef FACADE_H
#define FACADE_H
#include <iostream>
#include <memory>
#include "logic.h"

class Facade
{
private:
    FileWorker* fileWorker;
    JsonChecker* jsonChecker;
public:
    Facade(FileWorker* file = nullptr, JsonChecker* checker = nullptr):fileWorker(file), jsonChecker(checker){}
    ~Facade() {
        if (fileWorker != nullptr)
            delete fileWorker;
        if (jsonChecker != nullptr)
            delete jsonChecker;
    }
    std::string filePath;
    std::string readOperation();
    void checkOperation();
    void rollBack() {jsonChecker->rollBack();}
};

#endif // FACADE_H
