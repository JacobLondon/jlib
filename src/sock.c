#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctypes.h>
#include <string.h>
#include <jlib/sock.h>

#define BUFFER_MAX 1024

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

enum sock_family {
	SOCK_FAMILY_IPV4,
	SOCK_FAMILY_IPV6,
};

enum sock_type {
	SOCK_TYPE_UDP,
	SOCK_TYPE_TCP,
};

enum sock_conn {
	SOCK_CONN_CLIENT,
	SOCK_CONN_SERVER,
};

struct sock {
	SOCKET sock;
	unsigned short port;
	char *ip_address;
	struct sockaddr_in local;
	SOCKET *remote_socks;
	struct sockaddr_in *remotes;
	enum sock_conn conn;
	enum sock_type type;
	enum sock_family family;
	int addrlen;
	unsigned char **buf;
	int num_connections;
};

struct sock *sock_new(enum sock_family family, enum sock_type type, enum sock_conn conn, char *ip_address, unsigned short port)
{
	WSADATA wsa;
	struct hostent *hostp = NULL;
	int ret;
	unsigned int hostaddr;

	struct sock *self = calloc(1, sizeof(struct sock));
	if (self == NULL) {
		goto error;
	}
	self->family = family;
	self->type = type;
	self->conn = conn;
	self->addrlen = sizeof(struct sockaddr_in);
	if (ip_address != NULL) {
		self->ip_address = strdup(ip_address);
		if (self->ip_address == NULL) {
			goto error;
		}
	}

	/* Winsock version 2.2 */
	ret = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (ret != 0) {
		goto error;
	}

	switch (family) {
		case SOCK_FAMILY_IPV4:
			self->local.sin_family = AF_INET;
			break;
		case SOCK_FAMILY_IPV6:
			self->local.sin_family = AF_INET6;
			break;
		default:
			goto error;
	}

	if (conn == SOCK_CONN_SERVER) {
		/* specific address or any */
		self->local.sin_addr.s_addr = ip_address != NULL ? inet_addr(ip_address) : INADDR_ANY;
		self->local.sin_port = htons(port); /* host to network short (big endian for networks) */

		switch (type) {
			case SOCK_TYPE_TCP:
				self->sock = socket(self->local.sin_family, SOCK_STREAM, 0);
				break;
			case SOCK_TYPE_UDP:
				self->sock = socket(self->local.sin_family, SOCK_DGRAM, 0);
				/* forward prepare for the remote connection */
				self->remotes = calloc(1, sizeof(struct sockaddr_in));
				assert(self->remote_socks);
				break;
			default:
				goto error;
		}

		if (self->sock == INVALID_SOCKET) {
			goto error;
		}

		ret = bind(self->sock, (struct sockaddr *) &self->local, sizeof(self->local));
		if (ret == SOCKET_ERROR) {
			goto error;
		}
	}
	else /* SOCK_CONN_CLIENT */ {
		assert(ip_address != NULL);
		if (isalpha(ip_address[0])) {
			hostp = gethostbyname(ip_address);
		}
		else {
			hostaddr = inet_addr(ip_address);
			hostp = gethostbyaddr((char *) &hostaddr, sizeof(hostaddr), self->local.sin_family);
		}
		if (hostp == NULL) {
			goto error;
		}

		/* reset the addr stuff */
		memset(&self->local, 0, sizeof(self->local));
		memcpy(&self->local.sin_addr, hostp->h_addr, hostp->h_length);
		self->local.sin_family = hostp->h_addrtype;
		self->local.sin_port = htons(port);

		switch (type) {
			case SOCK_TYPE_TCP:
				self->sock = socket(self->local.sin_family, SOCK_STREAM, 0);
				break;
			case SOCK_TYPE_UDP:
				self->sock = socket(self->local.sin_family, SOCK_DGRAM, 0);
				/* forward prepare for the remote connection */
				self->remotes = calloc(1, sizeof(struct sockaddr_in));
				assert(self->remote_socks);
				break;
			default:
				goto error;
		}

		if (self->sock == SOCKET_ERROR) {
			goto error;
		}

		self->num_connections = 1;

	}

	return self;

error:
	if (self == NULL) {
		return NULL;
	}
	if (self->ip_address != NULL) {
		free(self->ip_address);
	}
	if (self->remotes != NULL) {
		free(self.remotes);
	}
	WSACleanup();
	free(self);
	return NULL;
}

