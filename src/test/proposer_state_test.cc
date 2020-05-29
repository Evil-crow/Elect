#include "state/proposer_state.h"

#include <gtest/gtest.h>

namespace elect_test {

TEST(PROPOSERSTATE_TEST, INIT_TEST) {
  elect::ProposerState state;

  GTEST_ASSERT_EQ(state.Preparing(), false);
  GTEST_ASSERT_EQ(state.Proposing(), false);
  GTEST_ASSERT_EQ(state.ProposalID(), 0);
  GTEST_ASSERT_EQ(state.Duration(), 0);
  GTEST_ASSERT_EQ(state.ExpireTime(), 0);
  GTEST_ASSERT_EQ(state.HighestReceivedProposalID(), 0);
  GTEST_ASSERT_EQ(state.LeaseOwner(), "");
  GTEST_ASSERT_EQ(state.IsActive(), false);
}

TEST(PROPOSERSTATE_TEST, SET_TEST) {
  elect::ProposerState state;

  state.SetPreparing(true);
  state.SetProposing(true);
  state.SetDuration(1000);
  state.SetExpireTime(1500);
  state.SetHighestReceivedProposalID(12345);
  state.SetLeaseOwner("Peco");
  state.SetProposalID(12344);

  GTEST_ASSERT_EQ(state.Preparing(), true);
  GTEST_ASSERT_EQ(state.Proposing(), true);
  GTEST_ASSERT_EQ(state.ProposalID(), 12344);
  GTEST_ASSERT_EQ(state.Duration(), 1000);
  GTEST_ASSERT_EQ(state.ExpireTime(), 1500);
  GTEST_ASSERT_EQ(state.HighestReceivedProposalID(), 12345);
  GTEST_ASSERT_EQ(state.LeaseOwner(), "Peco");
  GTEST_ASSERT_EQ(state.IsActive(), true);
}

}