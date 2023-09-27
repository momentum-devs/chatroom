#pragma once

#include <odb/pgsql/database.hxx>

#include "common/messages/MessageSerializer.h"
#include "MessageRouterFactory.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/tokenService/TokenService.h"
#include "server/domain/repositories/userRepository/UserRepository.h"
#include "SessionFactory.h"

namespace server::api
{
class SessionFactoryImpl : public SessionFactory
{
public:
    SessionFactoryImpl(boost::asio::io_context& context, const std::shared_ptr<odb::pgsql::database>& db,
                       const std::string& jwtSecret, int jwtExpireIn, const std::string& sendGridApiKey);

    std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> create() const override;

private:
    boost::asio::io_context& context;
    std::shared_ptr<odb::pgsql::database> db;
    MessageRouterFactory messageRouterFactory;
    std::shared_ptr<server::application::TokenService> tokenService;
};
}
