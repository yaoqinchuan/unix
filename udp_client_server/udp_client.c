#include <unp.h>
#include <stdio.h>
#define OK 0
#define ERR 1
#define MAX_BUFF_LEN 2048
#define MAX_IPV4_LEN 32
#define MAX_SEN_LEN 2000 //return ip should be considered
#define MAX_RCV_LEN 2048 
int init_socket(int *socket_fd, struct sockaddr_in *ip)
{
    char server_ip[] = "127.0.0.1";
    ip->sin_family = AF_INET;
    ip->sin_port = htons(SERV_PORT);
    if(inet_ntop(AF_INET,(const void *)&ip->sin_addr.s_addr, server_ip, sizeof(server_ip)) < 0 ) {
        perror("Transform server ip failed.");
    }
    if ((*socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket create failed:");
        return ERR;
    }
    return OK;
}
int main()
{
    int socket_fd = 0;
    struct sockaddr_in ip = { 0 };
    if (init_socket(&socket_fd, &ip) != OK) {
        return OK;
    }
    while (1)
    {
        char send_buf[MAX_SEN_LEN] = {0};
        printf("Please input message send to server:\r\n");
        fgets(send_buf, MAX_BUFF_LEN, stdin);
        int send_count = sendto(socket_fd, send_buf, MAX_BUFF_LEN, 0, (SA *) &ip, sizeof(ip));
        if (send_count < 0) {
            break;
        }
        char rcv_buf[MAX_RCV_LEN] = {0};
        int rcv_count = recvfrom(socket_fd, rcv_buf, MAX_RCV_LEN, 0, (SA *)NULL, NULL);
        if (rcv_count > 0) {
            printf("Receive echo message from server:\r\n");
            printf("%s",rcv_buf);
        }
    }
    
}