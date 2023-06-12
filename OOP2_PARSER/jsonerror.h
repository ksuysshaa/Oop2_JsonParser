#ifndef JSONERROR_H
#define JSONERROR_H
#include "iostream"

class SystemError : public std::exception
{
private:
    std::string errorMessage;
public:
    SystemError (const std::string& message) : errorMessage{message} {}
    const char* what() const noexcept final {
        return errorMessage.c_str();
    }
};

class JsonError : public std::exception
{
private:
    mutable std::string errorMessage;
    std::string errorType;
    std::string expectedString;
    int posInFile;
    int posInLine;
public:
    JsonError(const std::string& errorType, std::string expectedString, const int posInFile, const int posInLine):
        errorType {errorType}, expectedString {expectedString}, posInFile {posInFile}, posInLine {posInLine} {}
    const char* what() const noexcept final {
        std::string message = "Error: " + errorType + " in position: " + std::to_string(posInFile) + ", " + std::to_string(posInLine)  + ".\n" +
                "Expected: " + expectedString + ". ";
        errorMessage = message;
        return errorMessage.c_str();
    }
};

#endif // JSONERROR_H
