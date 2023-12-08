#pragma once

#include <memory>
#include <odb/core.hxx>
#include <odb/nullable.hxx>
#include <string>
#include <utility>

#include "Message.h"
#include "User.h"

namespace server::infrastructure
{
#pragma db object table("reactions")
class Reaction
{
public:
    Reaction(std::string idInit, std::string nameInit, std::shared_ptr<User> userInit,
             std::shared_ptr<Message> messageInit)
        : id{std::move(idInit)}, name{std::move(nameInit)}, user{std::move(userInit)}, message{std::move(messageInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::string getName() const
    {
        return name;
    }

    [[nodiscard]] std::shared_ptr<User> getUser() const
    {
        return user;
    }

    [[nodiscard]] std::shared_ptr<Message> getMessage() const
    {
        return message;
    }

private:
    Reaction() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string name;

#pragma db not_null
    std::shared_ptr<User> user;

#pragma db not_null
    std::shared_ptr<Message> message;
};
}