void sock_del(struct sock *self)
{
	int i;
	assert(self);
	closesocket(self->sock);
	WSACleanup();
	if (self->ip_address) {
		free(self->ip_address);
	}
	if (self->remotes != NULL) {
		free(self.remotes);
	}
	if (self->remote_socks != NULL) {
		free(self->remote_socks);
	}
	if (self->buf != NULL) {
		for (i = 0; i < self->num_connections; i++) {
			if (self->buf[i] != NULL) {
				free(self->buf[i]);
			}
		}
		free(self->buf);
	}
	free(self);
}

/**
 * 1 on success
 * 0 on error
 */
int sock_server_init(struct sock *self, int num_connections)
{
	int ret;
	int i;
	int success = 1;
	assert(self);

	self->num_connections = num_connections;

	if (self->type == SOCK_TYPE_TCP) {

		self->remote_socks = calloc(num_connections, sizeof(SOCKET));
		assert(self->remote_socks);

		self->remotes = calloc(num_connections, sizeof(struct sockaddr_in));
		assert(self->remotes);

		ret = listen(self->sock, num_connections);
		if (ret == SOCKET_ERROR) {
			free(self->remote_socks);
			free(self->remotes);
			return;
		}

		for (i = 0; i < num_connections; i++) {
			self->remote_socks[i] = accept(self->sock, (struct sockaddr *) &self->remotes[i], &self->addrlen);
			if (self->remote_socks[i] == INVALID_SOCKET) {
				fprintf(stderr, "sock_server_listen: accept() error %d\n", WSAGetLastError());
				success = 0;
			}
		}
	}

	self->buf = calloc(num_connections, sizeof(unsigned char *));
	assert(self->buf);
	for (i = 0; i < num_connections; i++) {
		self->buf[i] = calloc(BUFFER_MAX, sizeof(unsigned char));
		assert(self->buf[i]);
	}

	return success;
}

/**
 * 1 on success
 * 0 on error
 */
int sock_client_init(struct sock *self)
{
	int ret;
	assert(self);

	if (self->type == SOCK_TYPE_TCP) {
		ret = connect(self->sock, (struct sockaddr *) &self->local, sizeof(self->local));
		if (ret == SOCKET_ERROR) {
			return 0;
		}
	}

	self->buf = calloc(1, sizeof(unsigned char *));
	assert(self->buf);
	self->buf[0] = calloc(BUFFER_MAX, sizeof(unsigned char));
	assert(self->buffer[0]);

	return 1;
}

void sock_recv(struct sock *self)
{
	int ret;
	int i;
	assert(self);

	for (i = 0; i < self->num_connections; i++) {
		memset(&self->buf[i], 0, BUFFER_MAX);
		if (self->type == SOCK_TYPE_TCP) {
			ret = recv(self->remote_socks[i], self->buf[i], BUFFER_MAX, 0);
		}
		else /* UDP */ {
			ret = recvfrom(self->sock, self->buf[i], BUFFER_MAX, 0, (struct sockaddr *) &self->remotes[0], &self->addrlen);
		}

		if (ret == SOCKET_ERROR) {
			/* error handling? */
			continue;
		}
	}
}

/**
 * Fancy usage of n.
 * 
 * if n == 0:
 *     Send to all
 * if n == positive
 *     Send to that one
 * if n == negative
 *     Send to all but the positive of it
 */
void sock_send(struct sock *self, int n, unsigned char *buf, int len)
{
	int ret;
	int i;
	int start, end, skip;
	assert(self);

	if (n == 0) {
		start = 0;
		end = self->num_connections;
		skip = -1;
	}
	else if (n > 0) {
		start = n;
		end = n + 1;
		skip = -1;
	}
	else /* n < 0 */ {
		start = 0;
		end = self->num_connections;
		skip = n;
	}

	for (i = start; i < end; i++) {
		if (i == skip) {
			continue;
		}

		if (self->type == SOCK_TYPE_TCP) {
			ret = send(self->remote_socks[i], buf, len, 0);
		}
		else /* UDP */ {
			ret = sendto(self->sock, buf, len, 0, (struct sockaddr *) &self->remotes[0], self->addrlen)
		}

		if (ret == SOCKET_ERROR) {
			continue;
		}
	}
}

#endif /* _WIN32 */
