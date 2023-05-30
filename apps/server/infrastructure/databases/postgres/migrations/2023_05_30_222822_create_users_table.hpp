#pragma once

#include <tom/migration.hpp>

namespace Migrations
{
struct CreateUsersTable : Migration
{
    T_MIGRATION

    void up() const override
    {
        Schema::create("users", [](Blueprint &table)
                       {
                           table.id();
                           table.timestamps();
                           table.text("email").unique();
                           table.text("password");
                           table.text("nickname");
                       });
    }

    void down() const override
    {
        Schema::dropIfExists("users");
    }
};
}
