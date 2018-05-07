/*
** EPITECH PROJECT, 2018
** cnet
** File description:
** tests / selector.c
*/

#include "cnet.h"

tcp_socket_t *clients[5];
size_t n = 0;

int main(void)
{
	bzero(clients, sizeof(tcp_socket_t *) * 5);

	socket_selector_t *selector = socket_selector_create();
	tcp_listener_t *listener = tcp_listener_create();

	if (tcp_listener_listen(listener, 4242, IP_ANY) != SOCKET_DONE)
		exit(1);
	socket_selector_add_tcp_listener(selector, listener);

	while (1) {
		if (socket_selector_wait(selector, 0)) {
			if (socket_selector_is_tcp_listener_ready(selector, listener)) {
				tcp_socket_t *client = tcp_socket_bare();
				if (tcp_listener_accept(listener, &client) == SOCKET_DONE) {
					socket_selector_add_tcp_socket(selector, client);
					printf("NEW: %s\n", ip_address_to_string(tcp_socket_get_remote_address(client)));
					clients[n++] = client;
				}
			}
			else {
				for (size_t i = 0; i < n; ++i) {
					if (socket_selector_is_tcp_socket_ready(selector, clients[i])) {
						char buff[128]; bzero(buff, 128);
						size_t len = 0;
						if (tcp_socket_receive(clients[i], buff, 128, &len) != SOCKET_DONE) {
							fprintf(stderr, "tcp_socket_receive() failed\n");
							continue;
						}
						printf("GET: [%s]\n", buff);
						for (size_t j = 0; j < n; ++j) {
							if (j == i) continue;
							tcp_socket_send(clients[j], buff, len, NULL);
						}
					}
				}
			}
		}
	}

	tcp_listener_destroy(listener);
	socket_selector_destroy(selector);
	return (0);
}