//
// Created by Crow on 5/14/20.
//

#include "write_session.h"

#include <glog/logging.h>
#include <google/protobuf/util/delimited_message_util.h>

#include "network.h"

namespace elect {

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

  if (google::protobuf::util::SerializeDelimitedToOstream(msg.Msg(), &output)) {
    asio::async_write(*socket_, *output_stream, [this, self, output_stream](asio::error_code ec, size_t transfer) {
      if (!ec) {
        LOG(INFO) << "All message send Ok!";
      } else {
        LOG(INFO) << "ReadSession peer connection closed ["
                  << socket_->remote_endpoint().address().to_string()
                  << ":"
                  << socket_->remote_endpoint().port()
                  << "]";
        if (socket_->is_open()) {
          socket_->close();
          network_->RemoveWriteSession(node_id);
        }
      }
    });
  }
}


}
