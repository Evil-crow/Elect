
#ifndef ELECT_PAXOS_LEASE_H
#define ELECT_PAXOS_LEASE_H

#include <memory>
#include <functional>

#include <asio/steady_timer.hpp>
#include "config/config.h"
#include "message/message.h"
#include "algorithm/proposer.h"
#include "algorithm/acceptor.h"
#include "algorithm/learner.h"

namespace elect {

class Network;
class PaxosLease {
 public:
  using TimeoutHandler = std::function<void(void *)>;
  using LeaseHandler = std::function<void (void *, const char *, const char *)>;
  PaxosLease();
  ~PaxosLease();

  void AcquireLease();
  void Stop();
  void Continue();
  void SetOnLearnLease(LeaseHandler handler, void *ptr);
  void SetOnLeaseTimeout(TimeoutHandler handler, void *ptr);
  void SetOnLeaseChange(LeaseHandler handler, void *ptr);

 private:
  bool IsLeaseOwner();
  bool IsLeaseKnown();
  std::string GetLeaseOwner();
  uint64_t GetLeaseEpoch();

  void OnRead(const Message &msg);
  void OnLearnLease();
  void OnLeaseTimeout();
  void StartNewConnection(const std::string &nodeID);

  bool acquire_lease_;
  std::shared_ptr<Network> network_;
  std::shared_ptr<asio::steady_timer> startup_timeout_timer_;
  LeaseHandler on_learn_lease_handler_;
  TimeoutHandler on_lease_timeout_handler_;
  LeaseHandler on_lease_change_handler_;
  void *on_learn_lease_handler_ptr_;
  void *on_lease_timeout_handler_ptr_;
  void *on_lease_change_handler_ptr_;
  Proposer proposer_;
  Acceptor acceptor_;
  Learner learner_;
  std::string node_id_;
};

}

#endif //ELECT_PAXOS_LEASE_H
