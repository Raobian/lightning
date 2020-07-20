#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define DBG_SUBSYS S_LTG_NET

#include "ltg_net.h"
#include "ltg_utils.h"

int sock_destroy(ltg_sock_conn_t *sock)
{
        (void) sock_rbuffer_destroy(&sock->rbuf);

        (void) sock_wbuffer_destroy(&sock->wbuf);

        sock->send_total = 0;
        sock->recv_total = 0;
        sock->nh.u.sd.sd = -1;
        sock->nh.u.sd.seq = -1;
        sock->nh.u.sd.addr = -1;
        sock->sendclose = 0;

        return 0;
}

inline int sock_sdclose(int sd)
{
        return tcp_sock_close(sd);
}

int sock_close(ltg_sock_conn_t *sock)
{
        int ret;

        if (sock->nh.u.sd.sd != -1) 
                (void) sock_sdclose(sock->nh.u.sd.sd);

        sock->used = 0;
        ret = sock_destroy(sock);
        if (unlikely(ret))
                GOTO(err_ret, ret);

        return 0;
err_ret:
        return ret;
}

int sock_init(ltg_sock_conn_t *sock, sock_info_t *info)
{
        int ret;

        (void) info;

        sock->send_total = 0;
        sock->recv_total = 0;
        sock->limit_rate = 0;
        sock->nh.u.sd.sd = -1;
        sock->used = 0;

        ret = sock_rbuffer_create(&sock->rbuf);
        if (unlikely(ret))
                GOTO(err_ret, ret);

        ret = sock_wbuffer_create(&sock->wbuf);
        if (unlikely(ret))
                GOTO(err_ret, ret);

        return 0;
err_ret:
        return ret;
}

int sock_info2sock(net_handle_t *nh, const sock_info_t *info, int nonblock, int timeout)
{
        int ret;
        struct sockaddr_in sin;

        LTG_ASSERT(timeout <= 1000 * 1000 && timeout >= 100 * 1000);
        
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;

        sin.sin_addr.s_addr = info->addr;
        sin.sin_port = info->port;

        DBUG("try to connect %s:%u/%u\n", inet_ntoa(sin.sin_addr),
              ntohs(info->port), info->port);

        nh->u.sd.sd = -1;
        ret = tcp_sock_connect(nh, &sin, nonblock, timeout, 1);
        if (unlikely(ret)) {
                DBUG("try to connect %s:%u/%u (%u) %s\n", inet_ntoa(sin.sin_addr),
                      ntohs(info->port), info->port, ret, strerror(ret));
                GOTO(err_ret, ret);
        }

        return 0;
err_ret:
        return ret;
}
