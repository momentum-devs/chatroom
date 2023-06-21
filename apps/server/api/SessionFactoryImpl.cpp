#include "SessionFactoryImpl.h"

#include <boost/asio.hpp>
#include <utility>

#include "common/messages/MessageReaderImpl.h"
#include "common/messages/MessageSenderImpl.h"
#include "common/messages/MessageSerializerImpl.h"
#include "MessageHandlerImpl.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionFactoryImpl::SessionFactoryImpl(boost::asio::io_context& contextInit,
                                       std::shared_ptr<odb::pgsql::database> dbInit)
    : context{contextInit}, db{std::move(dbInit)}
{
}

std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> SessionFactoryImpl::create() const
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    auto userMapper = std::make_unique<server::infrastructure::UserMapperImpl>();

    auto userRepository = std::make_shared<server::infrastructure::UserRepositoryImpl>(db, std::move(userMapper));

    const auto hashService = std::make_shared<server::application::HashServiceImpl>();

    auto registerUserCommandHandler =
        std::make_unique<server::application::RegisterUserCommandHandlerImpl>(userRepository, hashService);

    auto messageHandler = std::make_unique<MessageHandlerImpl>(std::move(registerUserCommandHandler));

    auto session =
        std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender), std::move(messageHandler));

    return std::make_pair(std::move(socket), std::move(session));
}
}
