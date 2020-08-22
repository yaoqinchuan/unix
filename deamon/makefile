CFLAGS += -g
rmfiles = deamon_server
test:
	gcc $(CFLAGS) deamon_server.c -lsctp -o deamon_server
.PTONY: clean
clean :
	rm -rf $(rmfiles)
