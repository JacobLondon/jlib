#ifndef JLIB_SOCK_H
#define JLIB_SOCK_H

enum sock_conn {
	SOCK_CONN_NONE,
	SOCK_CONN_CLIENT,
	SOCK_CONN_SERVER,
	SOCK_CONN_COUNT
};

/*typedef struct sock_conn {
	enum socket_conn_type type;
	int port;
	const char *ip;
} sock_conn;*/

typedef struct sock {
	int family;
	int type;
	sock_conn conn;
} sock;


// https://github.com/doodleincode/c-socket-lib
// https://www.geeksforgeeks.org/udp-server-client-implementation-c/

/**
 * TCP Server
 * sock *server = sock_new(AF_INET, SOCK_STREAM);
 * sock_bind(server, NULL, 8765); // any ip, given port
 * sock_listen(server, 5);        // 5 connections
 * sock *client = sock_accept(server);
 * while (1) {
 *     sock_recv(client, buf, sizeof(buf));
 *     sock_send(client, buf, sizeof(buf));
 * }
 * sock_del(client);
 * sock_del(server);
 * 
 * TCP Client
 * sock *remote = sock_new(AF_INET, SOCK_STREAM);
 * sock_connect(s, "127.0.0.1", 8765);
 * while (1) {
 *     sock_send(remote, buf, sizeof(buf));
 *     sock_recv(remote, buf, sizeof(buf));
 * }
 * sock_del(remote);
 * 
 * UDP Server
 * sock *server = sock_new(AF_INET, SOCK_DGRAM);
 * sock_bind(server, NULL, 8765);
 * while (1) {
 *     sock_recv(server, buf, sizeof(buf)); // we know it's recvfrom / sendto from sock_conn in sock
 *     sock_send(server, buf, sizeof(buf));
 * }
 * sock_del(server);
 * 
 * UDP Client
 * sock *remote = sock_new(AF_INET, SOCK_DGRAM);
 * sock_bind(remote, INADDR_ANY, 8765);
 * while (1) {
 *     bytes = sock_recv(remove, buf, sizeof(buf))
 *     sock_send(remote, buf, sizeof(buf));
 * }
 * sock_cleanup(remote)
 * 
 */

sock *sock_new              (int family, int type);
void  sock_del              (sock *self);
void  sock_connect          (sock *self, const char *ip, int port);
void  sock_bind             (sock *self, const char *ip, int port);
void  sock_listen           (sock *self, size_t max_connections);
sock *sock_accept           (sock *server);
int   sock_send             (sock *self, const char *buf, size_t size);
int   sock_recv             (sock *self, char *buf, size_t size);
void  sock_create           (sock *self, int type, const char *ip, int port);

static void  sock_client_add(sock *self);
static void  sock_error     (sock *self, const char *msg);
static char *sock_inet_ntop (sock *self, struct sockaddr *sa);

#endif /* JLIB_SOCK_H */
