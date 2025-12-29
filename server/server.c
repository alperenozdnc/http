#include <http/def.h>
#include <http/misc.h>
#include <http/server.h>

#include <http/utils/print_http_header.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <ezcli/print.h>

#define EXAMPLE_RESPONSE                                                       \
    "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: "           \
    "14\r\n\r\n<h1>Hello</h1>\r\n\r\n";

bool server(in_port_t port) {
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

    for (;;) {
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
        PRINT_ACTION_INFO(HTTP_REQUEST_PREFIX, req);

        char *res = EXAMPLE_RESPONSE;
        write(client_fd, res, strlen(res));
        PRINT_ACTION_INFO(HTTP_RESPONSE_PREFIX, res);

        close(client_fd);
    }

    goto exit;

exit:
    close(tcp_socket);

    return ret;
}
