#include <boost/asio.hpp>

#include "common/utils/environmentParser/EnvironmentParser.h"
#include "common/utils/fileSystem/GetProjectPath.h"
#include "laserpants/dotenv/dotenv.h"
#include "loguru.hpp"

int main(int argc, char* argv[])
{
    auto dotEnvPath = common::utils::getProjectPath("chatroom") + "apps/client/.env";

    dotenv::init(dotEnvPath.c_str());

    common::utils::EnvironmentParser environmentParser;

    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    auto serverHostname = environmentParser.parseString("SERVER_HOSTNAME");

    auto serverPort = static_cast<unsigned short>(std::stoi(environmentParser.parseString("SERVER_PORT")));

    boost::asio::io_context context;

    boost::asio::ip::tcp::socket socket{context};

    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(serverHostname), serverPort};

    socket.connect(endpoint);

    LOG_S(INFO) << "Connected to " << socket.remote_endpoint().address();

    return 0;
}
