CFLAGS += -g
rmfiles = sctp_client_base sctp_server_base
test:
	gcc $(CFLAGS) sctp_client_base.c -lsctp -o sctp_client_base
	gcc $(CFLAGS) sctp_server_base.c -lsctp -o sctp_server_base
.PTONY: clean
clean :
	rm -rf $(rmfiles)
