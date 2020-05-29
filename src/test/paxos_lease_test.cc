#include "algorithm/paxos_lease.h"

void Handler(void *ptr, const char *str, const char *node_id) {
  (void)(ptr);
  std::cout << "Elect master: " << str << std::endl;
  std::cout << "Node ID: " << node_id << std::endl;
}

int main(int argc, char *argv[]) {
  void *ptr = nullptr;
  elect::PaxosLease lease;
//  lease.SetOnLearnLease(Handler, ptr);
  lease.SetOnLeaseChange(Handler, ptr);

  lease.AcquireLease();
}