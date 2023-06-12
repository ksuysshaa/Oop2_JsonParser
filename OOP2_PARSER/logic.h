#ifndef LOGIC_H
#define LOGIC_H
#include "iostream"
#include "fstream"
#include "stack"
#include "jsonerror.h"

class FileWorker
{
public:
    std::string filePath;
    FileWorker(std::string fp):filePath(fp){}
    std::string getTextFile();
};

class ExpectedPart;

class JsonChecker
{
public:
    JsonChecker();
    JsonChecker(const std::string& ft):fileText(ft){}
    void checkJson();
    std::string error();
private:
    std::string fileText;
    std::string errorMessage;
    std::string correctSymbols = "-.0123456789";
    int stringNum;
    int posInFile;
    int posInLine;
    bool containError;
    void skipEmpty (const std::string& fileText);
    void checkObject(const std::string& fileText);
    void checkString(const std::string& fileText);
    void checkSpecialType(const std::string &fileText);
    void checkNumber(const std::string& fileText);
    void checkArray(const std::string& fileText);
    void objectError(const std::string& fileText, ExpectedPart part);
    void arrayError(const std::string& fileText, bool expectValue);
};

class ExpectedPart
{
public:
    std::string current;
public:
    std::string key = "ключ";
    std::string colon = "двоеточие";
    std::string value = "значение";
    std::string endOfLine = ",";
    std::string endOfObject = "}";
};

#endif // LOGIC_H
