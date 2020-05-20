
#include "network.h"

#include "config/config.h"
#include "write_session.h"
#include "read_session.h"

#include <glog/logging.h>

#include <utility>

namespace elect {

Network::Network() noexcept :
    ctx_(std::make_shared<asio::io_context>()),
    acceptor_(std::make_shared<asio::ip::tcp::acceptor>(*ctx_)),
    handler_(nullptr) {

  // prepare acceptor for accept new connection
  const auto &config = Config::GetInstance();
  auto address = asio::ip::make_address(config.Ip());
  asio::ip::tcp::endpoint ep(address, config.Port());

  acceptor_->open(ep.protocol());
  acceptor_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_->bind(ep);
  acceptor_->listen();
  DoAccept();

  // connect to the other nodes, for initlization
  auto nodes = config.NodeAddress();
  for (const auto &var : nodes) {
    asio::ip::tcp::endpoint ep_var(asio::ip::make_address(var.first), var.second);
    auto sock_var = std::make_shared<asio::ip::tcp::socket>(*ctx_);
    sock_var->async_connect(ep_var, [this, sock_var, var](asio::error_code ec) {
      if (!ec) {
        auto ptr = std::make_shared<WriteSession>(ctx_, std::move(*sock_var), this);
        auto node_id = std::string(var.first + ":" + std::to_string(var.second));
        write_sessions_.emplace(node_id, ptr);

        // to notify other node -> this node is online, can connect it
        Message msg;
        const auto &config = Config::GetInstance();
        msg.ConnectOnlineNode(config.NodeID());
        SendMessage(node_id, msg);
      } else {
        LOG(INFO) << "Can't connect now ["
                  << var.first
                  << ":"
                  << var.second
                  << "]";
      }
    });
  }
}

Network::~Network() {
  Stop();
  read_sessions_.clear();
  write_sessions_.clear();
}

void Network::Start() {
  ctx_->run();
}

void Network::Stop() {
  ctx_->stop();
}

void Network::NewConnetion(const std::string &node_id) {
  if (write_sessions_.count(node_id))
    return;

  auto pos = node_id.find(':');
  auto ip = node_id.substr(0, pos);
  auto port = ::strtol(node_id.c_str() + pos + 1, nullptr, 10);

  asio::ip::tcp::endpoint ep(asio::ip::make_address(ip), port);
  auto socket = std::make_shared<asio::ip::tcp::socket>(*ctx_);
  socket->async_connect(ep, [this, socket, node_id](asio::error_code ec) {
    if (!ec) {
      auto ptr = std::make_shared<WriteSession>(ctx_, std::move(*socket), this);
      write_sessions_.emplace(node_id, ptr);
    } else {
      LOG(INFO) << "Can't connect now ["
                << node_id
                << "]";
    }
  });
}

void Network::SetReadHandler(ReadHandler handler) {
  handler_ = std::move(handler);
}

void Network::DoAccept() {
  acceptor_->async_accept([this](asio::error_code ec, asio::ip::tcp::socket socket) {
    if (!ec) {
      auto endpoint = socket.remote_endpoint();
      auto ip = endpoint.address().to_string();
      auto port = endpoint.port();
      auto node_id = std::string(ip + ":" + std::to_string(port));
      auto ptr = std::make_shared<ReadSession>(this->ctx_, std::move(socket), this);
      read_sessions_.emplace(node_id, ptr);

      ptr->SetMessageHandler(handler_);
      ptr->Start();
    } else {
      LOG(INFO) << "Accept error ["
                << ec.message()
                << "]";
    }
    DoAccept();
  });
}

void Network::BroadcastMessage(const Message &msg) {
  for (const auto &var : write_sessions_) {
    var.second->SendMessage(msg);
  }
}

void Network::SendMessage(const std::string& node_id, const Message &msg) {
  if (write_sessions_.count(node_id)) {
    write_sessions_[node_id]->SendMessage(msg);
  }
}

void Network::RemoveReadSession(const std::string &node_id) {
  if (read_sessions_.count(node_id)) {
    read_sessions_.erase(node_id);
  }
}

void Network::RemoveWriteSession(const std::string &node_id) {
  if (write_sessions_.count(node_id)) {
    write_sessions_.erase(node_id);
  }
}

}