//
// Created by Crow on 5/15/20.
//

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <iostream>

#include "network/network.h"
#include "message/message.h"
#include "config/config.h"

#include <glog/logging.h>

void Handler(const elect::Message &msg) {
  switch (msg.MsgType()) {
    case elect::PaxosMsg::Type::PaxosMsg_Type_ONLINE_TO_CONNECT:
      std::cout << "It's To CONNECT [" << msg.NodeID() << "]" << std::endl;
      break;
    case elect::PaxosMsg::Type::PaxosMsg_Type_PREPARE_REQUEST:
      std::cout << "It's prepare request " << std::endl
                << "noedID: " << msg.NodeID() << std::endl
                << "proposalID: " << msg.ProposalID() << std::endl
                << "version: " << msg.Version() << std::endl;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(*argv);
  elect::Network network;
  network.SetReadHandler(Handler);

  std::thread t([&network](){network.Start();});
  sleep(2);
  elect::Message pr_msg;
  const auto &config = elect::Config::GetInstance();
  pr_msg.PrepareRequest(config.NodeID(), 9876543, 1);
  network.BroadcastMessage(pr_msg);

  t.join();
  return 0;
}