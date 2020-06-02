
#ifndef ELECT_ACCEPTOR_STATE_H
#define ELECT_ACCEPTOR_STATE_H

#include <cinttypes>
#include <string>

namespace elect {

class AcceptorState {
 public:
  AcceptorState() noexcept;
  void OnLeaseTimeout();

  void SetAccepted(bool state);
  void SetPromisedProposalID(uint64_t proposalID);
  void SetAcceptedProposalID(uint64_t proposalID);
  void SetAcceptedLeaseOwner(const std::string &owner);
  void SetAcceptedDuration(uint64_t duration);
  void SetAcceptedExpireTime(uint64_t expire_time);
  void SetAcceptedVersion(uint64_t version);

  bool Accepted() const;
  uint64_t PromisedProposalID() const;
  uint64_t AcceptedProposalID() const;
  uint64_t AcceptedDuration() const;
  uint64_t AcceptedExpireTime() const;
  uint64_t AcceptedVersion() const;
  const std::string &AcceptedLeaseOwner() const;

 private:
  bool               accepted_;
  uint64_t           promised_proposalID_;
  uint64_t           accepted_proposalID_;
  std::string        accepted_lease_owner_;
  unsigned           accepted_duration_;
  uint64_t           accepted_expire_time_;
  uint64_t           accepted_version_;
};
}

#endif //ELECT_ACCEPTOR_STATE_H
