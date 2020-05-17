
#ifndef ELECT_PROPOSER_STATE_H
#define ELECT_PROPOSER_STATE_H

#include <cinttypes>
#include <string>

namespace elect {

class ProposerState {
 public:
  ProposerState() noexcept;
  bool IsActive();

  void SetPreparing(bool state);
  void SetProposing(bool state);
  void SetProposalID(uint64_t proposalID);
  void SetHighestReceivedProposalID(uint64_t proposalID);
  void SetLeaseOwner(const std::string &owner);
  void SetDuration(uint64_t duration);
  void SetExpireTime(uint64_t expiretime);
  void SetVersion(uint64_t version);

  bool Preparing() const;
  bool Proposing() const;
  uint64_t ProposalID() const;
  uint64_t HighestReceivedProposalID() const;
  const std::string &LeaseOwner() const;
  uint64_t Duration() const;
  uint64_t ExpireTime() const;
  uint64_t Version() const;

 private:
  bool               preparing_;
  bool               proposing_;
  uint64_t           proposalID_;
  uint64_t           highest_received_proposalID_;
  std::string        lease_owner_;
  uint64_t           duration_;
  uint64_t           expire_time_;
  uint64_t           version_;
};

}


#endif //ELECT_PROPOSER_STATE_H
