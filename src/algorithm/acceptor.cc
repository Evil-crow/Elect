
#include "acceptor.h"

#include <utility>

#include <glog/logging.h>
#include "network/network.h"
#include "config/config.h"
#include "util/util.h"

namespace elect {

Acceptor::Acceptor(std::shared_ptr<Network> network) :
  network_(std::move(network)),
  lease_timeout_timer_(std::make_shared<asio::steady_timer>(*network_->GetIOContext())) {}

Acceptor::~Acceptor() {
  lease_timeout_timer_->cancel();
  msg_.Clear();
};

void Acceptor::ProcessMsg(const Message &msg) {
  msg_.Assign(msg.Msg());
  if (msg_.MsgType() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_REQUEST) {
    OnPrepareRequest();
  } else if (msg_.MsgType() == PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REQUEST) {
    OnProposeRequest();
  } else {
    std::cout << "Unknown type message" << std::endl;
  }
}

void Acceptor::OnLeaseTimeout() {
  state_.OnLeaseTimeout();
}

void Acceptor::OnPrepareRequest() {
  if (state_.Accepted() && state_.AcceptedExpireTime() < util::GetMilliTimestamp()) {
    lease_timeout_timer_->cancel();
    OnLeaseTimeout();
  }

  Message msg_to_send;
  const auto &config = Config::GetInstance();
  auto nodeID = config.NodeID();
  auto senderID = msg_.NodeID();
  if (/*(msg_.Version() < state_.Accepted_Version()) ||*/ (msg_.ProposalID() < state_.PromisedProposalID())) {
    LOG(INFO) << "Acceptor:  msg proposalID => " << msg_.ProposalID() << " Promised proposalID => " << state_.PromisedProposalID() << std::endl;
    msg_to_send.PrepareRejected(nodeID, msg_.ProposalID(), state_.Accepted_Version());
  } else {
    state_.SetPromisedProposalID(msg_.ProposalID());

    if (!state_.Accepted()) {
      msg_to_send.PrepareOpening(nodeID, msg_.ProposalID());
    } else {
      msg_to_send.PrepareAccepted(nodeID, msg_.ProposalID(), state_.AcceptedProposalID(), state_.AcceptedLeaseOwner(), state_.AcceptedDuration());
    }
  }

  SendResponse(senderID, msg_to_send);
}

void Acceptor::OnProposeRequest() {
  Message msg_to_send;
  const auto &config = Config::GetInstance();
  auto nodeID = config.NodeID();
  auto senderID = msg_.NodeID();

  if (state_.Accepted() && state_.AcceptedExpireTime() < util::GetMilliTimestamp()) {
    lease_timeout_timer_->cancel();
    OnLeaseTimeout();
  }

  if (msg_.ProposalID() < state_.PromisedProposalID()) {
    std::cout << "[INFO] Acceptor: Because of promisedID, reject propose request" << std::endl;
    msg_to_send.ProposeRejected(nodeID, msg_.ProposalID());
  } else {
    state_.SetAccepted(true);
    state_.SetAcceptedProposalID(msg_.ProposalID());
    state_.SetAcceptedLeaseOwner(msg_.LeaseOwner());
    state_.SetAcceptedDuration(msg_.Duration());
    state_.SetAcceptedExpireTime(util::GetMilliTimestamp() + state_.AcceptedDuration());

    asio::error_code ec;
    lease_timeout_timer_->expires_from_now(std::chrono::milliseconds(state_.AcceptedDuration()), ec);
    lease_timeout_timer_->async_wait([this](asio::error_code ec) {
      this->OnLeaseTimeout();
    });
    std::cout << "[INFO]Acceptor: Accepted propose request" << std::endl;
    msg_to_send.ProposeAccepted(nodeID, msg_.ProposalID());
  }

  SendResponse(senderID, msg_to_send);
}

void Acceptor::SendResponse(const std::string &node_id, const Message &msg) {
  network_->SendMessage(node_id, msg);
}

}