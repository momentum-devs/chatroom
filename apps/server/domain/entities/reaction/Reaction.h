#pragma once

#include <memory>
#include <optional>
#include <string>

#include "server/domain/entities/message/Message.h"
#include "server/domain/entities/user/User.h"

namespace server::domain
{
class Reaction
{
public:
    Reaction(std::string id, std::string name, std::shared_ptr<User> user, std::shared_ptr<Message> message);

    std::string getId() const;
    std::string getName() const;
    std::shared_ptr<User> getUser() const;
    std::shared_ptr<Message> getMessage() const;
    void setName(const std::string& name);

private:
    std::string id;
    std::string name;
    std::shared_ptr<User> user;
    std::shared_ptr<Message> message;
};
}
