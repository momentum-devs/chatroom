#include <boost/asio.hpp>
#include <memory>
#include <thread>

#include "api/SessionManager.h"
#include "common/utils/environmentParser/EnvironmentParser.h"
#include "common/utils/fileSystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"

int main(int argc, char* argv[])
{
    auto dotEnvPath = common::utils::getProjectPath("chatroom") + "apps/server/.env";

    dotenv::init(dotEnvPath.c_str());

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    auto databaseHost = common::utils::EnvironmentParser::parseString("DATABASE_HOST");
    auto databaseName = common::utils::EnvironmentParser::parseString("DATABASE_NAME");
    auto databaseUsername = common::utils::EnvironmentParser::parseString("DATABASE_USERNAME");
    auto databasePassword = common::utils::EnvironmentParser::parseString("DATABASE_PASSWORD");

    auto listenPort = common::utils::EnvironmentParser::parseInt("CHATROOM_PORT");

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
