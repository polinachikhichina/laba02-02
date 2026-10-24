#ifndef SIGN_H
#define SIGN_H

#include <iostream>
#include <string>

class SIGN {
private:
    std::string fullName;
    std::string zodiacSign;
    int birthDate[3];

public:
    SIGN();
    SIGN(const std::string& name, const std::string& sign,
         int day, int month, int year);
    SIGN(const SIGN& other);
    ~SIGN();

    SIGN& operator=(const SIGN& other);

    void setFullName(const std::string& name);
    void setZodiacSign(const std::string& sign);
    void setBirthDate(int day, int month, int year);

    const std::string& getFullName() const;
    const std::string& getZodiacSign() const;
    int getBirthDay() const;
    int getBirthMonth() const;
    int getBirthYear() const;
    static std::string getZodiacForDate(int day, int month);
    static std::string normalizeZodiacSign(const std::string& sign);

    void print() const;

    friend std::istream& operator>>(std::istream& in, SIGN& person);
    friend std::ostream& operator<<(std::ostream& out, const SIGN& person);
};

#endif
