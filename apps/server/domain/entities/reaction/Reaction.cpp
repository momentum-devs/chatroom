#include "Reaction.h"

namespace server::domain
{
Reaction::Reaction(std::string idInit, std::string nameInit, std::shared_ptr<User> userInit,
                   std::shared_ptr<Message> messageInit)
    : id{std::move(idInit)}, name{std::move(nameInit)}, user{std::move(userInit)}, message{std::move(messageInit)}
{
}

std::string Reaction::getId() const
{
    return id;
}

std::string Reaction::getName() const
{
    return name;
}

std::shared_ptr<User> Reaction::getUser() const
{
    return user;
}

std::shared_ptr<Message> Reaction::getMessage() const
{
    return message;
}
}
