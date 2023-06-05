#pragma once

#include <memory>
#include <orm/databasemanager.hpp>

#include "../DatabaseConfig.h"

namespace server::infrastructure
{
class DatabaseManagerFactory
{
public:
    static std::shared_ptr<Orm::DatabaseManager> create(const DatabaseConfig&);

    static void addConnection(const DatabaseConfig&);
};
}
