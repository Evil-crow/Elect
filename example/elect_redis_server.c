/* based on redis-5.0.9 */

static void HandleMasterChanged(void *data, const char *info, const char *addr, const char *id) {
  UNUSED(data);

  char ip[20];
  int len = 0;
  char *pos = strchr(addr, ':');
  long port = strtol(pos + 1, NULL, 10);
  for (; addr[len] != ':'; ++len) {
    ip[len] = addr[len];
  }
  ip[len] = '\0';

  if (server.cluster_enabled) {
    serverLog(LL_NOTICE,"REPLICAOF not allowed in cluster mode.");
    return;
  }

  /* this means: elect master is server itself. */
  if (!strcmp(info, id)) {
    if (server.masterhost) {
      replicationUnsetMaster();
      serverLog(LL_NOTICE,"MASTER MODE enabled");
    }
  } else {

    /* Check if we are already attached to the specified slave */
    if (server.masterhost && !strcasecmp(server.masterhost,ip)
        && server.masterport == port) {
      serverLog(LL_NOTICE,"REPLICAOF would result into synchronization with the master we are already connected with. No operation performed.");
      return;
    }
    /* There was no previous master or the user specified a different one,
     * we can continue. */
    replicationSetMaster(ip, (int)port);
    serverLog(LL_NOTICE,"REPLICAOF %s:%d enabled",
              server.masterhost, server.masterport);
  }
}

#include <elect/c.h>

static void *thread_elect_start(void *arg) {
  elect_t *elect = (elect_t *)(arg);
  elect_acquire_lease(elect);
}

int main(int argc, char **argv) {

  /* ...... */

  /* start elect action in another thread */
  pthread_t id;
  void *res;
  elect_t *elect = elect_create();
  elect_set_changed_handler(elect, HandleMasterChanged, &server);
  pthread_create(&id, NULL, thread_elect_start, (void *)elect);

  aeSetBeforeSleepProc(server.el,beforeSleep);
  aeSetAfterSleepProc(server.el,afterSleep);
  aeMain(server.el);
  pthread_join(id, &res);
  aeDeleteEventLoop(server.el);
  return 0;
}