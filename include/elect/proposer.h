
#ifndef ELECT_PROPOSER_H
#define ELECT_PROPOSER_H

#include <memory>
#include <functional>

#include <asio.hpp>
#include "state/proposer_state.h"
#include "message/message.h"

namespace elect {

class Network;
class Proposer {
 public:
  explicit Proposer(std::shared_ptr<Network> network);
  ~Proposer();

  void ProcessMsg(const Message &msg);
  void OnAcquireLeaseTimeout();
  void OnExtendLeaseTimeout();
  void StartAcquireLease();
  void StopAcquireLease();
  void SetNewLeaseVersion(uint64_t version);

  uint64_t HightProposalID() const;
  void SetHighestProposalID(uint64_t proposalID);

 private:
  void BroadcastMessage(const Message &msg);
  void OnPrepareResponse();
  void OnProposeResponse();
  void StartPreparing();
  void StartProposing();

  std::shared_ptr<Network> network_;
  ProposerState state_;
  Message msg_;
  std::shared_ptr<asio::steady_timer> acquire_lease_timer_;
  std::shared_ptr<asio::steady_timer> extend_lease_timer_;

  uint64_t highest_proposal_id_;
  uint32_t count_received_;
  uint32_t count_accepted_;
  uint32_t count_rejected_;
};

}

#endif //ELECT_PROPOSER_H
