#pragma once

#include <string>

namespace common::bytes
{
struct Bytes : public std::basic_string<unsigned char>
{
    using std::basic_string<unsigned char>::basic_string;

    template <class T>
    explicit inline Bytes(const std::basic_string<T>& str):
          Bytes{reinterpret_cast<const unsigned char *>(str.begin().base()),
                reinterpret_cast<const unsigned char *>(str.end().base())}
    {}

    explicit inline Bytes(const char* str):
          Bytes{std::string {str}}
    {}

    explicit inline Bytes(const u_int32_t data)
    {
        this->push_back(static_cast<unsigned char>(data >> 24));
        this->push_back(static_cast<unsigned char>(data >> 16));
        this->push_back(static_cast<unsigned char>(data >> 8));
        this->push_back(static_cast<unsigned char>(data));
    }

    explicit inline operator std::string ()
    {
        return {reinterpret_cast<const char *>(begin().base()), reinterpret_cast<const char *>(end().base())};
    }

    inline Bytes operator+(const Bytes& rhs)
    {
        return Bytes{static_cast<std::basic_string<unsigned char>>(*this) +
                     static_cast<std::basic_string<unsigned char>>(rhs)};
    }

    inline Bytes operator+(const unsigned char rhs)
    {
        return Bytes{static_cast<std::basic_string<unsigned char>>(*this) + rhs};
    }

    inline Bytes subBytes(size_type pos = 0, size_type n = npos) const
    {
      return Bytes{substr(pos,n)};
    }
};
}