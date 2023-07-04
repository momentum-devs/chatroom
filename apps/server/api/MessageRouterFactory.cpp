#include "MessageRouterFactory.h"

#include "MessageHandlers/CreateChannelMessageHandler.h"
#include "MessageHandlers/GetUserChannelsMessageHandler.h"
#include "MessageHandlers/LoginMessageHandler.h"
#include "MessageHandlers/RegisterMessageHandler.h"
#include "MessageRouterImpl.h"
#include "server/application/commandHandlers/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.h"
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

namespace server::api
{
MessageRouterFactory::MessageRouterFactory(std::shared_ptr<odb::pgsql::database> dbInit, const std::string& jwtSecret,
                                           const int jwtExpireIn)
    : db{std::move(dbInit)},
      hashService{std::make_shared<server::application::HashServiceImpl>()},
      tokenService{std::make_shared<server::application::TokenServiceImpl>(jwtSecret, jwtExpireIn)}

{
}

std::unique_ptr<MessageRouter> MessageRouterFactory::createMessageRouter()
{
    auto userMapper = std::make_shared<server::infrastructure::UserMapperImpl>();

    auto userRepository = std::make_shared<server::infrastructure::UserRepositoryImpl>(db, userMapper);

    auto registerUserCommandHandler =
        std::make_unique<server::application::RegisterUserCommandHandlerImpl>(userRepository, hashService);

    auto loginUserCommandHandler =
        std::make_unique<server::application::LoginUserCommandHandlerImpl>(userRepository, hashService, tokenService);

    auto channelMapper = std::make_shared<server::infrastructure::ChannelMapperImpl>();

    auto channelRepository = std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, channelMapper);

    auto userChannelMapper = std::make_shared<server::infrastructure::UserChannelMapperImpl>(userMapper, channelMapper);

    auto userChannelRepository = std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(
        db, userChannelMapper, userMapper, channelMapper);

    auto findChannelsToWhichUserBelongsQueryHandler =
        std::make_unique<server::application::FindChannelsToWhichUserBelongsQueryHandlerImpl>(userChannelRepository);

    auto addUserToChannelCommandHandler = std::make_shared<server::application::AddUserToChannelCommandHandlerImpl>(
        userChannelRepository, userRepository, channelRepository);

    auto createChannelCommandHandler = std::make_unique<server::application::CreateChannelCommandHandlerImpl>(
        channelRepository, addUserToChannelCommandHandler);

    auto createChannelMessageHandler =
        std::make_shared<CreateChannelMessageHandler>(tokenService, std::move(createChannelCommandHandler));

    auto getUserChannelsMessageHandler = std::make_shared<GetUserChannelsMessageHandler>(
        tokenService, std::move(findChannelsToWhichUserBelongsQueryHandler));

    auto loginMessageHandler = std::make_shared<LoginMessageHandler>(tokenService, std::move(loginUserCommandHandler));

    auto registerMessageHandler = std::make_shared<RegisterMessageHandler>(std::move(registerUserCommandHandler));

    std::unordered_map<common::messages::MessageId, std::shared_ptr<MessageHandler>> messageHandlers{
        {common::messages::MessageId::CreateChannel, createChannelMessageHandler},
        {common::messages::MessageId::GetUserChannels, getUserChannelsMessageHandler},
        {common::messages::MessageId::Login, loginMessageHandler},
        {common::messages::MessageId::Register, registerMessageHandler},
    };

    return std::make_unique<MessageRouterImpl>(std::move(messageHandlers));
}
}