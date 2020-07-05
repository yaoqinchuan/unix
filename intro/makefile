CFLAGS += -g
rmfiles = simple_tcp_client simple_tcp_server simple_tcp_server_sub_process
test:
	gcc $(CFLAGS) simple_tcp_client.c -o simple_tcp_client
	gcc $(CFLAGS) simple_tcp_server.c -o simple_tcp_server
	gcc $(CFLAGS) simple_tcp_server_sub_process.c -o simple_tcp_server_sub_process
.PTONY: clean
clean :
	rm -rf $(rmfiles)
