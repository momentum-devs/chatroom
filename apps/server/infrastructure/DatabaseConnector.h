#pragma once

#include <memory>
#include <tao/pq.hpp>

#include "DatabaseConfig.h"

namespace server::infrastructure
{
class DatabaseConnector
{
public:
    explicit DatabaseConnector(const DatabaseConfig&);

    std::shared_ptr<tao::pq::connection> getConnection();

private:
    std::shared_ptr<tao::pq::connection> connection;
    const DatabaseConfig& config;
};

}
