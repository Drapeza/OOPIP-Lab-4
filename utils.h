#pragma once
#include <string>

namespace Utils
{
    std::string currentDate();
    int readInt(const std::string &prompt, int minVal, int maxVal);
    bool isValidLogin(const std::string &login);

    double readDouble(const std::string &prompt, double minVal, double maxVal);
    std::string readLine(const std::string &prompt);
    bool askContinue(const std::string &prompt);
}
