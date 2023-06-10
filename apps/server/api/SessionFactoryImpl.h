#pragma once

#include "messages/MessageSerializer.h"
#include "server/domain/repositories/UserRepository.h"
#include "SessionFactory.h"

namespace server::api
{
class SessionFactoryImpl : public SessionFactory
{
public:
    explicit SessionFactoryImpl(boost::asio::io_context& context);

    std::pair<std::shared_ptr<boost::asio::ip::tcp::socket>, std::shared_ptr<Session>> create() const override;

private:
    boost::asio::io_context& context;
};
}
