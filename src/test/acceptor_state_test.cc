#include "state/acceptor_state.h"

#include <gtest/gtest.h>

namespace elect_test {

TEST(ACCEPTORSTATE_TEST, INIT_TEST) {
  elect::AcceptorState state;

  GTEST_ASSERT_EQ(state.Accepted(), false);
  GTEST_ASSERT_EQ(state.PromisedProposalID(), 0);
  GTEST_ASSERT_EQ(state.AcceptedProposalID(), 0);
  GTEST_ASSERT_EQ(state.AcceptedDuration(), 0);
  GTEST_ASSERT_EQ(state.AcceptedExpireTime(), 0);
  GTEST_ASSERT_EQ(state.AcceptedLeaseOwner(), "");
}

TEST(ACCEPTORSTATE_TEST, SET_TEST) {
  elect::AcceptorState state;

  state.SetAccepted(true);
  state.SetPromisedProposalID(12345);
  state.SetAcceptedProposalID(12344);
  state.SetAcceptedDuration(1000);
  state.SetAcceptedExpireTime(1500);
  state.SetAcceptedLeaseOwner("Peco");

  GTEST_ASSERT_EQ(state.Accepted(), true);
  GTEST_ASSERT_EQ(state.PromisedProposalID(), 12345);
  GTEST_ASSERT_EQ(state.AcceptedProposalID(), 12344);
  GTEST_ASSERT_EQ(state.AcceptedDuration(), 1000);
  GTEST_ASSERT_EQ(state.AcceptedExpireTime(), 1500);
  GTEST_ASSERT_EQ(state.AcceptedLeaseOwner(), "Peco");
}

TEST(ACCEPTORSTATE_TEST, TIMEOUT_TEST) {
  elect::AcceptorState state;

  state.SetAccepted(true);
  state.SetPromisedProposalID(12345);
  state.SetAcceptedProposalID(12344);
  state.SetAcceptedDuration(1000);
  state.SetAcceptedExpireTime(1500);
  state.SetAcceptedLeaseOwner("Peco");

  state.OnLeaseTimeout();
  GTEST_ASSERT_EQ(state.Accepted(), false);
  GTEST_ASSERT_EQ(state.PromisedProposalID(), 12345);
  GTEST_ASSERT_EQ(state.AcceptedProposalID(), 0);
  GTEST_ASSERT_EQ(state.AcceptedDuration(), 0);
  GTEST_ASSERT_EQ(state.AcceptedExpireTime(), 0);
  GTEST_ASSERT_EQ(state.AcceptedLeaseOwner(), "");
}

}