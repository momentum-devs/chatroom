#include <boost/asio.hpp>
#include <thread>

#include "common/environment/EnvironmentParser.h"
#include "common/filesystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandler.h"
#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandlerImpl.h"
#include "server/infrastructure/database/management/DatabaseManagerFactory.h"
#include "server/infrastructure/database/models/User.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"

int main(int argc, char* argv[])
{
    const auto projectPath = common::filesystem::getProjectPath("chatroom");

    dotenv::init(std::format("{}/apps/server/.env", projectPath).c_str());

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    const auto databaseHost = common::environment::EnvironmentParser::parseString("DATABASE_HOST");
    const auto databaseName = common::environment::EnvironmentParser::parseString("DATABASE_NAME");
    const auto databaseUsername = common::environment::EnvironmentParser::parseString("DATABASE_USERNAME");
    const auto databasePassword = common::environment::EnvironmentParser::parseString("DATABASE_PASSWORD");

    const auto databaseManager = server::infrastructure::DatabaseManagerFactory::create(
        {databaseHost, databaseName, databaseUsername, databasePassword});

    std::unique_ptr<server::infrastructure::UserMapper> userMapper =
        std::make_unique<server::infrastructure::UserMapperImpl>();

    std::shared_ptr<server::domain::UserRepository> userRepository =
        std::make_shared<server::infrastructure::UserRepositoryImpl>(std::move(userMapper));

    std::unique_ptr<server::application::CreateUserCommandHandler> createUserCommandHandler =
        std::make_unique<server::application::CreateUserCommandHandlerImpl>(userRepository);

    createUserCommandHandler->execute({"michal.cieslar@gmail.com", "secret123"});

    //    const auto listenPort = common::environment::EnvironmentParser::parseInt("CHATROOM_PORT");
    //
    //    const auto numberOfSupportedThreads = std::thread::hardware_concurrency();

    //    boost::asio::io_context context;
    //
    //    std::unique_ptr<server::api::SessionManager> sessionManager =
    //        std::make_unique<server::api::SessionManager>(context, listenPort);
    //
    //    sessionManager->startAcceptingConnections();
    //
    //    std::vector<std::thread> threads;
    //
    //    threads.reserve(numberOfSupportedThreads);
    //
    //    for (std::size_t n = 0; n < numberOfSupportedThreads; ++n)
    //    {
    //        threads.emplace_back([&] { context.run(); });
    //    }
    //
    //    for (auto& thread : threads)
    //    {
    //        if (thread.joinable())
    //        {
    //            thread.join();
    //        }
    //    }

    return 0;
}
