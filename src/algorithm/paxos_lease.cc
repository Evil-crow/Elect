
#include "paxos_lease.h"

#include "util/util.h"
#include "message/message.h"
#include "network/network.h"

#include <utility>

namespace elect {

PaxosLease::PaxosLease() :
  acquire_lease_(false),
  network_(std::make_shared<Network>()),
  startup_timeout_timer_(std::make_shared<asio::steady_timer>(*network_->GetIOContext())),
  on_learn_lease_handler_(nullptr),
  on_lease_change_handler_(nullptr),
  on_lease_timeout_handler_(nullptr),
  on_learn_lease_handler_ptr_(nullptr),
  on_lease_change_handler_ptr_(nullptr),
  on_lease_timeout_handler_ptr_(nullptr),
  proposer_(network_),
  acceptor_(network_),
  learner_(network_){
  const auto &config = Config::GetInstance();
  node_id_ = config.NodeID();
  learner_.SetOnLearnLease([this](){ this->OnLearnLease(); });
  learner_.SetOnLeaseTimout([this](){ this->OnLeaseTimeout(); });
  network_->SetReadHandler([this](const Message &msg){ this->OnRead(msg); });
}

PaxosLease::~PaxosLease() {
  acquire_lease_ = false;
  network_->Stop();
  asio::error_code ec;
  startup_timeout_timer_->cancel(ec);
  on_learn_lease_handler_ = nullptr;
  on_lease_timeout_handler_ = nullptr;
  on_lease_change_handler_ = nullptr;
  on_learn_lease_handler_ptr_ = nullptr;
  on_lease_timeout_handler_ptr_ = nullptr;
  on_lease_change_handler_ptr_ = nullptr;
}

void PaxosLease::OnRead(const Message &msg) {
  if (msg.IsRequest() && (msg.ProposalID() > proposer_.HightProposalID())) {
    proposer_.SetHighestProposalID(msg.ProposalID());
  }

  if (msg.IsResponse()) {
    proposer_.ProcessMsg(msg);
  } else if (msg.IsRequest()) {
    acceptor_.ProcessMsg(msg);
  } else if (msg.IsLearnChosen()) {
    learner_.ProcessMsg(msg);
    proposer_.SetNewLeaseVersion(msg.Version());           // increase lease version
  } else if (msg.IsConnectOnline()) {
    StartNewConnection(msg.NodeID());
  } else {
    std::cout << "Unknown type message" << std::endl;
  }
}

void PaxosLease::AcquireLease() {
  acquire_lease_ = true;
  proposer_.StartAcquireLease();
  network_->Start();
}

bool PaxosLease::IsLeaseOwner() {
  return learner_.IsLeaseOwner();
}

bool PaxosLease::IsLeaseKnown() {
  return learner_.IsLeaseKnown();
}

std::string PaxosLease::GetLeaseOwner() {
  return learner_.GetLeaseOwner();
}

uint64_t PaxosLease::GetLeaseEpoch() {
  return learner_.GetLeaseEpoch();
}

void PaxosLease::SetOnLearnLease(LeaseHandler handler, void *ptr) {
  on_learn_lease_handler_ = std::move(handler);
  on_learn_lease_handler_ptr_ = ptr;
}

void PaxosLease::SetOnLeaseChange(LeaseHandler handler, void *ptr) {
  on_lease_change_handler_ = std::move(handler);
  on_lease_change_handler_ptr_ = ptr;
}

void PaxosLease::SetOnLeaseTimeout(TimeoutHandler handler, void *ptr) {
  on_lease_timeout_handler_ = std::move(handler);
  on_lease_timeout_handler_ptr_ = ptr;
}

void PaxosLease::Stop() {
  network_->Stop();
}

void PaxosLease::Continue() {
  network_->Start();
}

void PaxosLease::StartNewConnection(const std::string &nodeID) {
  network_->NewConnetion(nodeID);
}

void PaxosLease::OnLearnLease() {
  if (on_learn_lease_handler_) {
    on_learn_lease_handler_(on_learn_lease_handler_ptr_,
      GetLeaseOwner().c_str(),
      node_id_.c_str());
  }

  if (learner_.IsLeaseChanged()) {
    if (on_lease_change_handler_) {
      on_lease_change_handler_(on_lease_change_handler_ptr_,
        GetLeaseOwner().c_str(),
        node_id_.c_str());
    }
    learner_.SetChangedFlag(false);
  }

  if (!IsLeaseOwner()) {
    proposer_.StopAcquireLease();
  }
}

void PaxosLease::OnLeaseTimeout() {
  if (on_lease_timeout_handler_) {
    on_lease_timeout_handler_(on_lease_timeout_handler_ptr_);
  }

  if (acquire_lease_) {
    proposer_.StartAcquireLease();
  }
}
}