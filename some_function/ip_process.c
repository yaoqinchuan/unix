#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
/*
inet_aton,inet_ntoa and inet_aton be used by ipv4
*/
struct in_addr test_inet_aton(const char * ipaddr)
{
    struct in_addr addr_n = {0};
    int ret = inet_aton(ipaddr, &addr_n);
    if (ret == 0) {
        printf("invalid input ipv4 address!\r\n");
        return addr_n;
    }
    printf("test_inet_aton:ip address %s, the net sequence is %d.\r\n", ipaddr, addr_n.s_addr);
    return addr_n;
}

void test_inet_ntoa(struct in_addr addr_n)
{
    char *addr_s = NULL;
    addr_s = inet_ntoa(addr_n);
    if (addr_s == NULL) {
        printf("invalid ipv4 net sequence!\r\n");
    }
    printf("test_inet_ntoa:ip net sequence is %d, the ip address is %s.\r\n", addr_n.s_addr, addr_s);
    return;
}
struct in_addr test_inet_addr(const char * ipaddr)
{
    struct in_addr addr_n = {0};
    int ret = inet_aton(ipaddr, &addr_n);
    if (ret == 0) {
        printf("invalid input ipv4 address!\r\n");
        return addr_n;
    }
    printf("test_inet_addr:ip address %s, the net sequence is %d.\r\n", ipaddr, addr_n.s_addr);
    return addr_n;
}

void test_ipv4_function(char *input)
{
    (void)test_inet_addr(input);
    struct in_addr ip_n = test_inet_aton(input);
    test_inet_ntoa(ip_n);
    getchar();
}

/*
inet_pton and inet_ntop can be used by ipv4 and ipv6
*/

void test_inet_pton(int family, const char *ip_s, struct in6_addr *ip_n)
{
    int ret = inet_pton(family, ip_s, (void *)ip_n);
    if (ret != 1) {
        printf("test_inet_pton:invalid ip address!\r\n");
        return;
    }
    printf("test_inet_pton:ip address %s.\r\n", ip_s);
    return;
}

void test_inet_ntop(int family, struct in6_addr *ip_n)
{
    char *ip_s = (char *)malloc((INET6_ADDRSTRLEN+1) * sizeof(char));
    (void)memset(ip_s, 0, (INET6_ADDRSTRLEN +1) * sizeof(char));
    const char *ip_s_r = NULL;
    if (family == AF_INET) {
        ip_s_r = inet_ntop(family, (const void *)ip_n, ip_s, INET_ADDRSTRLEN);
    } else {
        ip_s_r = inet_ntop(family, (const void *)ip_n, ip_s, INET6_ADDRSTRLEN);
    }
    
    if (ip_s_r == NULL) {
        printf("test_inet_ntop:invalid ip address!\r\n");
        return;
    }
    printf("test_inet_ntop:ip address %s.\r\n", ip_s_r);
    return;
}

void test_ipv6_function(char *input, int family)
{
    struct in6_addr ip_n = {0};
    test_inet_pton(family, input, &ip_n);
    test_inet_ntop(family, &ip_n);
    return;

}
int main(int argc, char **argv)
{ 
    if (argc < 2) {
        printf("please input ip address!");
        return 0;
    }
    char *input = argv[1];
    char *family = argv[2];
    int family_n = atoi(family);
    test_ipv6_function(input, family_n);
    return 0;
}