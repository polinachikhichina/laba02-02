#include "SignArray.h"
#include <iostream>
#include <stdexcept>

namespace {
    bool earlier(const SIGN& a, const SIGN& b) {
        if (a.getBirthYear() != b.getBirthYear())
            return a.getBirthYear() < b.getBirthYear();
        if (a.getBirthMonth() != b.getBirthMonth())
            return a.getBirthMonth() < b.getBirthMonth();
        return a.getBirthDay() < b.getBirthDay();
    }
}

SignArray::SignArray() : data(new SIGN*[2]), size(0), capacity(2) {
    for (int i = 0; i < capacity; ++i) data[i] = nullptr;
}

SignArray::SignArray(int initialCapacity)
    : data(nullptr), size(0), capacity(initialCapacity < 1 ? 2 : initialCapacity) {
    data = new SIGN*[capacity];
    for (int i = 0; i < capacity; ++i) data[i] = nullptr;
}

SignArray::SignArray(const SignArray& other)
    : data(new SIGN*[other.capacity]), size(other.size), capacity(other.capacity) {
    for (int i = 0; i < capacity; ++i) data[i] = nullptr;
    for (int i = 0; i < size; ++i) data[i] = new SIGN(*other.data[i]);
}

SignArray::~SignArray() {
    for (int i = 0; i < size; ++i) delete data[i];
    delete[] data;
}

SignArray& SignArray::operator=(const SignArray& other) {
    if (this == &other) return *this;

    for (int i = 0; i < size; ++i) delete data[i];
    delete[] data;

    size = other.size;
    capacity = other.capacity;
    data = new SIGN*[capacity];
    for (int i = 0; i < capacity; ++i) data[i] = nullptr;
    for (int i = 0; i < size; ++i) data[i] = new SIGN(*other.data[i]);

    return *this;
}

void SignArray::resize(int newCapacity) {
    SIGN** newData = new SIGN*[newCapacity];
    for (int i = 0; i < newCapacity; ++i) newData[i] = nullptr;
    for (int i = 0; i < size; ++i) newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void SignArray::checkIndex(int index) const {
    if (index < 0 || index >= size)
        throw std::out_of_range("Такой записи не существует.");
}

int SignArray::getSize() const { return size; }
bool SignArray::empty() const { return size == 0; }

void SignArray::add(const SIGN& person, int position) {
    if (position < 0 || position > size)
        throw std::out_of_range("Позиция должна быть от 1 до количества записей + 1.");

    if (size == capacity) resize(capacity * 2);

    for (int i = size; i > position; --i) data[i] = data[i - 1];
    data[position] = new SIGN(person);
    ++size;
}

void SignArray::remove(int position) {
    checkIndex(position);
    delete data[position];
    for (int i = position; i < size - 1; ++i) data[i] = data[i + 1];
    data[size - 1] = nullptr;
    --size;
}

void SignArray::edit(int position, const SIGN& person) {
    checkIndex(position);
    *data[position] = person;
}

const SIGN& SignArray::get(int position) const {
    checkIndex(position);
    return *data[position];
}

SIGN& SignArray::get(int position) {
    checkIndex(position);
    return *data[position];
}

void SignArray::sortByBirthDate() {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (!earlier(*data[j], *data[j + 1])) {
                SIGN* temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

void SignArray::printAll() const {
    if (empty()) {
        std::cout << "Записей пока нет.\n";
        return;
    }

    std::cout << "\n№ | Фамилия и имя | Знак зодиака | Дата рождения\n";
    std::cout << "-------------------------------------------------------------\n";
    for (int i = 0; i < size; ++i)
        std::cout << i + 1 << " | " << *data[i] << "\n";
}
