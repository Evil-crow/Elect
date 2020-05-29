#include <gtest/gtest.h>
#include "../config/config.h"

namespace elect_test {

  TEST(CONFIG_TEST, MAX_TIMEOUT_TEST) {
    const auto &config = elect::Config::GetInstance();
    GTEST_ASSERT_EQ(config.MaxLeaseTime(), 4000);
  }

  TEST(CONFIG_TEST, NODE_NO_TEST) {
    const auto &config = elect::Config::GetInstance();
    GTEST_ASSERT_EQ(config.NodeNo(), 0);
  }

  TEST(CONFIG_TEST, ACQUIRE_TIMEOUT_TEST) {
    const auto &config = elect::Config::GetInstance();
    GTEST_ASSERT_EQ(config.AcquireLeaseTimeout(), 2000);
  }

  TEST(CONFIG_TEST, ADDRESS_TEST) {
    const auto &config = elect::Config::GetInstance();
    auto address = config.NodeAddress();
    GTEST_ASSERT_EQ(address[0].second, 10000);
    GTEST_ASSERT_EQ(address[1].second, 10001);
    GTEST_ASSERT_EQ(address[2].second, 10002);
  }

  TEST(CONFIG_TEST, NODE_TEST) {
    const auto &config = elect::Config::GetInstance();
    GTEST_ASSERT_EQ(config.Ip(), "127.0.0.1");
    GTEST_ASSERT_EQ(config.Port(), 10000);
    GTEST_ASSERT_EQ(config.NodeID(), "127.0.0.1:10000");
  }

  TEST(CONFIG_TEST, PARAM_TEST) {
    const auto &config = elect::Config::GetInstance();
    GTEST_ASSERT_EQ(config.NodeNum(), 3);
    GTEST_ASSERT_EQ(config.MinMajority(), 2);
  }
}
