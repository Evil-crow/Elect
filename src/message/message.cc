
#include "message.h"

namespace elect {

Message::Message(const PaxosMsg &msg) {
  msg_.CopyFrom(msg);
}

void Message::Init(elect::PaxosMsg::Type type, const std::string &nodeID) {
  msg_.set_type(type);
  msg_.set_node_id(nodeID);
}

bool Message::PrepareRequest(const std::string &nodeID, uint64_t proposalID, uint64_t version) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PREPARE_REQUEST, nodeID);
  msg_.set_proposal_id(proposalID);
  msg_.set_version(version);
  return true;
}

bool Message::PrepareRejected(const std::string &nodeID, uint64_t proposalID, uint64_t version) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PREPARE_REJECT, nodeID);
  msg_.set_proposal_id(proposalID);
  msg_.set_version(version);

  return true;
}

bool Message::PrepareAccepted(
  const std::string& nodeID,
  uint64_t proposalID,
  uint64_t accepted_propposalID,
  const std::string& lease_owner,
  uint64_t duration) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PREPARE_ACCEPTED, nodeID);
  msg_.set_proposal_id(proposalID);
  msg_.set_accepted_proposal_id(accepted_propposalID);
  msg_.set_lease_owner(lease_owner);
  msg_.set_duration(duration);

  return true;
}

bool Message::PrepareOpening(const std::string& nodeID, uint64_t proposalID) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PREPARE_OPENING, nodeID);
  msg_.set_proposal_id(proposalID);

  return true;
}

bool Message::ProposeRequest(
  const std::string& nodeID,
  uint64_t proposalID,
  const std::string& lease_owner,
  uint64_t duration) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REQUEST, nodeID);
  msg_.set_proposal_id(proposalID);
  msg_.set_lease_owner(lease_owner);
  msg_.set_duration(duration);

  return true;
}

bool Message::ProposeRejected(const std::string& nodeID, uint64_t proposalID) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REJECT, nodeID);
  msg_.set_proposal_id(proposalID);

  return true;
}

bool Message::ProposeAccepted(const std::string& nodeID, uint64_t proposalID) {
  Init(PaxosMsg::Type::PaxosMsg_Type_PROPOSE_ACCEPTED, nodeID);
  msg_.set_proposal_id(proposalID);

  return true;
}

bool Message::LearnChosen(
  const std::string& nodeID,
  const std::string& lease_owner,
  uint64_t duration,
  uint64_t expiretime,
  uint64_t version) {
  Init(PaxosMsg::Type::PaxosMsg_Type_LEAEN_CHOSEN, nodeID);
  msg_.set_lease_owner(lease_owner);
  msg_.set_duration(duration);
  msg_.set_expire_time(expiretime);
  msg_.set_version(version);

  return true;
}

void Message::Clear() {
  msg_.Clear();
}

void Message::Assign(const PaxosMsg &msg) {
  msg_.CopyFrom(msg);
}

bool Message::ConnectOnlineNode(const std::string &nodeID) {
  Init(PaxosMsg::Type::PaxosMsg_Type_ONLINE_TO_CONNECT, nodeID);

  return true;
}

bool Message::IsRequest() const {
  return (msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_REQUEST ||
  msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REQUEST);
}

bool Message::IsResponse() const {
  return IsPrepareResponse() || IsProposeResponse();
}

bool Message::IsPrepareResponse() const {
  return (msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_OPENING ||
  msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_ACCEPTED ||
  msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PREPARE_REJECT);
}

bool Message::IsProposeResponse() const {
  return (msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PROPOSE_ACCEPTED ||
  msg_.type() == PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REJECT);
}

bool Message::IsConnectOnline() const {
  return msg_.type() == PaxosMsg::Type::PaxosMsg_Type_ONLINE_TO_CONNECT;
}

bool Message::IsLearnChosen() const {
  return msg_.type() == PaxosMsg::Type::PaxosMsg_Type_LEAEN_CHOSEN;
}

}