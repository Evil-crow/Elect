
#include "proposer.h"

#include <utility>

#include "network/network.h"
#include "config/config.h"
#include "util/util.h"

#include <glog/logging.h>

namespace elect {

Proposer::Proposer(std::shared_ptr<Network> network)
  : network_(std::move(network)),
    count_received_(0),
    count_accepted_(0),
    count_rejected_(0),
    highest_proposal_id_(0),
    acquire_lease_timer_(std::make_shared<asio::steady_timer>(*network_->GetIOContext())),
    extend_lease_timer_(std::make_shared<asio::steady_timer>(*network_->GetIOContext())){}


Proposer::~Proposer() {
  network_->Stop();
  msg_.Clear();

  asio::error_code ec;
  acquire_lease_timer_->cancel(ec);
  extend_lease_timer_->cancel(ec);
  highest_proposal_id_ = 0;
  count_accepted_ = 0;
  count_received_ = 0;
  count_rejected_ = 0;
}

void Proposer::StartAcquireLease() {
  acquire_lease_timer_->cancel();

  if (!state_.IsActive()) {
    StartPreparing();
  }
}

void Proposer::StopAcquireLease() {
  asio::error_code ec;
  state_.SetPreparing(false);
  state_.SetProposing(false);
  acquire_lease_timer_->cancel(ec);
  extend_lease_timer_->cancel(ec);
}

void Proposer::SetNewLeaseVersion(uint64_t version) {
  state_.SetVersion(version);
}

void Proposer::BroadcastMessage(const Message &msg) {
  count_received_ = 0;
  count_accepted_ = 0;
  count_rejected_ = 0;

  network_->BroadcastMessage(msg);
}

void Proposer::ProcessMsg(const Message &msg) {
  msg_.Assign(msg.Msg());

  if (msg_.IsPrepareResponse()) {
    OnPrepareResponse();
  } else if (msg_.IsProposeResponse()) {
    OnProposeResponse();
  } else {
    std::cout << "Unknow type message" << std::endl;
  }
}

void Proposer::OnPrepareResponse() {
  const auto &config = Config::GetInstance();

  if (!state_.Preparing() || state_.ProposalID() != msg_.ProposalID()) {
    return;
  }

  count_received_++;

  if (msg_.MsgType() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_REJECT) {
    std::cout << "[INFO]Proposer: prepare request has been rejected" << std::endl;
    count_rejected_++;

    // because of network error, increase the new version
    if (msg_.Version() > state_.Version()) {
      state_.SetVersion(msg_.Version());
    }
  } else if (msg_.MsgType() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_ACCEPTED &&
                          msg_.ProposalID() >= state_.HighestReceivedProposalID()) {
    std::cout << "[INFO]Proposer: prepare request has been accepted by someone" << std::endl;
    state_.SetHighestReceivedProposalID(msg_.ProposalID());
    state_.SetLeaseOwner(msg_.LeaseOwner());
  }

  if (count_rejected_ >= config.MinMajority()) {
    StartPreparing();
    return;
  }

  if ((count_received_ - count_rejected_) >= config.MinMajority()) {
    StartProposing();
  }
}

void Proposer::OnProposeResponse() {
  const auto &config = Config::GetInstance();
  if (state_.ExpireTime() < util::GetMilliTimestamp()) {
    std::cout << "[SYS]Already expired, wait for timer" << std::endl;
    return;
  }

  if (!state_.Proposing() || state_.ProposalID() != msg_.ProposalID()) {
    return;
  }

  count_received_++;

  if (msg_.MsgType() == PaxosMsg::Type::PaxosMsg_Type_PROPOSE_ACCEPTED) {
    count_accepted_++;
  }

  LOG(INFO) << "[INFO]Received: " << count_received_ << std::endl;
  LOG(INFO) << "[INFO]Accepted: " << count_accepted_ << std::endl;
  LOG(INFO) << "[INFO]Rejected: " << count_rejected_ << std::endl;

  if (count_accepted_ >= config.MinMajority() &&
        state_.ExpireTime() - util::GetMilliTimestamp() > 500) {
    state_.SetProposing(false);
    state_.SetVersion(state_.Version() + 1);                // increase version to reject new online node start prepare.
    Message msg;
    msg.LearnChosen(config.NodeID(),
      state_.LeaseOwner(),
      state_.ExpireTime() - util::GetMilliTimestamp(),
      state_.ExpireTime(),
      state_.Version());

    asio::error_code ec;
    acquire_lease_timer_->cancel(ec);
    extend_lease_timer_->expires_from_now(
      std::chrono::milliseconds((state_.ExpireTime() - util::GetMilliTimestamp())), ec);
    extend_lease_timer_->async_wait([this](asio::error_code ec) {
      this->OnExtendLeaseTimeout();
    });

    this->BroadcastMessage(msg);
    return;
  }

  if (count_received_ == config.NodeNum()) {
    StartPreparing();
  }
}

void Proposer::StartPreparing() {
  const auto &config = Config::GetInstance();
  asio::error_code ec;
  acquire_lease_timer_->expires_from_now(std::chrono::milliseconds(config.AcquireLeaseTimeout()), ec);
  acquire_lease_timer_->async_wait([this](asio::error_code ec) {
    this->OnAcquireLeaseTimeout();
  });

  state_.SetPreparing(true);
  state_.SetProposing(false);
  state_.SetLeaseOwner(config.NodeID());
  state_.SetHighestReceivedProposalID(0);
  state_.SetProposalID(util::NewProposalID(state_.ProposalID(), config.NodeNo()));

  if (state_.ProposalID() > HightProposalID()) {
    SetHighestProposalID(state_.ProposalID());
  }

  Message msg;
  msg.PrepareRequest(config.NodeID(), state_.ProposalID(), state_.Version());

  this->BroadcastMessage(msg);
}

void Proposer::StartProposing() {
  const auto &config = Config::GetInstance();
  state_.SetPreparing(false);
  if (state_.LeaseOwner() != config.NodeID()) {
    return;
  }

  state_.SetProposing(true);
  state_.SetDuration(config.MaxLeaseTime());
  state_.SetExpireTime(util::GetMilliTimestamp() + state_.Duration());

  Message msg;
  msg.ProposeRequest(config.NodeID(), state_.ProposalID(), state_.LeaseOwner(), state_.Duration());
  this->BroadcastMessage(msg);
}

void Proposer::OnAcquireLeaseTimeout() {
  StartPreparing();
}

void Proposer::OnExtendLeaseTimeout() {
  if (!state_.IsActive()) {
    StartPreparing();
  }
}

uint64_t Proposer::HightProposalID() const {
  return highest_proposal_id_;
}

void Proposer::SetHighestProposalID(uint64_t proposalID) {
  highest_proposal_id_ = proposalID;
}
}