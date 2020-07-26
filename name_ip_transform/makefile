CFLAGS += -g
rmfiles = name_ip_trans 
test:
	gcc $(CFLAGS) name_ip_trans.c -lsctp -o name_ip_trans
.PTONY: clean
clean :
	rm -rf $(rmfiles)
