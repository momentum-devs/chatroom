#include <boost/asio.hpp>
#include <odb/exception.hxx>
#include <thread>

#include "api/SessionManager.h"
#include "common/filesystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "server/api/ConnectionAcceptorImpl.h"
#include "server/api/SessionFactoryImpl.h"
#include "server/config/ConfigProvider.h"
#include "server/core/database/DatabaseConnectionFactory.h"

// TODO: add application class
int main(int argc, char* argv[])
{
    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    dotenv::init(std::format("{}/apps/server/.env", projectPath).c_str());

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    server::config::ConfigProvider configProvider;

    const auto databaseHost = configProvider.getDatabaseHost();
    const auto databaseName = configProvider.getDatabaseName();
    const auto databaseUsername = configProvider.getDatabaseUsername();
    const auto databasePassword = configProvider.getDatabasePassword();
    const auto serverPort = configProvider.getServerPort();
    const auto jwtSecret = configProvider.getJwtSecret();
    const auto jwtExpireIn = configProvider.getJwtExpireIn();

    const auto numberOfSupportedThreads = std::thread::hardware_concurrency();

    auto db = server::core::DatabaseConnectionFactory::create(
        {databaseHost, databaseName, databaseUsername, databasePassword});

    boost::asio::io_context context;

    auto sessionFactory = std::make_unique<server::api::SessionFactoryImpl>(context, db, jwtSecret, jwtExpireIn);

    auto connectionAcceptor =
        std::make_unique<server::api::ConnectionAcceptorImpl>(context, serverPort, std::move(sessionFactory));

    std::unique_ptr<server::api::SessionManager> sessionManager =
        std::make_unique<server::api::SessionManager>(std::move(connectionAcceptor));

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
