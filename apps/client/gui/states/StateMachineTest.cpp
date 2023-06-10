#include "StateMachine.h"

#include <gtest/gtest.h>

#include "StateMock.h"

using namespace ::testing;
using namespace client::gui;

namespace
{

}

class StateMachineTest : public Test
{
public:
    std::shared_ptr<StateMock> state1 = std::make_shared<StrictMock<StateMock>>();
    std::shared_ptr<StateMock> state2 = std::make_shared<StrictMock<StateMock>>();

    StateMachine stateMachine;
};

TEST_F(StateMachineTest, addOneState)
{
    EXPECT_CALL(*state1, activate());

    stateMachine.addNextState(state1);
}

TEST_F(StateMachineTest, addTwoStates)
{
    EXPECT_CALL(*state1, activate());
    EXPECT_CALL(*state1, deactivate());
    EXPECT_CALL(*state2, activate());

    stateMachine.addNextState(state1);
    stateMachine.addNextState(state2);
}

TEST_F(StateMachineTest, returnToPreviousStateWithOneState)
{
    EXPECT_CALL(*state1, activate());
    EXPECT_CALL(*state1, deactivate());

    stateMachine.addNextState(state1);
    stateMachine.returnToThePreviousState();
}

TEST_F(StateMachineTest, returnToPreviousStateWithTwoStates)
{
    EXPECT_CALL(*state1, activate()).Times(2);
    EXPECT_CALL(*state1, deactivate());
    EXPECT_CALL(*state2, activate());
    EXPECT_CALL(*state2, deactivate());

    stateMachine.addNextState(state1);
    stateMachine.addNextState(state2);
    stateMachine.returnToThePreviousState();
}
