/*
** EPITECH PROJECT, 2018
** cnet
** File description:
** socket_selector / remove.c
*/

#include "cnet/socket_selector.h"

static void socket_selector_remove_fd(socket_selector_t *selector, int fd)
{
	if (fd == -1 || fd >= FD_SETSIZE)
		return;
	FD_CLR(fd, &selector->all_sockets);
	FD_CLR(fd, &selector->sockets_ready);
}

void socket_selector_remove_socket(socket_selector_t *selector,
	socket_t *socket)
{
	socket_selector_remove_fd(selector, socket->handle);
}

void socket_selector_remove_tcp_socket(socket_selector_t *selector,
	tcp_socket_t *socket)
{
	socket_selector_remove_fd(selector, socket->handle);
}

void socket_selector_remove_tcp_listener(socket_selector_t *selector,
	tcp_listener_t *listener)
{
	socket_selector_remove_fd(selector, listener->handle);
}