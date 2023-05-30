#include <boost/asio.hpp>

#include "api/SessionImpl.h"
#include "common/environment/EnvironmentParser.h"
#include "common/filesystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"
#include "messages/MessageSerializerImpl.h"

int main(int argc, char* argv[])
{
    auto dotEnvPath = common::filesystem::getProjectPath("chatroom") + "apps/client/.env";

    dotenv::init(dotEnvPath.c_str());

    common::environment::EnvironmentParser environmentParser;

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    auto serverHostname = environmentParser.parseString("SERVER_HOSTNAME");

    auto serverPort = static_cast<unsigned short>(std::stoi(environmentParser.parseString("SERVER_PORT")));

    auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    auto session = std::make_unique<client::api::SessionImpl>(messageSerializer);

    session->connect(serverHostname, serverPort);

    return 0;
}
