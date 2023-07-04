#pragma once

#include "messages/Message.h"
namespace server::api
{
class MessageRouter
{
public:
    virtual ~MessageRouter() = default;

    virtual common::messages::Message route(const common::messages::Message& message) = 0;
};
}