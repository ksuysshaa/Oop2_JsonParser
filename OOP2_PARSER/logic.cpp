#include "logic.h"
#include "jsonerror.h"

#define STRING_ERROR "incorrect string"
#define SPECIAL_TYPE_ERROR "incorrect special type"
#define NUMBER_ERROR "incorrect number"

std::string FileWorker::getTextFile()
{
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
        throw SystemError("File was not opened");
    std::string tmp;
    std::string data;
    while(std::getline(file, tmp))
        data += tmp + "\n";
    return data;
}

void JsonChecker::checkJson()
{
    skipEmpty(fileText);
    if (fileText[stringNum] == '"')
        checkString(fileText);
    else if (fileText[stringNum] == '[')
        checkArray(fileText);
    else if (fileText[stringNum] == '{')
        checkObject(fileText);
    else if (fileText[stringNum] == 'f' || fileText[stringNum] == 't' || fileText[stringNum] == 'n')
        checkSpecialType(fileText);
    else if (correctSymbols.find(fileText[stringNum]) != std::string::npos)
        checkNumber(fileText);
    else {
        containError = true;
        throw JsonError("incorrect elements", "correct elements", posInFile, posInLine);
    }
    if (containError == false) {
        skipEmpty(fileText);
        if (fileText[stringNum])
            throw JsonError("more than 1 root element", "EOF", posInFile, posInLine);
    }
}

void JsonChecker::checkString(const std::string& fileText)
{
    stringNum++;
    posInLine++;
    while (fileText[stringNum] != '"') {
        if (!fileText[stringNum] || fileText[stringNum] == '\n') {
            containError = true;
            throw JsonError(STRING_ERROR, "\"", posInFile, posInLine);
            break;
        }
        stringNum++;
        posInLine++;
    }
    stringNum++;
    posInLine++;
}

void JsonChecker::checkSpecialType(const std::string& fileText)
{
    std::string expected;
    if (fileText[stringNum] == 't')
        expected = "true";
    else if (fileText[stringNum] == 'f')
        expected = "false";
    else
        expected = "null";
    for (int i = 0; expected[i]; i++) {
        if (expected[i] != fileText[stringNum]) {
            containError = true;
            throw JsonError(SPECIAL_TYPE_ERROR, expected, posInFile, posInLine);
            break;
        }
        stringNum++;
        posInLine++;
    }
    stringNum++;
    posInLine++;
}

void JsonChecker::checkNumber(const std::string& fileText)
{
    int pointsCounter = 0;
    int minusCounter = 0;
    while (correctSymbols.find(fileText[stringNum]) != std::string::npos) {
        if (fileText[stringNum] == '.')
            pointsCounter++;
        if (pointsCounter > 1) {
            containError = true;
            throw JsonError("incorrect number", "number with one point", posInFile, posInLine);
            break;
        }
        if (fileText[stringNum] == '-')
            minusCounter++;
        if (minusCounter > 1) {
            containError = true;
            throw JsonError("incorrect number", "number with one minus", posInFile, posInLine);
            break;
        }
        stringNum++;
        posInLine++;
    }
}

void JsonChecker::checkObject(const std::string& fileText)
{
    if (containError != true) {
        stringNum++;
        skipEmpty(fileText);
        ExpectedPart part;
        part.current = part.key;
        if (fileText[stringNum] == '}') {
            stringNum++;
            posInLine++;
        } else {
            objectError(fileText, part);
            if (containError != true) {
                stringNum++;
                posInLine++;
            }
        }
    }
}

void JsonChecker::objectError(const std::string& fileText, ExpectedPart part)
{
    while (fileText[stringNum] && containError == false) {
        skipEmpty(fileText);
        if (part.current == part.key && fileText[stringNum] == '"') {
            checkString(fileText); part.current = part.colon;
        } else if (part.current == part.colon && fileText[stringNum] == ':') {
            part.current = part.value; stringNum++; posInLine++;
        } else if (part.current == part.value && fileText[stringNum] == '"') {
            checkString(fileText); part.current = part.endOfLine;
        } else if (part.current == part.value && correctSymbols.find(fileText[stringNum]) != std::string::npos) {
            checkNumber(fileText); part.current = part.endOfLine;
        } else if (part.current == part.value && (fileText[stringNum] == 'f' || fileText[stringNum] == 't' || fileText[stringNum] == 'n')) {
            checkSpecialType(fileText); part.current = part.endOfLine;
        } else if (part.current == part.value && fileText[stringNum] == '{') {
            checkObject(fileText); part.current = part.endOfLine;
        } else if (part.current == part.value && fileText[stringNum] == '[') {
            checkArray(fileText); part.current = part.endOfLine;
        } else if (part.current == part.endOfLine && fileText[stringNum] == ',') {
            stringNum++; posInLine++; part.current = part.key;
        } else if (part.current == part.endOfLine && fileText[stringNum] == '\n') {
            stringNum++; posInFile++; posInLine = 1; part.current = part.endOfObject;
        } else if ((part.current == part.endOfObject || part.current == part.endOfLine) && fileText[stringNum] == '}')
            break;
        else {
            containError = true; throw JsonError("incorrect object ", part.current, posInFile, posInLine);
        }
    }
}

void JsonChecker::checkArray(const std::string& fileText)
{
    if (containError != true) {
        stringNum++;
        posInLine++;
        bool expectValue = true;
        skipEmpty(fileText);
        if (fileText[stringNum] == ']') {
            stringNum++;
            posInLine++;
        } else {
            arrayError(fileText, expectValue);
            if (containError == false){
                stringNum++;
                posInLine++;
            }
        }
    }
}

void JsonChecker::arrayError(const std::string& fileText, bool expectValue)
{
    while (fileText[stringNum] && containError == true) {
        skipEmpty(fileText);
        if (!expectValue && fileText[stringNum] == ']')
            break;
        else if (!expectValue && fileText[stringNum] == '.') {
            stringNum++; posInLine++; expectValue = true;
        } else if (expectValue && fileText[stringNum] == '"') {
            checkString(fileText); expectValue = false;
        } else if (expectValue && fileText[stringNum] == '[') {
            checkArray(fileText); expectValue = false;
        } else if (expectValue && fileText[stringNum] == '{') {
            checkObject(fileText); expectValue = false;
        } else if (expectValue && (fileText[stringNum] == 'f' || fileText[stringNum] == 't' || fileText[stringNum] == 'n')) {
            checkSpecialType(fileText); expectValue = false;
        } else if (expectValue && correctSymbols.find(fileText[stringNum]) != std::string::npos) {
            checkNumber(fileText); expectValue = false;
        } else if (expectValue && (fileText[stringNum] == ',' || fileText[stringNum] == ']')) {
            containError = true; throw JsonError ("incorrect array", "array element", posInFile, posInLine);
        } else if (!expectValue && fileText[stringNum] != ',') {
            containError = true; throw JsonError("incorrect array", "\",\"", posInFile, posInLine);
        } else {
            containError = true; throw JsonError("incorrect array", "end of array", posInFile, posInLine);
        }
    }
}

void JsonChecker::skipEmpty(const std::string& fileText)
{
    while (fileText[stringNum] == ' ' || fileText[stringNum] == '\n' || fileText[stringNum] == '\t') {
        if (fileText[stringNum] == '\n') {
            posInFile++;
            posInLine = 1;
        }
        else
            posInLine++;
        stringNum++;
    }
}
