#include <memory>
#include <thread>
#include <boost/asio.hpp>

#include "api/SessionManager.h"
#include "application/UserRepository.h"
#include "common/environmentParser/EnvironmentParser.h"
#include "infrastructure/DatabaseConfig.h"
#include "loguru.hpp"
#include "api/SessionManager.h"

int main(int argc, char* argv[])
{
    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    common::environmentParser::EnvironmentParser environmentParser;

    auto dbUsername = environmentParser.parseString("DB_USERNAME");
    auto dbPassword = environmentParser.parseString("DB_PASSWORD");
    auto dbHost = environmentParser.parseString("DB_HOST");
    auto dbPort = environmentParser.parseString("DB_PORT");
    auto dbName = environmentParser.parseString("DB_NAME");

    auto listenPort = std::stoi(environmentParser.parseString("CHATROOM_PORT"));

    const auto dbConfig = server::infrastructure::DatabaseConfig{
        dbUsername, dbPassword, dbHost, dbPort, dbName,
    };

    std::unique_ptr<server::infrastructure::DatabaseConnector> databaseConnector =
        std::make_unique<server::infrastructure::DatabaseConnector>(dbConfig);

    std::unique_ptr<server::application::UserRepository> userRepository =
        std::make_unique<server::infrastructure::UserRepositoryImpl>(std::move(databaseConnector));

    auto numberOfSupportedThreads = std::thread::hardware_concurrency();

    boost::asio::io_context context;

    std::unique_ptr<server::api::SessionManager> sessionManager =
        std::make_unique<server::api::SessionManager>(context, listenPort);

    sessionManager->startAcceptingConnections();

    std::vector<std::thread> threads;

    threads.reserve(numberOfSupportedThreads);

    for (std::size_t n = 0; n < numberOfSupportedThreads; ++n)
    {
        threads.emplace_back([&] { context.run(); });
    }

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return 0;
}
