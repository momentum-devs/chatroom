#pragma once

#include <span>
#include <vector>

#include "NumberGenerator.h"

namespace common::random
{
class Helper
{
public:
    template <class T>
    static T arrayElement(std::span<const T> data)
    {
        const auto index = NumberGenerator::integer<size_t>(data.size() - 1);

        return data[index];
    }

    template <class T>
    static T arrayElement(const std::vector<T>& data)
    {
        const auto index = NumberGenerator::integer<size_t>(data.size() - 1);

        return data[index];
    }
};
}
