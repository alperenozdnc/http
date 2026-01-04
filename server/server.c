#include <http/ctx.h>
#include <http/def.h>
#include <http/misc.h>
#include <http/server.h>

#include <http/utils/build_path.h>
#include <http/utils/get_req_pathname.h>
#include <http/utils/print_http_header.h>
#include <http/utils/respond.h>

#include <ezcli/print.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool server(in_port_t port, ctx_s *ctx) {
    bool ret = true;
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcp_socket < 0)
        return false; // no sockets to close yet, not 'goto exit'ing

    int OPT_REUSEADDR = SO_REUSEADDR;

    setsockopt(tcp_socket, SOL_SOCKET, OPT_REUSEADDR, &OPT_REUSEADDR,
               sizeof(OPT_REUSEADDR));

    struct in_addr addr = {.s_addr = htonl(INADDR_LOOPBACK)}; // localhost

    struct sockaddr_in _socket_addr = {
        .sin_family = AF_INET, .sin_addr = addr, .sin_port = port};

    memset(&_socket_addr.sin_zero, 0, sizeof(_socket_addr.sin_zero));

    struct sockaddr *socket_addr = (struct sockaddr *)&_socket_addr;

    if (bind(tcp_socket, socket_addr, sizeof(*socket_addr)) < 0) {
        perror("bind()");

        ret = false;
        goto exit;
    }

    if (listen(tcp_socket, SOMAXCONN) < 0) {
        perror("listen()");

        ret = false;
        goto exit;
    }

    while (true) {
        socklen_t socket_len = sizeof(*socket_addr);
        int client_fd = accept(tcp_socket, socket_addr, &socket_len);

        if (client_fd < 0) {
            perror("accept()");

            ret = false;
            goto exit;
        }

        char req[HTTP_MAX_REQUEST_SIZE];
        memset(req, 0, HTTP_MAX_REQUEST_SIZE);
        read(client_fd, req, HTTP_MAX_REQUEST_SIZE);

        if (!(*req)) {
            close(client_fd);

            continue;
        }

        PRINT_ACTION_INFO(HTTP_REQUEST_PREFIX, req);

        char *pathname = get_req_pathname(req);
        char *path = build_path(ctx, pathname);

        respond(ctx, client_fd, path);

        free(path);
        free(pathname);

        close(client_fd);
    }

    goto exit;

exit:
    close(tcp_socket);

    return ret;
}
