#include <iostream>

#include <fmt/format.h>
#include <boost/asio.hpp>

#include "common/messages/Message.h"
#include "loguru.hpp"

int main(int argc, char* argv[])
{
    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    common::messages::Message message{common::messages::MessageId::Register,
                                      common::bytes::Bytes{"abc"},
                                      common::bytes::Bytes{"xyz"}};

    boost::asio::ip::tcp::socket socket;

    boost::asio::ip::tcp::endpoint endpoint{};

    std::cout << message <<std::endl;

    return 0;
}