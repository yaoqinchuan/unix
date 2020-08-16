#include <unp.h>
#define MAX_FD 1000
#define MAX_RCV_BUF 1024
#define MAX_IP_LEN 256
#define ERR 1
#define OK 0
int init_tcp(int *socketfd, const char* hostname, const char* service)
{
    int n = 0;
    struct sockaddr_in ip = {0};
    struct addrinfo *add_info = NULL;
    struct addrinfo *sares = NULL;
    struct addrinfo hints = {0};
    struct sockaddr sa = {0};
    hints.ai_family = AF_INET; // 函数返回的是适用于指定主机名和服务名且适合任何协议族的地址
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(hostname, service, &hints, &add_info) != 0) {
        printf("Get address info failed.\r\n");
        return ERR;
    }
    sares = add_info;
    do {
        if ((*socketfd = socket(add_info->ai_family, add_info->ai_socktype, add_info->ai_protocol)) <= 0) {
            perror("Create socket failed.\r\n");
            continue;
        }
        if (connect(*socketfd, (SA *)sares->ai_addr, sares->ai_addrlen) == 0) {
            printf("Connect socket success.\r\n");
            break;
        }
        close(*socketfd);
        *socketfd = -1;
    }while((sares = sares->ai_next) != NULL);

    freeaddrinfo(add_info);
    return OK;
}
int main(int argc , char **arg)
{
    if (argc != 3) {
        printf("Input parameter is invalid, please try again\r\n");
        return OK;
    }
    int socketfd = 0;
    if (init_tcp(&socketfd, arg[1], arg[2]) != OK) {
        return OK;
    }
    struct sockaddr_in peer_add = {0};
    socklen_t len = sizeof(peer_add);
    if (getpeername(socketfd, (SA *)&peer_add, &len) < 0) {
        perror("Get peer name error.\r\n");
        return OK;
    }
    char ip_addr[MAX_IP_LEN]= {0};
    if (inet_ntop(peer_add.sin_family, (void*)&peer_add.sin_addr.s_addr, ip_addr, MAX_IP_LEN) == NULL) {
        perror("Transform IP failed.\r\n");
        return OK; 
    };
    printf("Connect to %s success\r\n", ip_addr);
    char rcv[MAX_RCV_BUF] = {0};
    int n = 0;
    int times = 0;
    while ((n = read(socketfd, rcv, MAX_RCV_BUF)) > 0)
    {
        printf("Receive message%d:\r\n", times);
        printf("%s\r\n", rcv);
        memset(rcv, 0, MAX_RCV_BUF);
    }
    return OK;
}