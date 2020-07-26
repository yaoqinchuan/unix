#include <unp.h>

typedef void (*func)(void);
#define MAX_BUF_SIZE 2048
enum
{
    HOSTBYNAME,
    HOSTBYADDR,
    SERVBYNAME,
    SERVBYPORT,
    GETADDRINFO
} func_index;

typedef struct
{
    int index;
    func fun;
    char *help_str;
} name_ip_node;

// test function gethostbyname
void test_gethostbyname(void)
{
    char buf[MAX_BUF_SIZE] = {0};
    struct hostent *host_info = NULL;
    do
    {
        printf("Please input ip name:");
        scanf("%s", buf);
        host_info = gethostbyname(buf);
        if (host_info == NULL)
        {
            printf("\r\nInput is invalid,try again.");
        }
    } while (host_info == NULL);
    printf(" The offical name of host is:%s.", host_info->h_name);
    char **alias_point = host_info->h_aliases;
    while (*alias_point != NULL)
    {
        printf("\r\n The alias name of host is:%s.", *alias_point);
        alias_point++;
    }
    printf("\r\n The address type is ipv%d.", (host_info->h_addrtype == AF_INET ? 4 : 6));
    char **IPV4_point = host_info->h_addr_list;
    while (*IPV4_point != NULL)
    {
        printf("\r\n The address host is:%s.", inet_ntop(host_info->h_addrtype, *IPV4_point, buf, sizeof(buf)));
        IPV4_point++;
    }
    free(host_info);
}
void test_gethostbyaddr(void)
{
    char buf[MAX_BUF_SIZE] = {0};
    struct sockaddr_in ip = {0};
    struct hostent *host_info = NULL;
    while (1)
    {
        printf("Please input ip:");
        scanf("%s", buf);
        if (inet_pton(AF_INET, buf, &(ip.sin_addr)) < 0)
        {
            printf("\r\nTransform ip failed");
            return;
        }
        //this is sin_addr
        host_info = gethostbyaddr(&ip.sin_addr, sizeof(ip), AF_INET);
        if (host_info == NULL)
        {
            printf("\r\nInput is invalid,try again.");
        }
        else
        {
            break;
        }
    }
    printf(" The offical name of host is:%s.", host_info->h_name);
    char **alias_point = host_info->h_aliases;
    while (*alias_point != NULL)
    {
        printf("\r\n The alias name of host is:%s.", *alias_point);
        alias_point++;
    }
    printf("\r\n The address type is ipv%d.", (host_info->h_addrtype == AF_INET ? 4 : 6));
    char **IPV4_point = host_info->h_addr_list;
    while (*IPV4_point != NULL)
    {
        printf("\r\n The address host is:%s.", inet_ntop(host_info->h_addrtype, *IPV4_point, buf, sizeof(buf)));
        IPV4_point++;
    }
    printf("\r\nend.");
}
void test_getservbyname(void)
{
    printf("Please input server and protocal example:telnet tcp:\r\n");
    char server[MAX_BUF_SIZE] = {0};
    char proto[MAX_BUF_SIZE] = {0};
    struct servent *ser = NULL;
    while (1)
    {
        scanf("%s %s", server, proto);
        if ((strlen(server) != 0) && (strlen(proto) != 0))
        {

            if (strcmp(proto, "NULL") == 0)
            {
                ser = getservbyname(server, NULL);
            }
            else
            {
                ser = getservbyname(server, proto);
            }
            if (ser != NULL)
            {
                break;
            }
        }
        printf("Invalid input parameters,please input server and protocal again:");
    }
    printf(" \r\nServer offical name is: %s", ser->s_name);
    char **alias = ser->s_aliases;
    while (*alias != NULL)
    {
        printf(" \r\nServer alias is: %s", *alias);
        alias++;
    }
    printf(" \r\nServer port is: %d", ser->s_port);
    printf(" \r\nServer protocol is: %s\r\n", ser->s_proto);
}

static void test_getservbyport(void)
{
    printf("Please input port and protocal example:990 tcp:\r\n");
    int port = 0;
    char proto[MAX_BUF_SIZE] = {0};
    struct servent *ser = NULL;
    while (1)
    {
        scanf("%d %s", &port, proto);
        if (strlen(proto) != 0)
        {

            if (strcmp(proto, "NULL") == 0)
            {
                ser = getservbyport(htons(port), NULL);
            }
            else
            {
                ser = getservbyport(htons(port), proto);
            }
            if (ser != NULL)
            {
                break;
            }
        }
        printf("Invalid input parameters,please input port and protocal again:");
    }
    printf(" \r\nServer offical name is: %s", ser->s_name);
    char **alias = ser->s_aliases;
    while (*alias != NULL)
    {
        printf(" \r\nServer alias is: %s", *alias);
        alias++;
    }
    printf(" \r\nServer port is: %d", ser->s_port);
    printf(" \r\nServer protocol is: %s\r\n", ser->s_proto);
}

static void test_getaddrinfo(void)
{
    printf("Please input hostname and server example:localhost ftp:\r\n");
    char hostname[MAX_BUF_SIZE] = {0};
    char server[MAX_BUF_SIZE] = {0};
    struct addrinfo *ad_info = NULL;
    while (1)
    {
        scanf("%s %s", hostname, server);
        if ((strlen(server) != 0) && (strlen(hostname) != 0))
        {
            int ret = getaddrinfo(hostname, server, NULL, &ad_info);
            if (ret == 0)
            {
                break;
            }
        }
        printf("Invalid input parameters,please input port and protocal again:");
    }
    do {
        char ip_buf[MAX_BUF_SIZE] ={0};
        struct sockaddr_in *ip_in = NULL;
        printf("Host canonnaem  is: %s", ad_info->ai_canonname);
        printf(" \r\nHost ip version is: ipv%d", (ad_info->ai_addr->sa_family == AF_INET ? 4:6));
        inet_ntop(ad_info->ai_addr->sa_family, &(((struct sockaddr_in *)(ad_info->ai_addr))->sin_addr), ip_buf, sizeof(ip_buf));
        printf(" \r\nHost ip is: %s", ip_buf);
        printf(" \r\nServer port is: %d", ((struct sockaddr_in *)(ad_info->ai_addr))->sin_port);
        ad_info = ad_info->ai_next;
        printf("\r\n");
    }while (ad_info != NULL);
}

name_ip_node func_tbl[] = {
    {HOSTBYNAME, test_gethostbyname, "Test gethostbyname."},
    {HOSTBYADDR, test_gethostbyaddr, "Test gethostbyaddr."},
    {SERVBYNAME, test_getservbyname, "Test getserbbyname."},
    {SERVBYPORT, test_getservbyport, "Test getservbyport."},
    {GETADDRINFO, test_getaddrinfo, "Test getaddrinfo."}};

static void init_help_message(int *select_ans)
{
    printf("\r\nPlease select test function:");
    for (int i = 0; i < sizeof(func_tbl) / sizeof(name_ip_node); i++)
    {
        printf("\r\n   %d : %s", func_tbl[i].index, func_tbl[i].help_str);
    }
    printf("\r\nPlease select:");
    while (1)
    {
        scanf("%d", select_ans);
        if (*select_ans >= sizeof(func_tbl) / sizeof(name_ip_node))
        {
            printf("\r\nInput select is invalid,please select again!");
        }
        else
        {
            break;
        }
        printf("\r\nPlease select:");
    }
}
int main()
{
    int select_ans = 0;
    init_help_message(&select_ans);
    func_tbl[select_ans].fun();
    return 0;
}