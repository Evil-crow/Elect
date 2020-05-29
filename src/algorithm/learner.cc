
#include "learner.h"

#include "network/network.h"
#include "message/message.h"
#include "util/util.h"
#include "config/config.h"

namespace elect {

Learner::Learner(std::shared_ptr<Network> network) :
  network_(std::move(network)),
  lease_timeout_timer_(std::make_shared<asio::steady_timer>(*network_->GetIOContext())),
  learn_lease_handler_(nullptr),
  lease_timeout_handler_(nullptr){}

Learner::~Learner() {
  asio::error_code ec;
  lease_timeout_timer_->cancel(ec);
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
  if (state_.Learned() && state_.ExpireTime() < util::GetMilliTimestamp()) {
    asio::error_code ec;
    lease_timeout_timer_->cancel(ec);
    OnLeaseTimeout();
  }

  const auto &config = Config::GetInstance();
  uint64_t expire_time;
  if (msg_.LeaseOwner() == config.NodeID()) {
    expire_time = msg_.ExpireTime() - 50;
  } else {
    expire_time = util::GetMilliTimestamp() + msg_.Duration();
  }

  if (expire_time < util::GetMilliTimestamp()) {
    return;
  }

  state_.SetChanged(owner_ != msg_.LeaseOwner());          // flag whether it has changed
  state_.SetLearned(true);
  state_.SetLeaseOwner(msg_.LeaseOwner());
  state_.SetExpireTime(msg_.ExpireTime());
  owner_.assign(msg_.LeaseOwner());                         // update new owner

  asio::error_code ec;
  lease_timeout_timer_->cancel(ec);
  lease_timeout_timer_->expires_from_now(std::chrono::milliseconds(expire_time - util::GetMilliTimestamp()), ec);
  lease_timeout_timer_->async_wait([this](asio::error_code ec) {
    if (!ec) {
      this->OnLeaseTimeout();
    }
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

void Learner::SetChangedFlag(bool flag) {
  state_.SetChanged(flag);
}

}