
#ifndef ELECT_UTIL_H
#define ELECT_UTIL_H

#include <cinttypes>

namespace elect {
namespace util {

  uint64_t GetMilliTimestamp();

  uint64_t GetMicroTimestamp();

  uint64_t NewProposalID(uint64_t proposalID, uint64_t nodeNo);

}
}

#endif //ELECT_UTIL_H
