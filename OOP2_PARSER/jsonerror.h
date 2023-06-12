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
    int numInCols;
    int numInRows;
public:
    JsonError(const std::string& errorType, std::string expecredString, const int numInCols, const int numInRows):
        errorType {errorType}, expectedString {expectedString}, numInCols {numInCols}, numInRows {numInRows} {}
    const char* what() const noexcept final {
        std::string message = "Error:" + errorType + "in position: " + std::to_string(numInCols) + ", " + std::to_string(numInRows)  + ".\n" +
                "Expected string: " + expectedString + ". ";
        errorMessage = message;
        return errorMessage.c_str();
    }
};

#endif // JSONERROR_H
