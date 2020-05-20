
#include "learner.h"

#include "network/network.h"
#include "message/message.h"
#include "util/util.h"
#include "config/config.h"

#include <glog/logging.h>

namespace elect {

Learner::Learner(std::shared_ptr<Network> network) :
  network_(std::move(network)),
  lease_timeout_timer_(std::make_shared<asio::steady_timer>(*network_->GetIOContext())),
  learn_lease_handler_(nullptr),
  lease_timeout_handler_(nullptr){}

Learner::~Learner() {
  lease_timeout_timer_->cancel();
  msg_.Clear();
  learn_lease_handler_ = nullptr;
  lease_timeout_handler_ = nullptr;
}

void Learner::ProcessMsg(const Message &msg) {
  msg_.Assign(msg.Msg());
  if (msg_.MsgType() == PaxosMsg::Type::PaxosMsg_Type_LEAEN_CHOSEN) {
    OnLearnChosen();
  } else {
    std::cout << "Unknown type msg" << std::endl;
  }
}

void Learner::OnLearnChosen() {
  if (state_.Learned() || state_.ExpireTime() < util::GetMilliTimestamp()) {
    lease_timeout_timer_->cancel();
    OnLeaseTimeout();
  }

  const auto &config = Config::GetInstance();
  uint64_t expire_time;
  if (msg_.LeaseOwner() == config.NodeID()) {
    expire_time = msg_.ExpireTime();
  } else {
    expire_time = util::GetMilliTimestamp() + msg_.Duration() - 500;
  }

  if (expire_time < util::GetMilliTimestamp()) {
    return;
  }

  state_.SetChanged(state_.LeaseOwner() != msg_.LeaseOwner());          // flag whether it has changed
  state_.SetLearned(true);
  state_.SetLeaseOwner(msg_.LeaseOwner());
  state_.SetExpireTime(msg_.ExpireTime());
  std::cout << "Node: " << state_.LeaseOwner() << " has lease for " << state_.ExpireTime() - util::GetMilliTimestamp() << std::endl;

  asio::error_code ec;
  LOG(INFO) << "last expire time: " << state_.ExpireTime() - util::GetMilliTimestamp() << std::endl;
  lease_timeout_timer_->expires_from_now(std::chrono::milliseconds(state_.ExpireTime() - util::GetMilliTimestamp()), ec);
  lease_timeout_timer_->async_wait([this](asio::error_code ec) {
    this->OnLeaseTimeout();
  });

  // someone acquire lease complete, call handler
  if (learn_lease_handler_) {
    learn_lease_handler_();
  }
}

void Learner::OnLeaseTimeout() {
  lease_timeout_timer_->cancel();
  state_.OnLeaseTimeout();

  if (lease_timeout_handler_) {
    lease_timeout_handler_();
  }
}

void Learner::CheckLease() {
  auto now_time = util::GetMilliTimestamp();
  if (state_.Learned() && state_.ExpireTime() < now_time) {
    OnLeaseTimeout();
  }
}

bool Learner::IsLeaseOwner()
{
  CheckLease();
  const auto &config = Config::GetInstance();
  return state_.Learned() && state_.LeaseOwner() == config.NodeID();
}

bool Learner::IsLeaseKnown()
{
  CheckLease();
  return state_.Learned();
}

bool Learner::IsLeaseChanged() {
  return state_.Changed();
}

std::string Learner::GetLeaseOwner()
{
  CheckLease();
  if (state_.Learned())
    return state_.LeaseOwner();
  else
    return std::string{};
}

uint64_t Learner::GetLeaseEpoch()
{
  CheckLease();
  return state_.LeaseEpoch();
}

void Learner::SetOnLearnLease(Handler handler) {
  learn_lease_handler_ = std::move(handler);
}

void Learner::SetOnLeaseTimout(Handler handler) {
  lease_timeout_handler_ = std::move(handler);
}

}