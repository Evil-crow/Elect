
#include "c.h"
#include "algorithm/elect.h"

extern "C" {
using elect::Elect;

struct elect_t {
  Elect *lease;
};

elect_t *elect_create() {
  auto lease_ptr = new Elect;
  auto ptr = new elect_t{lease_ptr};

  return ptr;
}

void elect_free(elect_t *ptr) {
  free(ptr->lease);
  free(ptr);
}

void elect_acquire_lease(elect_t *ptr) {
  ptr->lease->AcquireLease();
}

void elect_stop(elect_t *ptr) {
  ptr->lease->Stop();
}

void elect_continue(elect_t *ptr) {
  ptr->lease->Continue();
}

void elect_set_learn_handler(elect_t *ptr, learn_handler handler, void *data) {
  ptr->lease->SetOnLearnLease(handler, data);
}

void elect_set_changed_handler(elect_t *ptr, learn_handler handler, void *data) {
  ptr->lease->SetOnLeaseChange(handler, data);
}

void elect_set_timeout_handler(elect_t *ptr, timeout_handler handler, void *data) {
  ptr->lease->SetOnLeaseTimeout(handler, data);
}
}