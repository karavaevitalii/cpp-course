#include <algorithm>

#include "includes/big_integer.h"
#include "test/gtest/gtest.h"

typedef __uint128_t u128;

big_integer::big_integer()
    : sign_(true)
    , data_(0)
{}
big_integer::big_integer(big_integer const& other)
    : sign_(other.sign_)
    , data_(other.data_)
{}
big_integer::big_integer(int number)
    : sign_(true)
{
    int64_t tmp = number;
    if (number < 0)
    {
        sign_ = false;
        tmp = -tmp;
    }
    data_.resize(1, static_cast<size_t>(tmp));
}
void big_integer::add_long_short(size_t const x)
{
    u128 result = data_[0] + static_cast<u128>(x);
    data_[0] = static_cast<size_t>(result);
    size_t carry = static_cast<size_t>(result >> std::numeric_limits<size_t>::digits);
    size_t position = 1;
    while (carry != 0)
    {
        if (position == data_.size())
            data_.push_back(0);
        result = static_cast<u128>(data_[position]) + carry;
        data_[position] = static_cast<size_t>(result);
        carry = static_cast<size_t>(result >> std::numeric_limits<size_t>::digits);
        ++position;
    }
}
void big_integer::mul_long_short(size_t const x)
{
    u128 carry = 0;
    u128 result = 0;
    u128 multiplier = static_cast<u128>(x);
    for (size_t i = 0; i != data_.size() || carry > 0; ++i)
    {
        if (i == data_.size())
            data_.push_back(0);
        result = multiplier * data_[i] + carry;
        data_[i] = static_cast<size_t>(result);
        carry = result >> std::numeric_limits<size_t>::digits;
    }
}
big_integer::big_integer(std::string const& str)
    :big_integer(0)
{
    size_t position = 0;
    if (str[position] == '-')
    {
        sign_ = false;
        ++position;
    }
    for (; position != str.length(); ++position)
    {
        size_t digit = static_cast<size_t>(str[position] - '0');
        mul_long_short(10);
        add_long_short(digit);
    }
}
big_integer::~big_integer()
{ }
big_integer& big_integer::operator =(big_integer const& that)
{
    sign_ = that.sign_;
    data_ = that.data_;
    return *this;
}
bool big_integer::is_zero() const
{
    return (data_.empty()) || (data_.size() == 1 && data_[0] == 0);
}
int big_integer::compare_by_abs(big_integer const& that) const
{
    if (this->is_zero() && that.is_zero())
        return 0;
    if (this->data_.size() > that.data_.size())
        return 1;
    if (this->data_.size() < that.data_.size())
        return -1;
    for (size_t i = this->data_.size(); i != 0; --i)
    {
        if (this->data_[i - 1] > that.data_[i - 1])
            return 1;
        if (this->data_[i - 1] < that.data_[i - 1])
            return -1;
    }
    return 0;
}
int big_integer::compare(big_integer const& that) const
{
    if (this->sign_ != that.sign_)
        return this->sign_ ? 1 : -1;
    int ret = this->compare_by_abs(that);
    if (this->sign_ && that.sign_)
        return ret;
    else
        return -ret;
}
bool operator ==(big_integer const& left, big_integer const& right)
{
    return left.compare(right) == 0;
}
bool operator !=(big_integer const& left, big_integer const& right)
{
    return !(left == right);
}
bool operator >(big_integer const& left, big_integer const& right)
{
    return left.compare(right) == 1;
}
bool operator >=(big_integer const& left, big_integer const& right)
{
    return left.compare(right) >= 0;
}
bool operator <(big_integer const& left, big_integer const& right)
{
    return left.compare(right) == -1;
}
bool operator <=(big_integer const& left, big_integer const& right)
{
    return left.compare(right) <= 0;
}
void big_integer::trim() {
    while (!data_.empty() && data_.back() == 0)
        data_.pop_back();
    if (data_.empty())
        sign_ = true;
}
void big_integer::add_unsigned(big_integer const& rhs)
{
    size_t carry = 0;
    u128 result = 0;
    size_t limit = std::max(this->data_.size(), rhs.data_.size());
    for (size_t i = 0; i < limit || carry > 0; ++i)
    {
        if (i == this->data_.size())
            this->data_.push_back(0);
        result = static_cast<u128>(carry) + this->data_[i] + (i < rhs.data_.size()? rhs.data_[i] : 0);
        carry = static_cast<size_t>(result >> std::numeric_limits<size_t>::digits);
        this->data_[i] = static_cast<size_t>(result);
    }
    this->trim();
}
void big_integer::sub_long_short(size_t const x)
{
    __int128_t subtrahend = static_cast<__int128_t>(x);
    __int128_t result = 0;
    for (size_t i = 0; i != data_.size(); ++i)
    {
        if (subtrahend != 0)
        {
            result = data_[i];
            result -= subtrahend;
            if (result >= 0) {
                data_[i] = static_cast<size_t>(result);
                subtrahend = 0;
            }
            else
            {
                result += (static_cast<u128>(std::numeric_limits<size_t>::max()) + 1);
                data_[i] = static_cast<size_t>(result);
            }
        }
    }
}
void big_integer::sub_unsigned(big_integer const& rhs)
{
    bool borrow = false;
    __int128_t result = 0;
    __int128_t minuend = 0;
    __int128_t subtrahend = 0;
    for (size_t i = 0; i != this->data_.size(); ++i)
    {
        minuend = this->data_[i];
        subtrahend = i < rhs.data_.size() ? rhs.data_[i] : 0;
        if (borrow)
        {
            minuend -= 1;
            borrow = false;
        }
        result = minuend - subtrahend;
        if (result < 0)
        {
            result += (static_cast<u128>(std::numeric_limits<size_t>::max()) + 1);
            borrow = true;
        }
        this->data_[i] = static_cast<size_t>(result);
    }
}
big_integer big_integer::operator +() const
{
    return *this;
}
big_integer big_integer::operator -() const
{
    if (*this == 0)
        return *this;
    big_integer ret(*this);
    ret.sign_ = !ret.sign_;
    return ret;
}
big_integer& big_integer::operator +=(big_integer const& rhs)
{
    if (rhs.is_zero())
        return *this;
    if (this->is_zero())
        return *this = rhs;
    if (this->sign_ == rhs.sign_)
        rhs.data_.size() == 1 ? this->add_long_short(rhs.data_.back()) : this->add_unsigned(rhs);
    else
    {
        int compare = this->compare_by_abs(rhs);
        if (compare == 0)
            return *this = 0;
        if (compare == 1)
            rhs.data_.size() == 1 ? this->sub_long_short(rhs.data_.back()) : this->sub_unsigned(rhs);
        else
        {
            big_integer minuend(rhs);
            minuend.sub_unsigned(*this);
            *this = minuend;
            this->sign_ = rhs.sign_;
        }
    }
    this->trim();
    return *this;
}
big_integer operator +(big_integer left, const big_integer& right)
{
    left += right;
    return left;
}
big_integer& big_integer::operator ++()
{
    if (*this > 0)
        add_long_short(1);
    else
        sub_long_short(1);
    return *this;

}
big_integer big_integer::operator ++(int)
{
    big_integer ret(*this);
    ++ret;
    return ret;
}
big_integer& big_integer::operator -=(big_integer const& rhs)
{
    if (rhs.is_zero())
        return *this;
    if (this->is_zero())
        return *this = -rhs;
    if (this->sign_ == rhs.sign_)
    {
        int compare = this->compare_by_abs(rhs);
        if (compare == 0)
            return *this = 0;
        if (compare == 1)
            rhs.data_.size() == 1 ? this->sub_long_short(rhs.data_.back()) : this->sub_unsigned(rhs);
        else
        {
            big_integer minuend(rhs);
            minuend.sub_unsigned(*this);
            *this = minuend;
            this->sign_ = !this->sign_;
        }
    }
    else
        rhs.data_.size() == 1 ? this->sub_long_short(rhs.data_.back()) : this->add_unsigned(rhs);
    this->trim();
    return *this;
}
big_integer operator -(big_integer left, const big_integer& right)
{
    left -= right;
    return left;
}
big_integer& big_integer::operator --()
{
    if (*this < 0)
        add_long_short(1);
    else
        sub_long_short(1);
    return *this;
}
big_integer big_integer::operator --(int)
{
    big_integer ret(*this);
    --ret;
    return ret;
}
big_integer& big_integer::operator <<=(int shift)
{
    size_t insert = static_cast<size_t>(shift) / std::numeric_limits<size_t>::digits;
    data_.insert(0, insert, 0);
    size_t shl = static_cast<size_t>(shift) % std::numeric_limits<size_t>::digits;
    if (shl != 0)
    {
        for (size_t i = data_.size(); i != 1; --i)
            data_[i - 1] = ((data_[i - 1] << shl) |
                (data_[i - 2] >> (std::numeric_limits<size_t>::digits  - shl)));
        data_[0] <<= shl;
    }
    trim();
    return *this;
}
big_integer operator <<(big_integer number, int shift)
{
    number <<= shift;
    return number;
}
big_integer& big_integer::operator >>=(int shift)
{
    size_t del = static_cast<size_t>(shift) / std::numeric_limits<size_t>::digits;
    if (del > data_.size())
        return *this = 0;
    data_.erase(0, del);
    size_t shr = static_cast<size_t>(shift) % std::numeric_limits<size_t>::digits;
    if (shr != 0)
    {
        for (size_t i = 0; i != data_.size() - 1; ++i)
            data_[i] = ((data_[i] >> shr) |
                (data_[i + 1] << (std::numeric_limits<size_t>::digits - shr)));
        data_[data_.size() - 1] >>= shr;
    }
    trim();
    if (*this < 0)
        add_long_short(1);
    return *this;
}
big_integer operator >>(big_integer number, int shift)
{
    number >>= shift;
    return number;
}
big_integer big_integer::mul_long_short_return(size_t const x) const
{
    size_t carry = 0;
    u128 result = 0;
    big_integer ret(*this);
    for (size_t i = 0; i < ret.data_.size() || carry; ++i)
    {
        if (i == ret.data_.size())
            ret.data_.push_back(0);
        result = static_cast<u128>(ret.data_[i]) * x + carry;
        ret.data_[i] = static_cast<size_t>(result);
        carry = static_cast<size_t>(result >> std::numeric_limits<size_t>::digits);
    }
    ret.trim();
    return ret;
}
big_integer& big_integer::operator *=(big_integer const& rhs)
{
    this->sign_ = this->sign_ == rhs.sign_;
    big_integer result(this->mul_long_short_return(rhs.data_[0]));
    for (size_t i = 1; i != rhs.data_.size(); ++i) {
        result += this->mul_long_short_return(rhs.data_[i]) << std::numeric_limits<size_t>::digits * i;
    }
    return *this = result;
}
big_integer operator *(big_integer left, const big_integer& right)
{
    left *= right;
    return left;
}
size_t big_integer::div_long_short(size_t const x)
{
    size_t carry = 0;
    u128 result = 0;
    for (size_t i = data_.size(); i != 0; --i)
    {
        result = data_[i - 1] + carry * (static_cast<u128>(std::numeric_limits<size_t>::max()) + 1);
        data_[i - 1] = static_cast<size_t>(result / x);
        carry = static_cast<size_t>(result % x);
    }
    trim();
    return carry;
}
big_integer& big_integer::operator/=(big_integer const& rhs)
{
    bool flag_sign = this->sign_ == rhs.sign_;
    int compare = this->compare_by_abs(rhs);
    if (compare < 1)
    {
        this->sign_ = flag_sign && (compare != -1);
        *this = (compare == 0);
        return *this;
    }
    if (rhs.data_.size() == 1)
    {
        this->div_long_short(rhs.data_.back());
        this->sign_ = flag_sign;
        return *this;
    }
    big_integer divisor(rhs);
    this->sign_= true;
    divisor.sign_ = true;
    size_t normalize = static_cast<size_t>((static_cast<u128>(std::numeric_limits<size_t>::max()) + 1) / (divisor.data_.back() + 1));
    this->mul_long_short(normalize);
    divisor.mul_long_short(normalize);
    size_t m = this->data_.size() - divisor.data_.size();
    size_t n = divisor.data_.size();
    big_integer quotient;
    quotient.data_.resize(m + 1);
    if (*this >= divisor << (std::numeric_limits<size_t>::digits * m))
    {
        *this -= divisor << (std::numeric_limits<size_t>::digits * m);
        quotient.data_.back() = 1;
    }
    u128 result = 0;
    for (size_t i = m; i != 0; --i)
    {
        result = ((static_cast<u128>(n + i - 1 < this->data_.size() ? this->data_[n + i - 1] : 0)
            << std::numeric_limits<size_t>::digits) +
            this->data_[n + i - 2]) /
            static_cast<u128>(divisor.data_.back());
        quotient.data_[i - 1] = static_cast<size_t>((std::min(result, static_cast<u128>(std::numeric_limits<size_t>::max()))));
        *this -= divisor.mul_long_short_return(quotient.data_[i - 1]) << (std::numeric_limits<size_t>::digits * (i - 1));
        while (*this < 0)
        {
            --quotient.data_[i - 1];
            *this += divisor << (std::numeric_limits<size_t>::digits * (i - 1));
        }
    }
    quotient.sign_ = flag_sign;
    quotient.trim();
    return *this = quotient;
}
big_integer operator /(big_integer left, const big_integer& right)
{
    left /= right;
    return left;
}
big_integer& big_integer::operator %=(const big_integer& rhs)
{
    return *this -= (*this / rhs * rhs);
}
big_integer operator %(big_integer left, const big_integer& right)
{
    left %= right;
    return left;
}
void big_integer::code()
{
    if (!sign_)
    {
        for (size_t i = 0; i != data_.size(); ++i)
            data_[i] = ~data_[i];
        add_long_short(1);
    }
}
void big_integer::decode()
{
    if (!sign_)
    {
        sub_long_short(1);
        for (size_t i = 0; i != data_.size(); ++i)
            data_[i] = ~data_[i];
    }
}
big_integer& big_integer::operator &=(const big_integer& rhs)
{
    big_integer that(rhs);
    while (this->data_.size() > that.data_.size())
        that.data_.push_back(0);
    while (this->data_.size() < that.data_.size())
        this->data_.push_back(0);
    this->code();
    that.code();
    size_t limit = std::max(this->data_.size(), that.data_.size());
    for (size_t i = 0; i != limit; ++i)
        this->data_[i] &= that.data_[i];
    this->sign_ = this->sign_ || that.sign_;
    this->decode();
    this->trim();
    return *this;
}
big_integer operator &(big_integer left, const big_integer& right)
{
    left &= right;
    return left;
}
big_integer& big_integer::operator |=(const big_integer& rhs)
{
    big_integer that(rhs);
    while (this->data_.size() > that.data_.size())
        that.data_.push_back(0);
    while (this->data_.size() < that.data_.size())
        this->data_.push_back(0);
    this->code();
    that.code();
    size_t limit = std::max(this->data_.size(), that.data_.size());
    for (size_t i = 0; i != limit; ++i)
        this->data_[i] |= that.data_[i];
    this->sign_ = this->sign_ && that.sign_;
    this->decode();
    this->trim();
    return *this;
}
big_integer operator |(big_integer left, const big_integer& right)
{
    left |= right;
    return left;
}
big_integer& big_integer::operator ^=(const big_integer& rhs)
{
    big_integer that(rhs);
    while (this->data_.size() > that.data_.size())
        that.data_.push_back(0);
    while (this->data_.size() < that.data_.size())
        this->data_.push_back(0);
    this->code();
    that.code();
    size_t limit = std::max(this->data_.size(), that.data_.size());
    for (size_t i = 0; i != limit; ++i)
        this->data_[i] ^= that.data_[i];
    this->sign_ = !(this->sign_ ^ that.sign_);
    this->decode();
    this->trim();
    return *this;
}
big_integer operator ^(big_integer left, const big_integer& right)
{
    left ^= right;
    return left;
}
big_integer big_integer::operator ~() const
{
    big_integer ret(*this);
    ret.sub_long_short(1);
    ret.sign_ = !ret.sign_;
    return ret;
}
std::string to_string(big_integer const& a)
{
    if (a.is_zero())
        return "0";
    std::string result = "";
    big_integer tmp(a);
    while (tmp != 0)
    {
        size_t digit = tmp.div_long_short(10);
        result.push_back(static_cast<char>(digit + '0'));
    }
    if (!a.sign_)
        result.push_back('-');
    std::reverse(result.begin(), result.end());
    return result;
}
std::ostream& operator <<(std::ostream &ostream, big_integer const& a)
{
    return ostream << to_string(a);
}

#undef u128
