CFLAGS += -g
rmfiles = tcp_select_client tcp_select_server tcp_poll_server tcp_epoll_server
test:
	gcc $(CFLAGS) tcp_select_client.c -o tcp_select_client
	gcc $(CFLAGS) tcp_select_server.c -o tcp_select_server
	gcc $(CFLAGS) tcp_poll_server.c -o tcp_poll_server
	gcc $(CFLAGS) tcp_epoll_server.c -o tcp_epoll_server
.PTONY: clean
clean :
	rm -rf $(rmfiles)
