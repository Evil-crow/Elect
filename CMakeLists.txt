cmake_minimum_required(VERSION 3.12)
project(Elect)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
add_subdirectory(src/test)
set(SRC_CONFIG
        src/config/config.h src/config/config.cc)
set(SRC_MESSAGE
        src/message/paxos_msg.proto
        src/message/paxos_msg.pb.cc src/message/paxos_msg.pb.h
        src/message/message.cc src/message/message.h)
set(SRC_NETWORK
        src/network/network.h src/network/network.cc
        src/network/write_session.cc src/network/write_session.h
        src/network/read_session.cc src/network/read_session.h)
set(SRC_STATE
        src/state/proposer_state.cc src/state/proposer_state.h
        src/state/acceptor_state.cc src/state/acceptor_state.h
        src/state/learner_state.cc  src/state/learner_state.h)
set(SRC_ALGORITHM
        src/algorithm/proposer.h src/algorithm/proposer.cc
        src/algorithm/acceptor.h src/algorithm/acceptor.cc
        src/algorithm/learner.h  src/algorithm/learner.cc
        src/algorithm/paxos_lease.h src/algorithm/paxos_lease.cc)
set(SRC_UTIL
        src/util/util.cc src/util/util.h)

