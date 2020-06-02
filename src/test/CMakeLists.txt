cmake_minimum_required(VERSION 3.12)
project(Test)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-g)

set(EXECUTABLE_OUTPUT_PATH ../../bin)

set(SRC_CONFIG
        ../config/config.h ../config/config.cc)
set(SRC_MESSAGE
        ../message/paxos_msg.proto
        ../message/paxos_msg.pb.cc ../message/paxos_msg.pb.h
        ../message/message.cc ../message/message.h)
set(SRC_NETWORK
        ../network/network.h ../network/network.cc
        ../network/write_session.cc ../network/write_session.h
        ../network/read_session.cc ../network/read_session.h)
set(SRC_STATE
        ../state/proposer_state.cc ../state/proposer_state.h
        ../state/acceptor_state.cc ../state/acceptor_state.h
        ../state/learner_state.cc  ../state/learner_state.h)
set(SRC_ALGORITHM
        ../algorithm/proposer.h ../algorithm/proposer.cc
        ../algorithm/acceptor.h ../algorithm/acceptor.cc
        ../algorithm/learner.h  ../algorithm/learner.cc
        ../algorithm/elect.h ../algorithm/elect.cc)
set(SRC_UTIL
        ../util/util.h  ../util/util.cc)

add_executable(config_test
        config_test.cc
        ${SRC_CONFIG})
target_link_libraries(config_test  gtest gtest_main yaml-cpp pthread)

#add_executable(network_test
#        network_test.cc
#        ${SRC_NETWORK}
#        ${SRC_MESSAGE}
#        ${SRC_STATE})
#        ${SRC_STATE})
#target_link_libraries(network_test
#        glog yaml-cpp protobuf pthread)

add_executable(proposer_state_test proposer_state_test.cc ${SRC_STATE})
target_link_libraries(proposer_state_test gtest gtest_main pthread)

add_executable(acceptor_state_test acceptor_state_test.cc ${SRC_STATE})
target_link_libraries(acceptor_state_test gtest gtest_main pthread)

add_executable(learner_state_test learner_state_test.cc ${SRC_STATE})
target_link_libraries(learner_state_test gtest gtest_main pthread)

add_executable(paxos_lease_test paxos_lease_test.cc ${SRC_CONFIG} ${SRC_MESSAGE} ${SRC_NETWORK} ${SRC_STATE} ${SRC_ALGORITHM} ${SRC_UTIL})
target_link_libraries(paxos_lease_test yaml-cpp glog protobuf pthread)