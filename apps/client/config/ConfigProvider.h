#pragma once

#include <string>

namespace client::config
{
class ConfigProvider
{
public:
    std::string getServerHost() const;
    int getServerPort() const;
};
}
