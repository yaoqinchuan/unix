#include <unp.h>

#define OK 0
#define ERR 1
#define SERVER_IP_LEN 32
#define MAX_BUFF_LEN 2048
#define MAX_SEN_LEN 2000 //return ip should be considered
#define MAX_RCV_LEN 2048
int init_socket(int *socket_fd)
{
    char server_ip_str[] = "127.0.0.1";
    struct sockaddr_in server_ip = {0};
    if (inet_pton(AF_INET, (const void *)&server_ip.sin_addr.s_addr, server_ip_str) < 0)
    {
        perror("Transform ip failed.\r\n");
        return ERR;
    };
    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(SERV_PORT);
    if ((*socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Create socket failed.\r\n");
        return ERR;
    }
    if (connect(*socket_fd, (SA *)&server_ip, sizeof(server_ip)) < 0)
    {
        perror("Connect failed.\r\n");
        return ERR;
    };
    return OK;
}
int main()
{
    int sock_fd = 0;
    if (init_socket(&sock_fd) != OK)
    {
        return 0;
    }
    while (1)
    {
        char send_buf[MAX_SEN_LEN] = {0};
        printf("Please input message send to server:\r\n");
        fgets(send_buf, MAX_BUFF_LEN, stdin);
        int send_count = sendto(sock_fd, send_buf, MAX_BUFF_LEN, 0, (SA *)NULL, 0);
        if (send_count < 0)
        {
            break;
        }
        char rcv_buf[MAX_RCV_LEN] = {0};
        int rcv_count = recvfrom(sock_fd, rcv_buf, MAX_RCV_LEN, 0, (SA *)NULL, NULL);
        if (rcv_count > 0)
        {
            printf("Receive echo message from server:\r\n");
            printf("%s", rcv_buf);
        }
        else
        {
            perror("Receive echo message failed.\r\n");
        }
    }
}