
#ifndef ELECT_MESSAGE_H
#define ELECT_MESSAGE_H

#include "message/paxos_msg.pb.h"

namespace elect {

class Message {
 public:
  Message() = default;
  explicit Message(const PaxosMsg &msg);
  Message(const Message &) = default;
  Message &operator=(const Message &) = default;
  Message(Message &&) = default;
  Message &operator=(Message &&) = default;

  // prepare msg for transport
  bool PrepareRequest(const std::string &nodeID, uint64_t proposalID, uint64_t version);
  bool PrepareRejected(const std::string& nodeID, uint64_t proposalID, uint64_t version);
  bool PrepareAccepted(const std::string& nodeID, uint64_t proposalID, uint64_t accepted_propposalID, const std::string& lease_owner, uint64_t duration);
  bool PrepareOpening(const std::string& nodeID, uint64_t proposalID);

  bool ProposeRequest(const std::string& nodeID, uint64_t proposalID, const std::string& lease_owner, uint64_t duration);
  bool ProposeRejected(const std::string& nodeID, uint64_t proposalID);
  bool ProposeAccepted(const std::string& nodeID, uint64_t proposalID);

  bool LearnChosen(const std::string& nodeID, const std::string &addr, const std::string& lease_owner, uint64_t duration, uint64_t expiretime, uint64_t version);
  bool ConnectOnlineNode(const std::string &nodeID);

  void Clear();
  void Assign(const PaxosMsg &msg);

  // get info from msg
  bool IsRequest() const;
  bool IsResponse() const;
  bool IsPrepareResponse() const;
  bool IsProposeResponse() const;
  bool IsConnectOnline() const;
  bool IsLearnChosen() const;

  inline PaxosMsg::Type MsgType() const { return msg_.type(); }
  inline std::string NodeID() const { return msg_.node_id(); }
  inline std::string Addr() const { return msg_.addr(); }
  inline uint64_t ProposalID() const { return msg_.proposal_id(); }
  inline uint64_t AcceptedProposalID() const { return msg_.accepted_proposal_id(); }
  inline std::string LeaseOwner() const { return msg_.lease_owner(); }
  inline uint64_t Duration() const { return msg_.duration(); }
  inline uint64_t ExpireTime() const { return msg_.expire_time(); }
  inline uint64_t Version() const { return msg_.version(); }
  inline const PaxosMsg &Msg() const { return msg_; }

 private:
  void Init(elect::PaxosMsg::Type type, const std::string &nodeID);

  PaxosMsg msg_;
};

}

#endif //ELECT_MESSAGE_H
