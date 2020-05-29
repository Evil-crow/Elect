
#ifndef ELECT_LEARNER_H
#define ELECT_LEARNER_H

#include <memory>
#include <functional>

#include <asio/steady_timer.hpp>

#include "message/message.h"
#include "state/learner_state.h"

namespace elect {

class Network;
class Learner {
 public:
  using Handler = std::function<void ()>;
  explicit Learner(std::shared_ptr<Network> network);
  ~Learner();

  void ProcessMsg(const Message &msg);
  void OnLeaseTimeout();
  bool IsLeaseOwner();
  bool IsLeaseKnown();
  bool IsLeaseChanged();
  std::string GetLeaseOwner();
  uint64_t GetLeaseEpoch();
  void SetOnLearnLease(Handler handler);
  void SetOnLeaseTimout(Handler handler);
  void SetChangedFlag(bool flag);

 private:
  void OnLearnChosen();
  void CheckLease();

  std::shared_ptr<Network> network_;
  std::shared_ptr<asio::steady_timer> lease_timeout_timer_;
  Message msg_;
  LearnerState state_;
  std::string owner_;
  Handler learn_lease_handler_;
  Handler lease_timeout_handler_;
};

}

#endif //ELECT_LEARNER_H
