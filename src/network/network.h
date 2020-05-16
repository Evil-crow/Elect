
#ifndef ELECT_NETWORK_H
#define ELECT_NETWORK_H

#include <map>
#include <vector>
#include <memory>
#include <functional>

#include <asio.hpp>
#include "message/message.h"

namespace elect {

class ReadSession;
class WriteSession;

using ReadSessionMaps = std::map<std::string, std::shared_ptr<ReadSession>>;
using WriteSessionMaps = std::map<std::string, std::shared_ptr<WriteSession>>;
using ReadHandler = std::function<void (const Message &msg)>;

class Network {
 public:
  Network() noexcept;
  Network(const Network &) = delete;
  Network &operator=(const Network &) = delete;
  Network(Network &&) noexcept = default;
  Network &operator=(Network &&) noexcept = default;
  ~Network();

  // network action
  void Start();
  void Stop();
  void NewConnetion(const std::string &node_id);
  void SetReadHandler(ReadHandler &&handler);
  void BroadcastMessage(const Message &msg);
  void SendMessage(const std::string& node_id, const Message &msg);

  // getter/setter
  inline std::shared_ptr<asio::io_context> GetIOContext() { return ctx_; }
  void RemoveReadSession(const std::string &node_id);
  void RemoveWriteSession(const std::string &node_id);

 private:
  void DoAccept();

  std::shared_ptr<asio::io_context> ctx_;
  std::shared_ptr<asio::ip::tcp::acceptor> acceptor_;
  ReadSessionMaps read_sessions_;
  WriteSessionMaps write_sessions_;
  ReadHandler handler_;
};

}

#endif //ELECT_NETWORK_H
