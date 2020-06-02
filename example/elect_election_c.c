//
// Created by Crow on 5/30/20.
//

#include "../include/elect/c.h"
#include <stdio.h>
void Handler(void *ptr, const char *str, const char *addr, const char *node_id) {
  (void)(ptr);
  printf("Elect master: %s\n", str);
  printf("Addr: %s\n", addr);
  printf("NodeID: %s\n", node_id);
}

int main(int argc, char *argv[]) {
  printf("This is C version test demo\n");
  void *data = NULL;
  elect_t *ptr = elect_create();
  elect_set_changed_handler(ptr, Handler, data);
  elect_acquire_lease(ptr);

  return 0;
}