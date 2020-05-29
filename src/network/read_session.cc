
#include "read_session.h"

#include <iostream>
#include <utility>

#include <google/protobuf/util/delimited_message_util.h>

#include "network.h"

namespace elect {

ReadSession::ReadSession(
  std::shared_ptr<asio::io_context> ctx,
  asio::ip::tcp::socket socket,
  Network *network) noexcept :
  ctx_(std::move(ctx)),
  socket_(std::make_unique<asio::ip::tcp::socket>(std::move(socket))),
  handler_(nullptr),
  network_(network){
  auto endpoint = socket_->remote_endpoint();
  auto ip = endpoint.address().to_string();
  auto port = endpoint.port();
  node_id.assign(std::string(ip + ":" + std::to_string(port)));
}

ReadSession::~ReadSession() {
  if (socket_->is_open()) {
    socket_->close();
  }
  network_->RemoveReadSession(node_id);
}

void ReadSession::SetMessageHandler(MessageHandler handler) {
  handler_ = std::move(handler);
}

void ReadSession::Start() {
  DoRead();
}

void ReadSession::DoRead() {
  auto self = shared_from_this();
  auto buf = input_stream_.prepare(4096);
  socket_->async_read_some(buf, [this, self](asio::error_code ec, size_t transfer) {
    if (!ec || (transfer != 0 && ec == asio::error::eof)) {
      input_stream_.commit(transfer);
      OnReadHandle();
    } else {
      if (socket_->is_open()) {
        socket_->close();
        network_->RemoveReadSession(node_id);
      }
    }
  });
}

void ReadSession::OnReadHandle() {
  bool eof, res{true};
  std::vector<Message> vec;
  std::istream input(&input_stream_);                          // asio::stream_buf -> std::istream
  google::protobuf::io::IstreamInputStream input_stream(&input);  // std::istream -> google::protobuf::io::IstreamInputStream (inherit from ZeroCopyStream)

  while (res) {
    PaxosMsg msg;
    res = google::protobuf::util::ParseDelimitedFromZeroCopyStream(&msg, &input_stream, &eof);
    if (res) {
      vec.emplace_back(msg);
    }
  }

  if (handler_) {
    for (const auto &var : vec) {
      handler_(var);
    }
  }
  DoRead();
}

}