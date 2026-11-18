#include <iostream>
#include <string>
#include <windows.h>
#include "bankOperations.h"
#include "utils.h"

static void searchMenu(BankOperations &bank)
{
    while (true)
    {
        std::cout << "\n====== Поиск и фильтры ======\n"
                     "[1] - Поиск по ID (map)\n"
                     "[2] - Быстрый поиск по ID (unordered_map)\n"
                     "[3] - Проверить наличие ID (unordered_set)\n"
                     "[4] - Операции с точной суммой (multiset, equal_range)\n"
                     "[5] - Операции в диапазоне сумм (multiset, lower_bound/upper_bound)\n"
                     "[6] - Операции счёта отправителя (unordered_multimap, equal_range)\n"
                     "[7] - Фильтр по типу операции\n"
                     "[8] - Количество операций типа (unordered_multiset, count)\n"
                     "[0] - Назад\n";
        int choice = Utils::readInt("Выбор: ", 0, 8);
        if (choice == 0)
            return;

        if (choice == 1)
            bank.find(Utils::readLine("ID операции: "));
        else if (choice == 2)
            bank.findFast(Utils::readLine("ID операции: "), std::cout);
        else if (choice == 3)
            std::cout << (bank.exists(Utils::readLine("ID операции: ")) ? "Есть\n" : "Нет\n");
        else if (choice == 4)
        {
            double a = Utils::readDouble("Сумма: ", 0.01, 1000000000);
            bank.printByAmount(a, std::cout);
        }
        else if (choice == 5)
        {
            double lo = Utils::readDouble("От: ", 0.01, 1000000000);
            double hi = Utils::readDouble("До: ", lo, 1000000000);
            bank.printAmountRange(lo, hi, std::cout);
        }
        else if (choice == 6)
            bank.printByAccount(Utils::readLine("Счёт отправителя: "), std::cout);
        else if (choice == 7)
            bank.filterByType(Utils::readLine("Тип операции: "), std::cout);
        else if (choice == 8)
            bank.printTypeCount(Utils::readLine("Тип операции: "), std::cout);
    }
}

static void viewMenu(const BankOperations &bank)
{
    while (true)
    {
        std::cout << "\n====== Просмотр по контейнерам ======\n"
                     "[1] - map (по ID)\n"
                     "[2] - multimap (по возрастанию суммы)\n"
                     "[3] - multimap (по убыванию суммы)\n"
                     "[4] - set (по ID)\n"
                     "[5] - multiset (по возрастанию суммы)\n"
                     "[6] - multiset (по убыванию суммы)\n"
                     "[7] - unordered_map\n"
                     "[8] - unordered_multimap\n"
                     "[9] - unordered_set\n"
                     "[10] - unordered_multiset (типы операций)\n"
                     "[0] - Назад\n";
        int choice = Utils::readInt("Выбор: ", 0, 10);
        if (choice == 0)
            return;

        if (choice == 1)
            bank.printMap(std::cout);
        else if (choice == 2)
            bank.printAsc(std::cout);
        else if (choice == 3)
            bank.printDesc(std::cout);
        else if (choice == 4)
            bank.printSet(std::cout);
        else if (choice == 5)
            bank.printMultisetAsc(std::cout);
        else if (choice == 6)
            bank.printMultisetDesc(std::cout);
        else if (choice == 7)
            bank.printUnorderedMap(std::cout);
        else if (choice == 8)
            bank.printUnorderedMultimap(std::cout);
        else if (choice == 9)
            bank.printUnorderedSet(std::cout);
        else if (choice == 10)
            bank.printTypes(std::cout);
    }
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    BankOperations bank;
    while (true)
    {
        std::cout << "\n====== Учёт банковских операций ======\n"
                     "[1] - Добавить операцию\n"
                     "[2] - Удалить операцию\n"
                     "[3] - Редактировать операцию\n"
                     "[4] - Поиск и фильтры\n"
                     "[5] - Просмотр по контейнерам\n"
                     "[6] - Сохранить в файл\n"
                     "[0] - Выход\n";
        int choice = Utils::readInt("Выбор: ", 0, 6);
        if (choice == 0)
            break;

        if (choice == 1)
        {
            do
            {
                std::string from = Utils::readLine("Счёт отправителя: ");
                std::string to = Utils::readLine("Счёт получателя: ");
                double amount = Utils::readDouble("Сумма: ", 0.01, 1000000000);
                std::string type = Utils::readLine("Тип операции: ");
                bank.add(Transaction(from, to, amount, type));
                std::cout << "Операция добавлена\n";
            } while (Utils::askContinue("Добавить ещё одну операцию?"));
        }

        if (choice == 2)
        {
            std::string id = Utils::readLine("ID операции: ");
            std::cout << (bank.remove(id) ? "Удалено\n" : "Не найдено\n");
        }
        if (choice == 3)
        {
            std::string id = Utils::readLine("ID операции: ");
            double amount = Utils::readDouble("Новая сумма: ", 0.01, 1000000000);
            std::string type = Utils::readLine("Новый тип: ");
            if (choice == 3)
            {
                std::string id = Utils::readLine("ID операции: ");
                double amount = Utils::readDouble("Новая сумма: ", 0.01, 1000000000);
                std::string type = Utils::readLine("Новый тип: ");
                if (bank.edit(id, amount, type))
                    std::cout << "Изменено\n";
                else
                    std::cout << "Не найдено\n";
            }
        }
        if (choice == 4)
            searchMenu(bank);
        if (choice == 5)
            viewMenu(bank);
        if (choice == 6)
        {
            std::string path = Utils::readLine("Имя файла (например, out.txt): ");
            if (bank.saveToFile(path))
                std::cout << "Сохранено\n";
            else
                std::cout << "Не удалось открыть файл\n";
        }
    }
    return 0;
}