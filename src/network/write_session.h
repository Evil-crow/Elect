
#ifndef ELECT_WRITE_SESSION_H
#define ELECT_WRITE_SESSION_H

#include <memory>

#include <asio.hpp>
#include "message/message.h"

namespace elect {

class Network;
class WriteSession : public std::enable_shared_from_this<WriteSession> {
 public:
  WriteSession(std::shared_ptr<asio::io_context> ctx, asio::ip::tcp::socket socket, Network *network);
  ~WriteSession();

  void SendMessage(const Message &msg);

 private:
  Network *network_;
  std::shared_ptr<asio::io_context> ctx_;
  std::unique_ptr<asio::ip::tcp::socket> socket_;
  asio::streambuf output_stream_;
  std::string node_id;
};

}

#endif //ELECT_WRITE_SESSION_H
