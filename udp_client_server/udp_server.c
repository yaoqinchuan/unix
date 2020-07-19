#include <unp.h>
#include <stdio.h>
#define OK 0
#define ERR 1
#define MAX_BUFF_LEN 2048
#define MAX_IPV4_LEN 32
int init_socket(int *socket_fd)
{
    struct sockaddr_in ip = {0};
    ip.sin_family = AF_INET;
    ip.sin_port = htons(SERV_PORT);
    ip.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((*socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket create failed:");
        return ERR;
    }
    if (bind(*socket_fd, (SA *)&ip, sizeof(ip)) < 0)
    {
        perror("Bind failed:");
        return ERR;
    }
    return OK;
}
// please confirm the buf lenth is enough by yourself
void attach_client_ip_2_buf(char *buf, struct sockaddr_in client_ip)
{
    char info_str[] = "message send by: ";
    struct in_addr ip_n = client_ip.sin_addr;
    char ip_buf[MAX_IPV4_LEN] = {0};
    if (inet_ntop(AF_INET, (const void *)&ip_n, ip_buf, MAX_IPV4_LEN) < 0)
    {
        perror("Get address failed");
    }
    strcat(info_str, ip_buf);
    strcat(info_str, "\r\n");
    strcat(buf, info_str);
}
void udp_message_proc(int socket_fd)
{
    struct sockaddr_in cli_ip = {0};
    char buf[MAX_BUFF_LEN] = {0};
    socklen_t addr_len = MAX_IPV4_LEN;
    int data_num = recvfrom(socket_fd, buf, MAX_BUFF_LEN, 0, (SA *)&cli_ip, &addr_len);
    if (data_num != 0)
    {
        attach_client_ip_2_buf(buf, cli_ip);
        if (sendto(socket_fd, buf, MAX_BUFF_LEN, 0, (SA *)&cli_ip, sizeof(cli_ip)) < 0)
        {
            perror("echo message failed");
        };
    }
}
int main()
{
    int socket_fd = 0;
    if (init_socket(&socket_fd) != OK)
    {
        return OK;
    }
    while (1)
    {
        udp_message_proc(socket_fd);
    }
}