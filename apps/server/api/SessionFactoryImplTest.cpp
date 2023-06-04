#include "SessionFactoryImpl.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace server::api;

namespace
{

}

class SessionFactoryImplTest : public Test
{
public:
    SessionFactoryImpl sessionFactory{};
};

TEST_F(SessionFactoryImplTest, sessionFactoryTest) {}