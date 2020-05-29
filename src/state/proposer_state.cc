
#include "proposer_state.h"

namespace elect {

ProposerState::ProposerState() noexcept
  : preparing_(false),
    proposing_(false),
    proposalID_(0),
    highest_received_proposalID_(0),
    duration_(0),
    expire_time_(0),
    version_(0) {}

bool ProposerState::IsActive() {
  return (Preparing() || Proposing());
}

void ProposerState::SetPreparing(bool state) {
  preparing_ = state;
}

void ProposerState::SetProposing(bool state) {
  proposing_ = state;
}

void ProposerState::SetProposalID(uint64_t proposalID) {
  proposalID_ = proposalID;
}

void ProposerState::SetHighestReceivedProposalID(uint64_t proposalID) {
  highest_received_proposalID_ = proposalID;
}

void ProposerState::SetLeaseOwner(const std::string &owner) {
  lease_owner_.assign(owner);
}

void ProposerState::SetDuration(uint64_t duration) {
  duration_ = duration;
}

void ProposerState::SetExpireTime(uint64_t expire_time) {
  expire_time_ = expire_time;
}

void ProposerState::SetVersion(uint64_t version) {
  version_ = version;
}

bool ProposerState::Preparing() const {
  return preparing_;
}

bool ProposerState::Proposing() const {
  return proposing_;
}

uint64_t ProposerState::ProposalID() const {
  return proposalID_;
}

uint64_t ProposerState::HighestReceivedProposalID() const {
  return highest_received_proposalID_;
}

uint64_t ProposerState::Duration() const {
  return duration_;
}

uint64_t ProposerState::ExpireTime() const {
  return expire_time_;
}

const std::string & ProposerState::LeaseOwner() const {
  return lease_owner_;
}

uint64_t ProposerState::Version() const {
  return version_;
}
}