#include <boost/asio.hpp>
#include <odb/exception.hxx>
#include <thread>

#include "common/filesystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "server/config/ConfigProvider.h"
#include "server/core/database/DatabaseConnectionFactory.h"
#include "server/core/session/ConnectionAcceptorImpl.h"
#include "server/core/session/SessionFactoryImpl.h"
#include "server/core/session/SessionManager.h"

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
    const auto sendGridApiKey = configProvider.getSendGridApiKey();
    const auto sendGridEmail = configProvider.getSendGridEmail();
    const auto refreshSessionsInterval = boost::posix_time::seconds{configProvider.getRefreshSessionsInterval()};

    const auto numberOfSupportedThreads = std::thread::hardware_concurrency();

    auto db = server::core::DatabaseConnectionFactory::create(
        {databaseHost, databaseName, databaseUsername, databasePassword});

    boost::asio::io_context context;

    auto sessionFactory = std::make_unique<server::core::SessionFactoryImpl>(context, db, jwtSecret, jwtExpireIn,
                                                                             sendGridApiKey, sendGridEmail);

    auto connectionAcceptor =
        std::make_unique<server::core::ConnectionAcceptorImpl>(context, serverPort, std::move(sessionFactory));

    std::unique_ptr<server::core::SessionManager> sessionManager =
        std::make_unique<server::core::SessionManager>(std::move(connectionAcceptor));

    sessionManager->startAcceptingConnections();

    boost::asio::deadline_timer timer(context, refreshSessionsInterval);

    std::function<void(const boost::system::error_code&)> refreshSessions = [&](const boost::system::error_code& /*e*/)
    {
        sessionManager->removeInactiveSessions();
        timer.expires_at(timer.expires_at() + refreshSessionsInterval);
        timer.async_wait(refreshSessions);
    };

    timer.async_wait(refreshSessions);

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
