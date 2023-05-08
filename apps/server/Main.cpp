#include <memory>

#include "../common/EnvironmentParser.h"
#include "application/UserRepository.h"
#include "infrastructure/DatabaseConfig.h"
#include "infrastructure/DatabaseConnector.h"
#include "infrastructure/UserRepositoryImpl.h"
#include "loguru.hpp"

int main(int argc, char* argv[])
{
    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    common::EnvironmentParser environmentParser;

    auto dbUsername = environmentParser.parseString("DB_USERNAME");
    auto dbPassword = environmentParser.parseString("DB_PASSWORD");
    auto dbHost = environmentParser.parseString("DB_HOST");
    auto dbPort = environmentParser.parseString("DB_PORT");
    auto dbName = environmentParser.parseString("DB_NAME");

    const auto dbConfig = server::infrastructure::DatabaseConfig{
        dbUsername, dbPassword, dbHost, dbPort, dbName,
    };

    std::unique_ptr<server::infrastructure::DatabaseConnector> databaseConnector =
        std::make_unique<server::infrastructure::DatabaseConnector>(dbConfig);

    std::unique_ptr<server::application::UserRepository> userRepository =
        std::make_unique<server::infrastructure::UserRepositoryImpl>(std::move(databaseConnector));

    return 0;
}
