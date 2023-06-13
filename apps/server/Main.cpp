#include <boost/asio.hpp>
#include <odb/exception.hxx>
#include <odb/pgsql/database.hxx>
#include <thread>

#include "api/SessionManager.h"
#include "common/filesystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "server/api/ConnectionAcceptorImpl.h"
#include "server/api/SessionFactoryImpl.h"
#include "server/config/ConfigProvider.h"
#include "server/infrastructure/database/tables/userTable/User.h"
#include "User.odb.h"

// TODO: add application class
int main(int argc, char* argv[])
{
    try
    {
        auto db = std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

        {
            server::infrastructure::User john("John", "", "");
            server::infrastructure::User jane("Jane", "", "");
            server::infrastructure::User joe("Joe", "", "");

            odb::transaction t(db->begin());

            db->persist(john);
            db->persist(jane);
            db->persist(joe);

            t.commit();
        }
    }
    catch (const odb::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

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

    const auto numberOfSupportedThreads = std::thread::hardware_concurrency();

    boost::asio::io_context context;

    auto sessionFactory = std::make_unique<server::api::SessionFactoryImpl>(context);

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
