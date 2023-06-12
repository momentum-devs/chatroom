#pragma once

#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>

#include <odb/database.hxx>

#include <odb/pgsql/database.hxx>

inline std::shared_ptr<odb::database> create_database() {
  using namespace odb::core;

  auto db = std::make_shared<odb::pgsql::database>("local", "local", "chatroom", "localhost", 5432);

  return db;
}
