#pragma once

#include <memory>
#include <string>

#include "config-cxx/Config.h"

namespace client::core
{
class ConfigProvider
{
public:
    ConfigProvider();

    std::string getServerHost() const;
    int getServerPort() const;

private:
    std::unique_ptr<config::Config> config;
};
}
