#ifndef __LTG_DEF_H__
#define __LTG_DEF_H__

#define LTG_TLS_MAX 256

#define LTG_MSG_MAX 256

#define MAX_PATH_LEN    (1024 * 4)
#define MAX_NAME_LEN    (256)
#define MAX_INFO_LEN    (512)

#define MAX_BUF_LEN     (1024 * 4)

#define PAGE_SIZE (1024 * 4)

#define MAX_LINE_LEN    (1024 * 2)

typedef unsigned long long LLU;
typedef long long LLD;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define BUFFER_SEG_SIZE (2 * 1024 * 1024)
#define RDMA_MESSAGE_SIZE 512

#define IO_MAX (1024 * 1024 * 4)

#define MAX_SGE  (IO_MAX / BUFFER_SEG_SIZE + 1)
#define RDMA_INFO_SIZE 1024

#define LNET_SERVICE_BASE 49152
#define LNET_SERVICE_RANGE (65535 - 49152)

#define NODEID_MAX (INT16_MAX)

#define MAXSIZE 256

#define SCHEDULE_MAX  (1024 * 2)

#define MAX_MSG_SIZE (512)
#define UUID_LEN        (64)
#define MAX_NET_COUNT 12

#define SOCKID_NORMAL 10
#define SOCKID_CORENET 20

#define ENABLE_HUGEPAGE 1

#define ENABLE_ANALYSIS 0

#endif
