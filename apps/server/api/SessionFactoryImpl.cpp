#include "SessionFactoryImpl.h"

#include <boost/asio.hpp>
#include <utility>

#include "common/messages/MessageReaderImpl.h"
#include "common/messages/MessageSenderImpl.h"
#include "server/application/commandHandlers/createUserCommandHandler/CreateUserCommandHandlerImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionFactoryImpl::SessionFactoryImpl(boost::asio::io_context& contextInit,
                                       std::shared_ptr<common::messages::MessageSerializer> messageSerializerInit,
                                       std::shared_ptr<server::domain::UserRepository> userRepositoryInit)
    : context{contextInit}, messageSerializer{std::move(messageSerializerInit)}, userRepository{userRepositoryInit}
{
}

std::shared_ptr<Session> SessionFactoryImpl::create() const
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);
    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);
    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);
    auto createUserCommandHandler = std::make_unique<server::application::CreateUserCommandHandlerImpl>(userRepository);

    return std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender),
                                         std::move(createUserCommandHandler));
}
}