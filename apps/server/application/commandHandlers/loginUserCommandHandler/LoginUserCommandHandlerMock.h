#pragma once

#include "LoginUserCommandHandler.h"

#include <gmock/gmock.h>

namespace server::application
{
class LoginUserCommandHandlerMock: public LoginUserCommandHandler
{
};
}
