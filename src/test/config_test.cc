#include <gtest/gtest.h>
#include "../config/config.h"

//namespace Test {
//
//  TEST(CONFIG_TEST, MAX_TIMEOUT_TEST) {
//    const auto &config = elect::Config::GetInstance();
//    GTEST_ASSERT_EQ(config.MaxLeaseTime(), 4000);
//  }
//
//  TEST(CONFIG_TEST, ACQUIRE_TIMEOUT_TEST) {
//    const auto &config = elect::Config::GetInstance();
//    GTEST_ASSERT_EQ(config.AcquireLeaseTimeout(), 2000);
//  }
//
//  TEST(CONFIG_TEST, ADDRESS_TEST) {
//    const auto &config = elect::Config::GetInstance();
//    auto address = config.NodeAddress();
//    GTEST_ASSERT_EQ(address[0].second, 10000);
//    GTEST_ASSERT_EQ(address[1].second, 10001);
//    GTEST_ASSERT_EQ(address[2].second, 10002);
//  }
//}


int main() {
  const auto &config = elect::Config::GetInstance();
  auto address = config.NodeAddress();

  assert(config.MaxLeaseTime() == 4000);
  assert(config.AcquireLeaseTimeout() == 2000);
  assert(address[0].second == 10000);
  assert(address[1].second == 10001);
  assert(address[2].second == 10002);

  std::cout << "Test Complete, OK!" << std::endl;
}