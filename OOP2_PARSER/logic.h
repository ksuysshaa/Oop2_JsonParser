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
    JsonChecker(): stringNum(0), posInFile(1), posInLine(1), containError(false), errorMessage("File is correct"){}
    JsonChecker(const std::string& ft):fileText(ft){}
    void checkJson();
    std::string error();
private:
    std::string fileText;
    std::string errorMessage = "File is corrrect";
    std::string correctSymbols = "-.0123456789";
    int stringNum = 0;
    int posInFile = 1;
    int posInLine = 1;
    bool containError = false;
    void skipEmpty(const std::string& fileText);
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
