#include "SIGN.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace {
    bool isLeapYear(int year) {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    bool isValidDate(int day, int month, int year) {
        if (year < 1 || month < 1 || month > 12 || day < 1) {
            return false;
        }

        const int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                                   31, 31, 30, 31, 30, 31};
        int maxDay = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(year)) {
            maxDay = 29;
        }
        return day <= maxDay;
    }

    std::string expectedZodiac(int day, int month) {
        if ((month == 3 && day >= 21) || (month == 4 && day <= 20)) return "Овен";
        if ((month == 4 && day >= 21) || (month == 5 && day <= 21)) return "Телец";
        if ((month == 5 && day >= 22) || (month == 6 && day <= 21)) return "Близнецы";
        if ((month == 6 && day >= 22) || (month == 7 && day <= 22)) return "Рак";
        if ((month == 7 && day >= 23) || (month == 8 && day <= 23)) return "Лев";
        if ((month == 8 && day >= 24) || (month == 9 && day <= 23)) return "Дева";
        if ((month == 9 && day >= 24) || (month == 10 && day <= 23)) return "Весы";
        if ((month == 10 && day >= 24) || (month == 11 && day <= 22)) return "Скорпион";
        if ((month == 11 && day >= 23) || (month == 12 && day <= 21)) return "Стрелец";
        if ((month == 12 && day >= 22) || (month == 1 && day <= 20)) return "Козерог";
        if ((month == 1 && day >= 21) || (month == 2 && day <= 19)) return "Водолей";
        return "Рыбы";
    }
}

std::string SIGN::normalizeZodiacSign(const std::string& sign) {
    if (sign == "Овен" || sign == "овен" || sign == "ОВЕН") return "Овен";
    if (sign == "Телец" || sign == "телец" || sign == "ТЕЛЕЦ") return "Телец";
    if (sign == "Близнецы" || sign == "близнецы" || sign == "БЛИЗНЕЦЫ") return "Близнецы";
    if (sign == "Рак" || sign == "рак" || sign == "РАК") return "Рак";
    if (sign == "Лев" || sign == "лев" || sign == "ЛЕВ") return "Лев";
    if (sign == "Дева" || sign == "дева" || sign == "ДЕВА") return "Дева";
    if (sign == "Весы" || sign == "весы" || sign == "ВЕСЫ") return "Весы";
    if (sign == "Скорпион" || sign == "скорпион" || sign == "СКОРПИОН") return "Скорпион";
    if (sign == "Стрелец" || sign == "стрелец" || sign == "СТРЕЛЕЦ") return "Стрелец";
    if (sign == "Козерог" || sign == "козерог" || sign == "КОЗЕРОГ") return "Козерог";
    if (sign == "Водолей" || sign == "водолей" || sign == "ВОДОЛЕЙ") return "Водолей";
    if (sign == "Рыбы" || sign == "рыбы" || sign == "РЫБЫ") return "Рыбы";
    throw std::invalid_argument("Неизвестный знак зодиака.");
}

SIGN::SIGN()
    : fullName("Не указано"), zodiacSign("Не указан"), birthDate{1, 1, 2000} {
}

SIGN::SIGN(const std::string& name, const std::string& sign,
           int day, int month, int year)
    : fullName(name), zodiacSign(normalizeZodiacSign(sign)), birthDate{day, month, year} {
    if (name.empty()) {
        throw std::invalid_argument("Фамилия и имя не могут быть пустыми.");
    }
    if (sign.empty()) {
        throw std::invalid_argument("Знак зодиака не может быть пустым.");
    }
    if (!isValidDate(day, month, year)) {
        throw std::invalid_argument("Некорректная дата рождения.");
    }
}

SIGN::SIGN(const SIGN& other)
    : fullName(other.fullName), zodiacSign(other.zodiacSign),
      birthDate{other.birthDate[0], other.birthDate[1], other.birthDate[2]} {
}

SIGN& SIGN::operator=(const SIGN& other) {
    if (this != &other) {
        fullName = other.fullName;
        zodiacSign = other.zodiacSign;
        birthDate[0] = other.birthDate[0];
        birthDate[1] = other.birthDate[1];
        birthDate[2] = other.birthDate[2];
    }
    return *this;
}

SIGN::~SIGN() {
}

void SIGN::setFullName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Фамилия и имя не могут быть пустыми.");
    }
    fullName = name;
}

void SIGN::setZodiacSign(const std::string& sign) {
    if (sign.empty()) {
        throw std::invalid_argument("Знак зодиака не может быть пустым.");
    }
    zodiacSign = normalizeZodiacSign(sign);
}

void SIGN::setBirthDate(int day, int month, int year) {
    if (!isValidDate(day, month, year)) {
        throw std::invalid_argument("Некорректная дата рождения.");
    }
    birthDate[0] = day;
    birthDate[1] = month;
    birthDate[2] = year;
}

const std::string& SIGN::getFullName() const { return fullName; }
const std::string& SIGN::getZodiacSign() const { return zodiacSign; }
int SIGN::getBirthDay() const { return birthDate[0]; }
int SIGN::getBirthMonth() const { return birthDate[1]; }
int SIGN::getBirthYear() const { return birthDate[2]; }

std::string SIGN::getZodiacForDate(int day, int month) {
    return expectedZodiac(day, month);
}

void SIGN::print() const {
    std::cout << *this;
}

std::istream& operator>>(std::istream& in, SIGN& person) {
    std::string name;
    std::string sign;
    std::string date;

    std::getline(in >> std::ws, name);
    if (!in) return in;
    std::getline(in >> std::ws, sign);
    std::getline(in >> std::ws, date);

    std::istringstream dateStream(date);
    char firstDot = 0;
    char secondDot = 0;
    int day = 0;
    int month = 0;
    int year = 0;

    if (!(dateStream >> day >> firstDot >> month >> secondDot >> year) ||
        firstDot != '.' || secondDot != '.') {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        person.setFullName(name);
        person.setZodiacSign(sign);
        person.setBirthDate(day, month, year);

        const std::string correctSign = SIGN::getZodiacForDate(day, month);
        if (person.getZodiacSign() != correctSign) {
            throw std::invalid_argument(
                "Дата " + date + " не соответствует знаку зодиака " + sign + ". Для этой даты знак — " + correctSign + ".");
        }
    } catch (const std::exception&) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const SIGN& person) {
    out << person.fullName << " | "
        << person.zodiacSign << " | "
        << std::setfill('0') << std::setw(2) << person.birthDate[0] << "."
        << std::setw(2) << person.birthDate[1] << "."
        << std::setw(4) << person.birthDate[2] << std::setfill(' ');
    return out;
}
