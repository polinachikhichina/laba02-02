#include "SIGN.h"
#include "SignArray.h"
#include "TextProcessor.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <string>
#include <fstream>

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& message)
{
    int value;

    std::cout << message;

    while (!(std::cin >> value))
    {
        if (std::cin.eof())
        {
            throw std::runtime_error("Ввод завершён.");
        }

        std::cout << "Ошибка: введите целое число.\n";
        clearInput();
        std::cout << message;
    }

    clearInput();

    return value;
}

SIGN readSign()
{
    std::string name;
    std::string sign;
    std::string date;

    std::cout << "Фамилия и имя: ";
    std::getline(std::cin, name);

    std::cout << "Знак зодиака: ";
    std::getline(std::cin, sign);

    std::cout << "Дата рождения (ДД.ММ.ГГГГ): ";
    std::getline(std::cin, date);

    std::istringstream dateStream(date);

    int day = 0;
    int month = 0;
    int year = 0;

    char firstDot = 0;
    char secondDot = 0;

    if (!(dateStream >> day >> firstDot >> month >> secondDot >> year) ||
        firstDot != '.' || secondDot != '.')
    {
        throw std::invalid_argument(
            "Дата должна быть в формате ДД.ММ.ГГГГ.");
    }

    SIGN person(name, sign, day, month, year);

    const std::string correctSign =
        SIGN::getZodiacForDate(day, month);

    if (person.getZodiacSign() != correctSign)
    {
        throw std::invalid_argument(
            "Дата " + date +
            " не соответствует знаку зодиака " + sign +
            ". Для этой даты знак — " + correctSign + ".");
    }

    return person;
}

void addRecord(SignArray& records)
{
    std::cout << "\n========== ДОБАВЛЕНИЕ ЗАПИСИ ==========\n";

    SIGN person = readSign();

    std::string positionMessage;

    if (records.empty())
    {
        positionMessage =
            "Введите позицию (1 - единственная позиция): ";
    }
    else
    {
        positionMessage =
            "Введите позицию (1 - начало, " +
            std::to_string(records.getSize() + 1) +
            " - конец): ";
    }

    int position = readInt(positionMessage);

    records.add(person, position - 1);

    std::cout << "Запись добавлена.\n";
}

void editRecord(SignArray& records)
{
    if (records.empty())
    {
        std::cout << "Записей для редактирования нет.\n";
        return;
    }

    int position =
        readInt("Введите номер записи для редактирования: ");

    SIGN& person = records.get(position - 1);

    std::cout << "\nЧто изменить?\n";
    std::cout << "1. Фамилию и имя\n";
    std::cout << "2. Знак зодиака\n";
    std::cout << "3. Дату рождения\n";
    std::cout << "4. Все данные\n";
    std::cout << "0. Отмена\n";

    int choice = readInt("Выберите пункт: ");

    switch (choice)
    {
        case 1:
        {
            std::string name;

            std::cout << "Новая фамилия и имя: ";
            std::getline(std::cin, name);

            person.setFullName(name);

            std::cout << "Фамилия и имя изменены.\n";

            break;
        }

        case 2:
        {
            std::string sign;

            std::cout << "Новый знак зодиака: ";
            std::getline(std::cin, sign);

            sign = SIGN::normalizeZodiacSign(sign);

            std::string correctSign =
                SIGN::getZodiacForDate(
                    person.getBirthDay(),
                    person.getBirthMonth());

            if (sign != correctSign)
            {
                throw std::invalid_argument(
                    "Знак не соответствует текущей дате рождения. "
                    "Для этой даты знак — " +
                    correctSign + ".");
            }

            person.setZodiacSign(sign);

            std::cout << "Знак зодиака изменён.\n";

            break;
        }

        case 3:
        {
            std::string date;

            std::cout << "Новая дата рождения (ДД.ММ.ГГГГ): ";
            std::getline(std::cin, date);

            std::istringstream dateStream(date);

            int day = 0;
            int month = 0;
            int year = 0;

            char firstDot = 0;
            char secondDot = 0;

            if (!(dateStream >> day >> firstDot >>
                  month >> secondDot >> year) ||
                firstDot != '.' || secondDot != '.')
            {
                throw std::invalid_argument(
                    "Дата должна быть в формате ДД.ММ.ГГГГ.");
            }

            // Определяем новый знак по новой дате
            std::string newSign =
                SIGN::getZodiacForDate(day, month);

            // Меняем дату
            person.setBirthDate(day, month, year);

            // Автоматически меняем знак
            person.setZodiacSign(newSign);

            std::cout << "Дата рождения изменена.\n";

            std::cout
                << "Знак зодиака автоматически изменён на "
                << newSign << ".\n";

            break;
        }

        case 4:
        {
            SIGN newPerson = readSign();

            person = newPerson;

            std::cout << "Все данные изменены.\n";

            break;
        }

        case 0:
        {
            std::cout << "Редактирование отменено.\n";

            break;
        }

        default:
        {
            std::cout << "Такого пункта меню нет.\n";

            break;
        }
    }
}

