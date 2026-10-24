#ifndef SIGN_ARRAY_H
#define SIGN_ARRAY_H

#include "SIGN.h"

class SignArray {
private:
    SIGN** data;
    int size;
    int capacity;

    void resize(int newCapacity);
    void checkIndex(int index) const;

public:
    SignArray();
    explicit SignArray(int initialCapacity);
    SignArray(const SignArray& other);
    ~SignArray();

    SignArray& operator=(const SignArray& other);

    int getSize() const;
    bool empty() const;

    void add(const SIGN& person, int position);
    void remove(int position);
    void edit(int position, const SIGN& person);
    const SIGN& get(int position) const;
    SIGN& get(int position);

    void sortByBirthDate();
    void printAll() const;
};

#endif
