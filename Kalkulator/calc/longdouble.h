#ifndef LONGDOUBLE_H
#define LONGDOUBLE_H


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class LongDouble {
    const size_t divDigits = 100;
    const size_t sqrtDigits = 100;
    char dec_digits[10] = { '0', '1', '2', '3', '4',
                            '5', '6', '7', '8', '9' };

    int sign;
    std::vector<int> digits;
    long exponent;

    void initFromString(const std::string& s);
    void removeZeroes();
    void normalize();

public:
    LongDouble();
    LongDouble(const LongDouble& x);
    LongDouble(long double value);
    LongDouble(const std::string& s);

    LongDouble& operator=(const LongDouble& x);

    bool operator>(const LongDouble& x) const;
    bool operator<(const LongDouble& x) const;
    bool operator>=(const LongDouble& x) const;
    bool operator<=(const LongDouble& x) const;
    bool operator==(const LongDouble& x) const;
    bool operator!=(const LongDouble& x) const;

    LongDouble operator-() const;

    LongDouble operator+(const LongDouble& x) const;
    LongDouble operator-(const LongDouble& x) const;
    LongDouble operator*(const LongDouble& x) const;
    LongDouble operator/(const LongDouble& x) const;

    LongDouble& operator+=(const LongDouble& x);
    LongDouble& operator-=(const LongDouble& x);
    LongDouble& operator*=(const LongDouble& x);
    LongDouble& operator/=(const LongDouble& x);

    LongDouble operator++(int);
    LongDouble operator--(int);

    LongDouble& operator++();
    LongDouble& operator--();

    LongDouble inverse() const;
    LongDouble sqrt() const;
    LongDouble pow(const LongDouble& n) const;
    LongDouble abs() const;

    bool isInteger() const;
    bool isEven() const;
    bool isOdd() const;
    bool isZero() const;

    friend std::ostream& operator<<(std::ostream& os, const LongDouble& value);

    void print_vector();
    void print_exponent();
    void print_fractional_part();

    string convert_fractional_part_to_string();
    string convert_integer_part_to_string();
    LongDouble trunc(const LongDouble &number);
};

#endif // LONGDOUBLE_H
