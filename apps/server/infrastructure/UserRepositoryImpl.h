#pragma once

#include <memory>
#include <vector>

#include "../application/UserRepository.h"
#include "../domain/User.h"
#include "DatabaseConnector.h"

namespace server::infrastructure
{
class UserRepositoryImpl : public application::UserRepository
{
public:
    UserRepositoryImpl(std::unique_ptr<DatabaseConnector> databaseConnectorInit);

    std::vector<domain::User> findUsers() override;
    void deleteUser(const std::string& id) override;

private:
    std::unique_ptr<DatabaseConnector> databaseConnector;
};
}
