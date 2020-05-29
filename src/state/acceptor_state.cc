
#include "acceptor_state.h"

namespace elect {

AcceptorState::AcceptorState() noexcept
  : accepted_(false),
    promised_proposalID_(0),
    accepted_proposalID_(0),
    accepted_duration_(0),
    accepted_expire_time_(0),
    accepted_version_(0){}

void AcceptorState::OnLeaseTimeout() {
  accepted_ = false;
  accepted_proposalID_ = 0;
  accepted_lease_owner_.clear();
  accepted_duration_ = 0;
  accepted_expire_time_ = 0;
}

void AcceptorState::SetAccepted(bool state) {
  accepted_ = state;
}

void AcceptorState::SetAcceptedProposalID(uint64_t proposalID) {
  accepted_proposalID_ = proposalID;
}

void AcceptorState::SetPromisedProposalID(uint64_t proposalID) {
  promised_proposalID_ = proposalID;
}

void AcceptorState::SetAcceptedDuration(uint64_t duration) {
  accepted_duration_ = duration;
}

void AcceptorState::SetAcceptedExpireTime(uint64_t expire_time) {
  accepted_expire_time_ = expire_time;
}

void AcceptorState::SetAcceptedVersion(uint64_t version) {
  accepted_version_ = version;
}

void AcceptorState::SetAcceptedLeaseOwner(const std::string &owner) {
  accepted_lease_owner_.assign(owner);
}

bool AcceptorState::Accepted() const {
  return accepted_;
}

uint64_t AcceptorState::PromisedProposalID() const {
  return promised_proposalID_;
}

uint64_t AcceptorState::AcceptedProposalID() const {
  return accepted_proposalID_;
}

uint64_t AcceptorState::AcceptedDuration() const {
  return accepted_duration_;
}

uint64_t AcceptorState::AcceptedExpireTime() const {
  return accepted_expire_time_;
}

uint64_t AcceptorState::AcceptedVersion() const {
  return accepted_version_;
}

const std::string & AcceptorState::AcceptedLeaseOwner() const {
  return accepted_lease_owner_;
}
}