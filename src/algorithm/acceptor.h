
#ifndef ELECT_ACCEPTOR_H
#define ELECT_ACCEPTOR_H

#include <memory>

#include <asio/steady_timer.hpp>

#include "message/message.h"
#include "state/acceptor_state.h"

namespace elect {

class Network;
class Acceptor {
 public:
  explicit Acceptor(std::shared_ptr<Network> network);
  ~Acceptor();

  void ProcessMsg(const Message &msg);
  void OnLeaseTimeout();

 private:
  void SendResponse(const std::string &node_id, const Message &msg);

  void OnPrepareRequest();
  void OnProposeRequest();

  std::shared_ptr<Network> network_;
  std::shared_ptr<asio::steady_timer> lease_timeout_timer_;
  Message msg_;
  AcceptorState state_;
};

}

#endif //ELECT_ACCEPTOR_H
