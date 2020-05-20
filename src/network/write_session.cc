//
// Created by Crow on 5/14/20.
//

#include "write_session.h"

#include <glog/logging.h>
#include <google/protobuf/util/delimited_message_util.h>

#include "network.h"

namespace elect {

std::map<PaxosMsg::Type, std::string> map = {
  {PaxosMsg::Type::PaxosMsg_Type_NONE, "noop"},
  {PaxosMsg::Type::PaxosMsg_Type_PREPARE_REQUEST, "Proposer: prepare request"},
  {PaxosMsg::Type::PaxosMsg_Type_PREPARE_REJECT, "Acceptor: prepare reject"},
  {PaxosMsg::Type::PaxosMsg_Type_PREPARE_ACCEPTED, "Acceptor: prepare accepted"},
  {PaxosMsg::Type::PaxosMsg_Type_PREPARE_OPENING, "Acceptor: prepare opening"},
  {PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REQUEST, "Proposer: propose request"},
  {PaxosMsg::Type::PaxosMsg_Type_PROPOSE_REJECT, "Acceptor: propose reject"},
  {PaxosMsg::Type::PaxosMsg_Type_PROPOSE_ACCEPTED, "Acceptor: propose accept"},
  {PaxosMsg::Type::PaxosMsg_Type_LEAEN_CHOSEN, "Learner: learn chosen"},
  {PaxosMsg::Type::PaxosMsg_Type_ONLINE_TO_CONNECT, "online to connect"},
};

WriteSession::WriteSession(
  std::shared_ptr<asio::io_context> ctx,
  asio::ip::tcp::socket socket,
  Network *network) :
  ctx_(std::move(ctx)),
  socket_(std::make_unique<asio::ip::tcp::socket>(std::move(socket))),
  network_(network){
  auto endpoint = socket_->remote_endpoint();
  auto ip = endpoint.address().to_string();
  auto port = endpoint.port();
  node_id.assign(std::string(ip + ":" + std::to_string(port)));
}

WriteSession::~WriteSession() {
  if (socket_->is_open()) {
    socket_->close();
    network_->RemoveWriteSession(node_id);
  }
}

void WriteSession::SendMessage(const Message &msg) {
  auto self = shared_from_this();
  auto output_stream = std::make_shared<asio::streambuf>();
  std::ostream output(output_stream.get());

  auto nodeid = msg.ProposalID();
  auto real_id = nodeid >> 8;
  auto real_no = nodeid & 255;
  LOG(INFO) << "Type: " << map[msg.MsgType()] << std::endl
            << "To: " << socket_->remote_endpoint().address().to_string() << ":" << socket_->remote_endpoint().port() << std::endl
            << "NodeID: " << msg.NodeID() << std::endl
            << "ProposalID: " << msg.ProposalID() << "[" << real_id << ":" << real_no << "]" << std::endl
            << "LeaseOwner: " << msg.LeaseOwner() << std::endl
            << "ExpireTime: " << msg.ExpireTime() << std::endl
            << "Duration: " << msg.Duration() << std::endl;

  if (google::protobuf::util::SerializeDelimitedToOstream(msg.Msg(), &output)) {
    asio::async_write(*socket_, *output_stream, [this, self, output_stream](asio::error_code ec, size_t transfer) {
      if (!ec) {
        std::cout << "[SYS]All message send Ok!";
      } else {
        if (socket_->is_open()) {
          socket_->close();
          network_->RemoveWriteSession(node_id);
        }
      }
    });
  }
}


}
