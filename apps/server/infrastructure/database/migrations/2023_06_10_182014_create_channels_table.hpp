#pragma once

#include <tom/migration.hpp>

namespace Migrations
{
struct CreateChannelsTable : Migration
{
    T_MIGRATION

    void up() const override
    {
        Schema::create("channels",
                       [](Blueprint& table)
                       {
                           table.id();
                           table.timestamps();
                           table.text("name");
                           table.bigInteger("creator_id");
                           table.foreign("creator_id").references("id").on("users");
                       });
    }

    void down() const override
    {
        Schema::dropIfExists("channels");
    }
};
}
