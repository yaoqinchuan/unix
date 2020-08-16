CFLAGS += -g
rmfiles = name_ip_trans practice_time_get_client
test:
	gcc $(CFLAGS) name_ip_trans.c -lsctp -o name_ip_trans
	gcc $(CFLAGS) practice_time_get_client.c -lsctp -o practice_time_get_client
.PTONY: clean
clean :
	rm -rf $(rmfiles)
