
#include "learner_state.h"

namespace elect {

LearnerState::LearnerState() noexcept
  : learned_(false),
    expire_time_(0),
    lease_epoch_(0){}

void LearnerState::OnLeaseTimeout() {
  learned_ = false;
  lease_owner_.clear();
  expire_time_ = 0;
  lease_epoch_++;
}

void LearnerState::SetLearned(bool state) {
  learned_ = state;
}

void LearnerState::SetLeaseOwner(const std::string &owner) {
  lease_owner_.assign(owner);
}

void LearnerState::SetExpireTime(uint64_t expire_time) {
  expire_time_ = expire_time;
}

void LearnerState::SetLeaseEpoch(uint64_t epoch) {
  lease_epoch_ = epoch;
}

bool LearnerState::Learned() const {
  return learned_;
}

const std::string & LearnerState::LeaseOwner() const {
  return lease_owner_;
}

uint64_t LearnerState::LeaseEpoch() const {
  return lease_epoch_;
}

uint64_t LearnerState::ExpireTime() const {
  return expire_time_;
}
}