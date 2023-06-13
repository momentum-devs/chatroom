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
#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/database/models/User.h"
#include "src/Person.hpp"

// TODO: add application class
int main(int argc, char* argv[])
{
    try
    {
        auto db = std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

        std::string john_id, jane_id, joe_id;
        {
            Person john("John", Date(1969, 1, 1));
            Person jane("Jane", Date(1969, 2, 4));
            Person joe("Joe", Date(1969, 3, 9));

            odb::transaction t(db->begin());

            john_id = db->persist(john);
            jane_id = db->persist(jane);
            joe_id = db->persist(joe);

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

    auto databaseManager = server::infrastructure::DatabaseManagerFactory::create(
        {databaseHost, databaseName, databaseUsername, databasePassword});

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
        threads.emplace_back(
            [&]
            {
                server::infrastructure::DatabaseManagerFactory::addConnection(
                    {databaseHost, databaseName, databaseUsername, databasePassword});

                context.run();
            });
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
