
#include "util.h"

#include <sys/time.h>

namespace elect {
namespace util {

uint64_t GetMilliTimestamp() {
  uint64_t now;
  struct timeval tv{0, 0};

  gettimeofday(&tv, nullptr);

  now = tv.tv_sec;
  now *= 1000;
  now += tv.tv_usec / 1000;

  return now;
}

uint64_t GetMicroTimestamp() {
  uint64_t now;
  struct timeval tv{0, 0};

  gettimeofday(&tv, nullptr);

  now = tv.tv_sec;
  now *= 1000000;
  now += tv.tv_usec;

  return now;
}

uint64_t NewProposalID(uint64_t proposalID, uint64_t nodeNo) {
  // <proposal count since restart> <nodeNo>
  proposalID = proposalID >> 8;
  proposalID++;
  proposalID = proposalID << 8;

  uint64_t nextProposalID = proposalID | nodeNo;
  return nextProposalID;
}

}
}