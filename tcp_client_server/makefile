CFLAGS += -g
rmfiles = tcp_relect_client tcp_relect_server 
test:
	gcc $(CFLAGS) tcp_relect_client.c -o tcp_relect_client
	gcc $(CFLAGS) tcp_relect_server.c -o tcp_relect_server
	
.PTONY: clean
clean :
	rm -rf $(rmfiles)
