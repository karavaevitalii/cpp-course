#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <limits>
#include <vector>
#include "storage.h"

struct big_integer
{
private:
    bool sign_;
    storage data_;
    void trim();
    void add_long_short(size_t const);
    void mul_long_short(size_t const);
    bool is_zero() const;
    int compare_by_abs(big_integer const&) const;
    int compare(big_integer const&) const;
    void add_unsigned(big_integer const&);
    void sub_long_short(size_t const);
    void sub_unsigned(big_integer const&);
    big_integer mul_long_short_return(size_t const) const;
    size_t div_long_short(size_t const);
    void code();
    void decode();
public:
    big_integer();
    big_integer(big_integer const&);
    big_integer(int);
    explicit big_integer(std::string const&);
    ~big_integer();
    big_integer& operator =(big_integer const&);
    friend bool operator ==(big_integer const&, big_integer const&);
    friend bool operator !=(big_integer const&, big_integer const&);
    friend bool operator >(big_integer const&, big_integer const&);
    friend bool operator >=(big_integer const&, big_integer const&);
    friend bool operator <(big_integer const&, big_integer const&);
    friend bool operator <=(big_integer const&, big_integer const&);
    big_integer& operator +=(big_integer const&);
    big_integer& operator -=(big_integer const&);
    big_integer operator +() const;
    big_integer operator -() const;
    big_integer& operator *=(big_integer const&);
    big_integer& operator /=(big_integer const&);
    big_integer& operator %=(big_integer const&);
    big_integer& operator ++();
    big_integer operator ++(int);
    big_integer& operator --();
    big_integer operator --(int);
    big_integer& operator &=(big_integer const&);
    big_integer& operator |=(big_integer const&);
    big_integer& operator ^=(big_integer const&);
    big_integer operator ~() const;
    big_integer& operator >>=(int);
    big_integer& operator <<=(int);
    friend std::string to_string(big_integer const&);
};
bool operator ==(big_integer const&, big_integer const&);
bool operator !=(big_integer const&, big_integer const&);
bool operator >(big_integer const&, big_integer const&);
bool operator >=(big_integer const&, big_integer const&);
bool operator <(big_integer const&, big_integer const&);
bool operator <=(big_integer const&, big_integer const&);
big_integer operator +(big_integer, big_integer const&);
big_integer operator -(big_integer, big_integer const&);
big_integer operator *(big_integer, big_integer const&);
big_integer operator /(big_integer, big_integer const&);
big_integer operator %(big_integer, big_integer const&);
big_integer operator &(big_integer, big_integer const&);
big_integer operator |(big_integer, big_integer const&);
big_integer operator ^(big_integer, big_integer const&);
big_integer operator >>(big_integer, int);
big_integer operator <<(big_integer, int);
std::string to_string(big_integer const&);
std::ostream& operator <<(std::ostream&, big_integer const&);

#endif //BIG_INTEGER_H
