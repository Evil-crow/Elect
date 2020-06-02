
#ifndef ELECT_C_H
#define ELECT_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct elect_t elect_t;
typedef void(*learn_handler)(void *, const char *, const char *, const char *);
typedef void(*timeout_handler)(void *);

elect_t *elect_create();
void elect_free(elect_t *ptr);
void elect_acquire_lease(elect_t *ptr);
void elect_stop(elect_t *ptr);
void elect_continue(elect_t *ptr);
void elect_set_learn_handler(elect_t *ptr, learn_handler handler, void *data);
void elect_set_changed_handler(elect_t *ptr, learn_handler handler, void *data);
void elect_set_timeout_handler(elect_t *ptr, timeout_handler handler, void *data);

#ifdef __cplusplus
};
#endif

#endif //ELECT_C_H
