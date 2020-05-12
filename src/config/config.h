
#ifndef ELECT_CONFIG_H
#define ELECT_CONFIG_H

#include <vector>
#include <string>

namespace elect {

using address = std::pair<std::string, int>;

struct Data {
  int max_lease_timeout;
  int acquire_lease_timeout;
  std::vector<elect::address> node_addrs;
};

class Config {
 public:
  static Config &GetInstance();

  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  int MaxLeaseTime() const { return data_.max_lease_timeout; }
  int AcquireLeaseTimeout() const { return data_.acquire_lease_timeout; }
  auto NodeAddress() const -> std::vector<elect::address> {
    return data_.node_addrs;
  }

 private:
  explicit Config(Data &&data) noexcept : data_(data) {}
  static Data GetData();

  Data data_;
};

}

#endif //ELECT_CONFIG_H
