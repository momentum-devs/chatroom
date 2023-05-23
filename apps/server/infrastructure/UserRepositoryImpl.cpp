#include "UserRepositoryImpl.h"

#include <format>

namespace server::infrastructure
{

namespace
{
const auto idColumn = "id";
const auto emailColumn = "email";
const auto passwordColumn = "password";
const auto nicknameColumn = "nickname";
}

UserRepositoryImpl::UserRepositoryImpl(std::unique_ptr<DatabaseConnector> databaseConnectorInit)
    : databaseConnector{std::move(databaseConnectorInit)}
{
}

std::vector<domain::User> UserRepositoryImpl::findUsers()
{
    auto users = std::vector<domain::User>();

    const auto connection = databaseConnector->getConnection();

    const auto findUsersQuery = R"(
                                    SELECT
                                        users."id",
                                        users."login",
                                        users."password",
                                        users."nickname",
                                    FROM users;)";

    const auto usersRows = connection->execute(findUsersQuery);

    for (const auto& userRow : usersRows)
    {
        domain::User user{userRow[idColumn].as<std::string>(), userRow[emailColumn].as<std::string>(),
                          userRow[passwordColumn].as<std::string>(), userRow[nicknameColumn].as<std::string>()};

        users.push_back(user);
    }

    return users;
}

void UserRepositoryImpl::deleteUser(const std::string& id)
{
    const auto connection = databaseConnector->getConnection();

    const auto deleteUserQuery = std::format("DELETE FROM users WHERE id='{}'", id);

    connection->execute(deleteUserQuery);
}
}
