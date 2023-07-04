#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>

#include "MessageRouter.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class MessageRouterFactory
{
public:
    MessageRouterFactory(std::shared_ptr<odb::pgsql::database> db, const std::string& jwtSecret, const int jwtExpireIn);
    std::unique_ptr<MessageRouter> createMessageRouter() const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<server::application::HashService> hashService;
    std::shared_ptr<server::application::TokenService> tokenService;
};
}