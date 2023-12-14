#include "SessionFactoryImpl.h"

#include <boost/asio.hpp>
#include <utility>

#include "common/messages/MessageReaderImpl.h"
#include "common/messages/MessageSenderImpl.h"
#include "common/messages/MessageSerializerImpl.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerImpl.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapperImpl.h"
#include "server/infrastructure/repositories/blacklistTokenRepository/BlacklistTokenRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "SessionImpl.h"

namespace server::core
{
SessionFactoryImpl::SessionFactoryImpl(boost::asio::io_context& context,
                                       const std::shared_ptr<odb::sqlite::database>& dbInit, std::string jwtSecretInit,
                                       int jwtExpireInInit, std::string sendGridApiKeyInit,
                                       std::string sendGridEmailInit)
    : context{context},
      db{dbInit},
      jwtSecret{std::move(jwtSecretInit)},
      jwtExpireIn{jwtExpireInInit},
      sendGridApiKey{std::move(sendGridApiKeyInit)},
      sendGridEmail{std::move(sendGridEmailInit)}
{
}

std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> SessionFactoryImpl::create() const
{
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);

    auto messageSerializer = std::make_shared<common::messages::MessageSerializerImpl>();

    auto messageReader = std::make_unique<common::messages::MessageReaderImpl>(context, socket, messageSerializer);

    auto messageSender = std::make_unique<common::messages::MessageSenderImpl>(socket, messageSerializer);

    std::shared_ptr<infrastructure::BlacklistTokenMapper> blacklistTokenMapper =
        std::make_shared<infrastructure::BlacklistTokenMapperImpl>();

    std::shared_ptr<domain::BlacklistTokenRepository> blacklistTokenRepository =
        std::make_shared<infrastructure::BlacklistTokenRepositoryImpl>(db, blacklistTokenMapper);

    auto tokenService =
        std::make_shared<server::application::TokenServiceImpl>(jwtSecret, jwtExpireIn, blacklistTokenRepository);

    MessageRouterFactory messageRouterFactory{db, tokenService, sendGridApiKey, sendGridEmail};

    auto messageRouter = messageRouterFactory.createMessageRouter();

    auto userMapper = std::make_shared<server::infrastructure::UserMapperImpl>();

    auto userRepository = std::make_shared<server::infrastructure::UserRepositoryImpl>(db, userMapper);

    auto logoutCommandHandler = std::make_unique<server::application::LogoutUserCommandHandlerImpl>(userRepository);

    auto session =
        std::make_shared<SessionImpl>(std::move(messageReader), std::move(messageSender), std::move(messageRouter),
                                      tokenService, std::move(logoutCommandHandler));

    return std::make_pair(std::move(socket), std::move(session));
}
}
