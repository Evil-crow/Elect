
#ifndef ELECT_CONFIG_H
#define ELECT_CONFIG_H

#include <vector>
#include <string>
#include <cmath>

namespace elect {

using address = std::pair<std::string, long>;

struct Data {
  int max_lease_timeout_;
  int acquire_lease_timeout;
  std::string node_id_;
  std::string ip_;
  long port_;
  std::vector<elect::address> node_addrs_;
};

class Config {
 public:
  static Config &GetInstance();

  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  // get configuration infos
  inline int MaxLeaseTime() const { return data_.max_lease_timeout_; }
  inline int AcquireLeaseTimeout() const { return data_.acquire_lease_timeout; }
  inline auto NodeAddress() const -> std::vector<elect::address> {
    return data_.node_addrs_;
  }
  inline std::string NodeID() const { return data_.node_id_; }
  inline std::string Ip() const { return data_.ip_; }
  inline long Port() const { return data_.port_; }


  // calculate params for system
  inline int NodeNum() const { return data_.node_addrs_.size(); }
  inline int MinMajority() const { return ::ceil(static_cast<double>(NodeNum()) / 2); }

 private:
  explicit Config(Data &&data) noexcept : data_(data) {}
  static Data GetData();

  Data data_;
};

}

#endif //ELECT_CONFIG_H
