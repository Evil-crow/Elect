#include "state/learner_state.h"

#include <gtest/gtest.h>

namespace elect_test {

TEST(LEARNERSTATE_TEST, INIT_TEST) {
  elect::LearnerState state;

  GTEST_ASSERT_EQ(state.Learned(), false);
  GTEST_ASSERT_EQ(state.LeaseOwner(), "");
  GTEST_ASSERT_EQ(state.ExpireTime(), 0);
  GTEST_ASSERT_EQ(state.LeaseEpoch(), 0);
}

TEST(LEARNERSTATE_TEST, SET_TEST) {
  elect::LearnerState state;

  state.SetLearned(true);
  state.SetLeaseOwner("Peco");
  state.SetLeaseEpoch(100);
  state.SetExpireTime(1400);

  GTEST_ASSERT_EQ(state.Learned(), true);
  GTEST_ASSERT_EQ(state.LeaseOwner(), "Peco");
  GTEST_ASSERT_EQ(state.ExpireTime(), 1400);
  GTEST_ASSERT_EQ(state.LeaseEpoch(), 100);
}

TEST(LEARNERSTATE_TEST, TIMEOUT_TEST) {
  elect::LearnerState state;

  state.SetLearned(true);
  state.SetLeaseOwner("Peco");
  state.SetExpireTime(1400);

  state.OnLeaseTimeout();
  GTEST_ASSERT_EQ(state.Learned(), false);
  GTEST_ASSERT_EQ(state.LeaseOwner(), "");
  GTEST_ASSERT_EQ(state.ExpireTime(), 0);
  GTEST_ASSERT_EQ(state.LeaseEpoch(), 1);

  state.OnLeaseTimeout();
  GTEST_ASSERT_EQ(state.Learned(), false);
  GTEST_ASSERT_EQ(state.LeaseOwner(), "");
  GTEST_ASSERT_EQ(state.ExpireTime(), 0);
  GTEST_ASSERT_EQ(state.LeaseEpoch(), 2);
}

}
