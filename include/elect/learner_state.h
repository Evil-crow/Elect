
#ifndef ELECT_LEARNER_STATE_H
#define ELECT_LEARNER_STATE_H

#include <cinttypes>
#include <string>

namespace elect {

class LearnerState {
 public:
  LearnerState() noexcept;

  void OnLeaseTimeout();
  void SetLearned(bool state);
  void SetLeaseOwner(const std::string &owner);
  void SetExpireTime(uint64_t expire_time);
  void SetLeaseEpoch(uint64_t epoch);
  void SetChanged(bool flag);

  bool Learned() const;
  const std::string &LeaseOwner() const;
  uint64_t ExpireTime() const;
  u_int64_t LeaseEpoch() const;
  bool Changed() const;

 private:
  bool learned_;
  bool changed_;
  std::string lease_owner_;
  uint64_t expire_time_;
  uint64_t lease_epoch_;
};

}

#endif //ELECT_LEARNER_STATE_H
