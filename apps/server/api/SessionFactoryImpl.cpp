#include "SessionFactoryImpl.h"

#include <boost/asio.hpp>
#include <utility>

#include "common/messages/MessageReaderImpl.h"
#include "common/messages/MessageSenderImpl.h"
#include "common/messages/MessageSerializerImpl.h"
#include "MessageHandlerImpl.h"
#include "server/application/commandHandlers/createChannelCommandHandler/CreateChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImpl.h"
#include "server/application/queryHandlers/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "SessionImpl.h"

namespace server::api
{
SessionFactoryImpl::SessionFactoryImpl(boost::asio::io_context& contextInit,
                                       std::shared_ptr<odb::pgsql::database> dbInit, const std::string& jwtSecret,
                                       const int jwtExpireIn)
    : context{contextInit},
      db{std::move(dbInit)},
      hashService{std::make_shared<server::application::HashServiceImpl>()},
      tokenService{std::make_shared<server::application::TokenServiceImpl>(jwtSecret, jwtExpireIn)}
{
}

std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> SessionFactoryImpl::create() const
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    auto userMapper = std::make_shared<server::infrastructure::UserMapperImpl>();

    auto userRepository = std::make_shared<server::infrastructure::UserRepositoryImpl>(db, userMapper);

    auto registerUserCommandHandler =
        std::make_unique<server::application::RegisterUserCommandHandlerImpl>(userRepository, hashService);

    auto loginUserCommandHandler =
        std::make_unique<server::application::LoginUserCommandHandlerImpl>(userRepository, hashService, tokenService);

    auto channelMapper = std::make_shared<server::infrastructure::ChannelMapperImpl>();

    auto channelRepository = std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, channelMapper);

    auto createChannelCommandHandler =
        std::make_unique<server::application::CreateChannelCommandHandlerImpl>(channelRepository);

    auto userChannelMapper = std::make_shared<server::infrastructure::UserChannelMapperImpl>(userMapper, channelMapper);

    auto userChannelRepository = std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(
        db, userChannelMapper, userMapper, channelMapper);

    auto findChannelsToWhichUserBelongsQueryHandler =
        std::make_unique<server::application::FindChannelsToWhichUserBelongsQueryHandlerImpl>(userChannelRepository);

    auto messageHandler = std::make_unique<MessageHandlerImpl>(
        tokenService, std::move(registerUserCommandHandler), std::move(loginUserCommandHandler),
        std::move(createChannelCommandHandler), std::move(findChannelsToWhichUserBelongsQueryHandler));

    auto session =
        std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender), std::move(messageHandler));

    return std::make_pair(std::move(socket), std::move(session));
}
}
