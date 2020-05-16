
#ifndef ELECT_READ_SESSION_H
#define ELECT_READ_SESSION_H

#include <memory>

#include <asio.hpp>
#include "message/message.h"

namespace elect {

using MessageHandler = std::function<void (const Message &msg)>;

class Network;
class ReadSession : public std::enable_shared_from_this<ReadSession> {
 public:
  ReadSession(
    std::shared_ptr<asio::io_context> ctx,
    asio::ip::tcp::socket socket,
    Network *network) noexcept;
  ~ReadSession();

  void Start();
  void SetMessageHandler(MessageHandler handler);

 private:
  void OnReadHandle();
  void DoRead();

  std::string node_id;
  Network *network_;
  std::shared_ptr<asio::io_context> ctx_;
  std::unique_ptr<asio::ip::tcp::socket> socket_;
  MessageHandler handler_;
  asio::streambuf input_stream_;
};

}

#endif //ELECT_READ_SESSION_H
