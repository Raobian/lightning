#ifndef __ECTD_H__
#define __ECTD_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <netinet/in.h>

#include "ltg_list.h"
#include "macros.h"
#include "ltg_id.h"

#define ETCD_NETWORK "network"
#define ETCD_MANAGE ETCD_NETWORK"/manage"
#define ETCD_NID ETCD_NETWORK"/nid"
#define ETCD_CORENET ETCD_NETWORK"/corenet"

typedef struct{
        int running;
        int retval;
        int ttl;
        int update;
        uint32_t magic;
        sem_t sem;
        sem_t stoped;
        char hostname[MAX_NAME_LEN];
        char key[MAX_PATH_LEN];
} etcd_lock_t;

typedef int (*etcd_func_t)(const char *value, int idx, void *arg);
typedef int (*etcd_dir_func_t)(int idx, void *arg);

int etcd_mkdir(const char *prefix, const char *dir, int ttl);
int etcd_del_dir(const char *prefix, const char *_key, int recursive);

int etcd_readdir(const char *_key, char *buf, int *buflen);

int etcd_del(const char *prefix, const char *_key);
int etcd_del2(char *key);

int etcd_list1(const char *prefix, const char *_key, etcd_node_t **_node);
int etcd_list(const char *_key, etcd_node_t **_node);

int etcd_create_text(const char *prefix, const char *_key, const char *_value, int ttl);
int etcd_set_text(const char *prefix, const char *_key, const char *_value,
                  int flag, int ttl);
int etcd_update_text(const char *prefix, const char *_key, const char *_value, int  *idx, int ttl);
int etcd_get_text (const char *prefix, const char *_key, char *value, int *idx);

int etcd_create(const char *prefix, const char *_key, const void *_value, int valuelen, int ttl);
int etcd_set_bin(const char *prefix, const char *_key, const void *_value,
                 int valuelen, int flag, int ttl);
int etcd_update(const char *prefix, const char *_key, const void *_value, int valuelen,
                int *idx, int ttl);
int etcd_get_bin(const char *prefix, const char *_key, void *_value, int *_valuelen, int *idx);

int etcd_lock_init(etcd_lock_t *lock, const char *prefix, const char *key, int ttl, uint32_t magic, int update);
int etcd_lock(etcd_lock_t *lock);
int etcd_unlock(etcd_lock_t *lock);
int etcd_watch_key(const char *prefix, const char *_key, int timeout,
                   etcd_func_t func, void *arg);
int etcd_watch_dir(const char *prefix, const char *_key, int timeout,
                   etcd_dir_func_t func, void *arg);

// master
int etcd_lock_health(etcd_lock_t *lock);

// slave
int etcd_locker(etcd_lock_t *lock, char *locker, nid_t *nid, uint32_t *_magic, int *idx);
int etcd_lock_watch(etcd_lock_t *lock, char *value, nid_t *nid, uint32_t *magic, int *idx);

int etcd_init();

#endif
