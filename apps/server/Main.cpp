#include <memory>
#include <thread>
#include <boost/asio.hpp>

#include "api/SessionManager.h"
#include "common/environmentParser/EnvironmentParser.h"
#include "loguru.hpp"
#include "laserpants/dotenv/dotenv.h"

int main(int argc, char* argv[])
{
    dotenv::init("/home/michal/repos/chatroom/apps/server/.env");

    common::environmentParser::EnvironmentParser environmentParser;

    std::cerr << environmentParser.parseString("DB_HOST") << std::endl;

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);


    auto dbUsername = environmentParser.parseString("DB_USERNAME");
    auto dbPassword = environmentParser.parseString("DB_PASSWORD");
    auto dbHost = environmentParser.parseString("DB_HOST");
    auto dbPort = environmentParser.parseString("DB_PORT");
    auto dbName = environmentParser.parseString("DB_NAME");

    auto listenPort = std::stoi(environmentParser.parseString("CHATROOM_PORT"));

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
