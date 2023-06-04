#pragma once

#include <memory>

#include "Session.h"

namespace server::api
{
class SessionFactory
{
public:
    virtual ~SessionFactory() = default;

    virtual std::shared_ptr<Session> create() const = 0;
};
}