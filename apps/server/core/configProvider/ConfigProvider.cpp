#include "ConfigProvider.h"

#include "envParser/EnvironmentParser.h"

namespace server::core
{
ConfigProvider::ConfigProvider() : config{std::make_unique<config::Config>()} {}

std::string ConfigProvider::getDatabasePath() const
{
    return config->get<std::string>("database.path");
}

int ConfigProvider::getServerPort() const
{
    return config->get<int>("server.port");
}

std::string ConfigProvider::getJwtSecret() const
{
    return config->get<std::string>("jwt.secret");
}

int ConfigProvider::getJwtExpiresIn() const
{
    return config->get<int>("jwt.expiresIn");
}

std::string ConfigProvider::getSendGridApiKey() const
{
    return config->get<std::string>("sendgrid.apiKey");
}

std::string ConfigProvider::getSendGridFromEmail() const
{
    return config->get<std::string>("sendgrid.fromEmail");
}

int ConfigProvider::getSessionRefreshInterval() const
{
    return config->get<int>("session.refreshInterval");
}

std::string ConfigProvider::getCxxEnv() const
{
    const auto cxxEnv = common::envParser::EnvironmentParser::parseString("CXX_ENV");

    if (!cxxEnv)
    {
        return "development";
    }

    return *cxxEnv;
}

std::string ConfigProvider::awsAccessKeyId() const
{
    return config->get<std::string>("aws.accessKeyId");
}

std::string ConfigProvider::awsSecretAccessKey() const
{
    return config->get<std::string>("aws.secretAccessKey");
}

std::string ConfigProvider::awsBucket() const
{
    return config->get<std::string>("aws.bucket");
}

}
