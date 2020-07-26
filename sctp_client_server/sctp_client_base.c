#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#define SERVER_PORT_NUM 19000
#define MAX_BUF_LEN 2048
#define OK 0
#define ERR 1
static int init_socket(int *socket_fd, struct sockaddr_in *server_ip, struct sctp_event_subscribe *evnts)
{
    char server_ip_str[] = "127.0.0.1";
    if (inet_pton(AF_INET, server_ip_str, (void *)&(server_ip->sin_addr)) < 0)
    {
        perror("IP transform failed.");
        return ERR;
    }
    server_ip->sin_family = AF_INET;
    server_ip->sin_port = htons(SERVER_PORT_NUM);
    if ((*socket_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0)
    {
        perror("Socket create failed.");
        return ERR;
    }
    return OK;
}

int main(int argc, char **arg)
{
    if (argc != 2)
    {
        printf("Please input start stream id!");
    }
    int start_stream_id = atoi(arg[1]);
    int sock_fd = 0;
    struct sockaddr_in server_ip = {0};
    struct sctp_event_subscribe evnts = {0};
    struct sctp_sndrcvinfo sinfo = {0};
    struct sockaddr_in rcv_peer_addr = {0};
    struct sctp_sndrcvinfo rcv_sinfo = {0};
    if (init_socket(&sock_fd, &server_ip, &evnts) != OK)
    {
        return OK;
    }
    evnts.sctp_data_io_event = 1;
    if (setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts)) < 0)
    {
        perror("Setsockopt failed.");
        return OK;
    }
    sinfo.sinfo_stream = start_stream_id;
    printf("Send stream num is : %d\r\n", start_stream_id);
    while (1)
    {
        char buf[MAX_BUF_LEN] = {0};
        if (fgets(buf, MAX_BUF_LEN, stdin) == NULL)
        {
            continue;
        }
        if (sinfo.sinfo_stream >= 10)
        {
            sinfo.sinfo_stream = 1;
        }
        printf("Message send: %s\r\n", buf);
        printf("Sock fd is: %d\r\n", sock_fd);
        printf("server_ip port is: %d\r\n", server_ip.sin_port);
        if (sctp_sendmsg(sock_fd, buf, strlen(buf), (struct sockaddr *)&server_ip, sizeof(server_ip), 0, 0, sinfo.sinfo_stream, 0, 0) < 0)
        {
            perror("Message send failed");
            continue;
        }
        printf("Message send success.\r\n");
        int flags = 0;
        int len = sizeof(rcv_peer_addr);
        char rcv_buf[MAX_BUF_LEN] = {0};
        sinfo.sinfo_stream++;
        int rcv_num = sctp_recvmsg(sock_fd, rcv_buf, MAX_BUF_LEN, (struct sockaddr *)&rcv_peer_addr, &len, &rcv_sinfo, &flags);
        if (rcv_num <= 0)
        {
            continue;
        }
        printf("Message rcv:\r\n");
        printf("  rcv context: %s\r\n", rcv_buf);
        printf("  rcv stream num: %d\r\n", rcv_sinfo.sinfo_stream);
        printf("  rcv seq num: %d\r\n", rcv_sinfo.sinfo_ssn);
        printf("  rcv assoc num:%d\r\n", (int)rcv_sinfo.sinfo_assoc_id);
    }
    close(sock_fd);
}