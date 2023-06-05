#pragma once

#include "HashService.h"

namespace server::application
{
class HashServiceImpl : public HashService
{
public:
    std::string hash(const std::string& data) const override;
};
}
