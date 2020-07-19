CFLAGS += -g
files = udp_client udp_server udp_client_connect server_tcp_udp
opt: 
	gcc $(CFLAGS) udp_client.c -o udp_client
	gcc $(CFLAGS) udp_server.c -o udp_server
	gcc $(CFLAGS) udp_client_connect.c -o udp_client_connect
	gcc $(CFLAGS) server_tcp_udp.c -o server_tcp_udp
.PTONY:clean
clean:
	rm -rf $(files)