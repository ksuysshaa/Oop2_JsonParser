#include "facade.h"
#include "logic.h"

std::string Facade::readOperation()
{
    return fileWorker->getTextFile();
}

void Facade::checkOperation()
{
    jsonCheker->checkJson();
}

