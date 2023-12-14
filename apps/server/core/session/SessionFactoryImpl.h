#pragma once

#include <odb/sqlite/database.hxx>

#include "common/messages/MessageSerializer.h"
#include "MessageRouterFactory.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/tokenService/TokenService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"
#include "SessionFactory.h"

namespace server::core
{
class SessionFactoryImpl : public SessionFactory
{
public:
    SessionFactoryImpl(boost::asio::io_context& context, const std::shared_ptr<odb::sqlite::database>& db,
                       std::string jwtSecret, int jwtExpireIn, std::string sendGridApiKey, std::string sendGridEmail);

    std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> create() const override;

private:
    boost::asio::io_context& context;
    std::shared_ptr<odb::sqlite::database> db;
    std::string jwtSecret;
    int jwtExpireIn;
    std::string sendGridApiKey;
    std::string sendGridEmail;
};
}
