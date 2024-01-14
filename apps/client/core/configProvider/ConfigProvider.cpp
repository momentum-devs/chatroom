#include "ConfigProvider.h"

#include "envParser/EnvironmentParser.h"

namespace client::core
{
ConfigProvider::ConfigProvider() : config{std::make_unique<config::Config>()} {}

std::string ConfigProvider::getServerHost() const
{
    return config->get<std::string>("server.host");
}

int ConfigProvider::getServerPort() const
{
    return config->get<int>("server.port");
}
}
