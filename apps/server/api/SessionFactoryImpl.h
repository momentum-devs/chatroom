#pragma once

#include "messages/MessageSerializer.h"
#include "server/domain/repositories/UserRepository.h"
#include "SessionFactory.h"

namespace server::api
{
class SessionFactoryImpl : public SessionFactory
{
public:
    SessionFactoryImpl(boost::asio::io_context& context,
                       std::shared_ptr<common::messages::MessageSerializer> messageSerializer,
                       std::shared_ptr<server::domain::UserRepository> userRepository);

    std::shared_ptr<Session> create() const override;

private:
    boost::asio::io_context& context;
    std::shared_ptr<common::messages::MessageSerializer> messageSerializer;
    std::shared_ptr<server::domain::UserRepository> userRepository;
};
}
