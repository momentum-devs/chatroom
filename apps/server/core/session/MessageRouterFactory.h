#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>

#include "MessageRouter.h"
#include "server/application/services/hashService/HashService.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::core
{
class MessageRouterFactory
{
public:
    MessageRouterFactory(std::shared_ptr<odb::sqlite::database> db,
                         std::shared_ptr<server::application::TokenService> tokenService, std::string sendGridApiKey,
                         std::string sendGridEmail);

    std::unique_ptr<MessageRouter> createMessageRouter() const;

private:
    std::shared_ptr<odb::sqlite::database> db;
    std::shared_ptr<server::application::TokenService> tokenService;
    std::string sendGridApiKey;
    std::string sendGridEmail;
};
}