void deleteRecord(SignArray& records)
{
    if (records.empty())
    {
        std::cout << "Записей для удаления нет.\n";
        return;
    }

    int position =
        readInt("Введите номер записи для удаления: ");

    records.remove(position - 1);

    std::cout << "Запись удалена.\n";
}

void findByZodiac(const SignArray& records)
{
    if (records.empty())
    {
        std::cout << "Записей пока нет.\n";
        return;
    }

    std::string sign;

    std::cout << "Введите знак зодиака: ";
    std::getline(std::cin, sign);

    try
    {
        sign = SIGN::normalizeZodiacSign(sign);
    }
    catch (const std::exception& error)
    {
        std::cout << "Ошибка: "
                  << error.what() << "\n";

        return;
    }

    bool found = false;

    std::cout << "\nЛюди со знаком \""
              << sign << "\":\n";

    for (int i = 0; i < records.getSize(); ++i)
    {
        if (records.get(i).getZodiacSign() == sign)
        {
            std::cout << records.get(i) << "\n";
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "Таких людей нет.\n";
    }
}

void signMenu(SignArray& records)
{
    while (true)
    {
        std::cout
            << "\n========== ЗАДАНИЕ 1: SIGN ==========\n";

        std::cout
            << "1. Добавить запись на любую позицию\n"
            << "2. Вывести все записи\n"
            << "3. Редактировать запись\n"
            << "4. Удалить запись\n"
            << "5. Упорядочить записи по датам рождения\n"
            << "6. Найти людей по знаку зодиака\n"
            << "0. Назад\n";

        int choice =
            readInt("Выберите пункт: ");

        try
        {
            switch (choice)
            {
                case 1:
                    addRecord(records);
                    break;

                case 2:
                    records.printAll();
                    break;

                case 3:
                    editRecord(records);
                    break;

                case 4:
                    deleteRecord(records);
                    break;

                case 5:
                    records.sortByBirthDate();

                    std::cout
                        << "Записи упорядочены по датам рождения.\n";

                    break;

                case 6:
                    findByZodiac(records);
                    break;

                case 0:
                    return;

                default:
                    std::cout
                        << "Такого пункта меню нет.\n";
            }
        }
        catch (const std::exception& error)
        {
            if (std::string(error.what()) == "Ввод завершён.")
            {
                return;
            }

            std::cout
                << "Ошибка: "
                << error.what()
                << "\n";
        }
    }
}

void createTestFile()
{
    std::ofstream file("input.txt");

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Не удалось создать input.txt.");
    }

    std::cout
        << "\n========== СОЗДАНИЕ ФАЙЛА input.txt ==========\n";

    std::cout
        << "Введите предложения с клавиатуры.\n";

    std::cout
        << "Можно вводить несколько строк. "
        << "Пустая строка завершает ввод.\n";

    std::string line;

    bool hasText = false;

    while (true)
    {
        std::cout << "> ";

        std::getline(std::cin, line);

        if (line.empty())
        {
            break;
        }

        if (hasText)
        {
            file << ' ';
        }

        file << line;

        hasText = true;
    }

    file.close();

    if (!hasText)
    {
        throw std::invalid_argument(
            "Файл не создан: не введено ни одного предложения.");
    }

    std::cout
        << "Текст сохранён в input.txt.\n";
}

void textMenu()
{
    std::string fileName;

    std::cout << "Введите имя файла: ";
    std::getline(std::cin, fileName);

    TextProcessor processor(fileName);

    processor.process();
}

int main()
{
    SignArray records;

    while (true)
    {
        std::cout
            << "\n1. Задание SIGN\n"
            << "2. Файловые и строковые потоки\n"
            << "3. Создать тестовый файл input.txt\n"
            << "0. Выход\n";

        int choice =
            readInt("Выберите пункт: ");

        try
        {
            switch (choice)
            {
                case 1:
                    signMenu(records);
                    break;

                case 2:
                    textMenu();
                    break;

                case 3:
                    createTestFile();
                    break;

                case 0:
                    return 0;

                default:
                    std::cout
                        << "Такого пункта меню нет.\n";
            }
        }
        catch (const std::exception& error)
        {
            if (std::string(error.what()) == "Ввод завершён.")
            {
                return 0;
            }

            std::cout
                << "Ошибка: "
                << error.what()
                << "\n";
        }
    }
}