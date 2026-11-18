#include <iostream>
#include "utils.h"
#include <ctime>

std::string Utils::currentDate()
{
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M", std::localtime(&t));
    return std::string(buf);
}

int Utils::readInt(const std::string &prompt, int minVal, int maxVal)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good() && value <= maxVal && value >= minVal)
        {
            std::cin.ignore(1000, '\n');
            break;
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Ошибка ввода. Введите число от " << minVal << " до " << maxVal << ".\n";
    }
    return value;
}


bool Utils::isValidLogin(const std::string &login)
{
    if (login.size() < 3 || login.size() > 30)
        return false;

    for (size_t i = 0; i < login.size(); i++)
    {
        if (!((login[i] >= 'A' && login[i] <= 'Z') || (login[i] >= 'a' && login[i] <= 'z') 
        || login[i] == '_'||(login[i] >= '0' && login[i] <= '9')))
            return false;
    }

    if (login[0] == '-' || login[login.size() - 1] == '-')
        return false;
    return true;
}

double Utils::readDouble(const std::string &prompt, double minVal, double maxVal)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good() && value >= minVal && value <= maxVal)
        {
            std::cin.ignore(1000, '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Ошибка ввода. Введите число от " << minVal << " до " << maxVal << ".\n";
    }
}

std::string Utils::readLine(const std::string &prompt)
{
    std::string s;
    do
    {
        std::cout << prompt;
        std::getline(std::cin, s);
    } while (s.empty());
    return s;
}

bool Utils::askContinue(const std::string &prompt)
{
    return readInt(prompt + " (1 - да, 0 - нет): ", 0, 1) == 1;
}