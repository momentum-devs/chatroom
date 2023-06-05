#include "SessionFactoryImpl.h"

#include <boost/asio.hpp>
#include <utility>

#include "common/messages/MessageReaderImpl.h"
#include "common/messages/MessageSenderImpl.h"
#include "MessageHandlerImpl.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionFactoryImpl::SessionFactoryImpl(boost::asio::io_context& contextInit,
                                       std::shared_ptr<common::messages::MessageSerializer> messageSerializerInit,
                                       std::shared_ptr<server::domain::UserRepository> userRepositoryInit)
    : context{contextInit},
      messageSerializer{std::move(messageSerializerInit)},
      userRepository{std::move(userRepositoryInit)}
{
}

std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> SessionFactoryImpl::create() const
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);
    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);
    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);
    auto registerUserCommandHandler =
        std::make_unique<server::application::RegisterUserCommandHandlerImpl>(userRepository);
    auto messageHandler = std::make_unique<MessageHandlerImpl>(std::move(registerUserCommandHandler));
    auto session =
        std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender), std::move(messageHandler));

    return std::make_pair(std::move(socket), std::move(session));
}
}
