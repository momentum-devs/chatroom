#include "SessionFactoryImpl.h"

#include <boost/asio.hpp>
#include <utility>

#include "common/messages/MessageReaderImpl.h"
#include "common/messages/MessageSenderImpl.h"
#include "common/messages/MessageSerializerImpl.h"
#include "server/application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionFactoryImpl::SessionFactoryImpl(boost::asio::io_context& contextInit,
                                       std::shared_ptr<odb::pgsql::database> dbInit, const std::string& jwtSecret,
                                       const int jwtExpireIn, std::string sendGridApiKey)
    : context{contextInit}, messageRouterFactory{std::move(dbInit), jwtSecret, jwtExpireIn, std::move(sendGridApiKey)}
{
}

std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> SessionFactoryImpl::create() const
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    auto messageRouter = messageRouterFactory.createMessageRouter();

    auto session =
        std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender), std::move(messageRouter));

    return std::make_pair(std::move(socket), std::move(session));
}
}
