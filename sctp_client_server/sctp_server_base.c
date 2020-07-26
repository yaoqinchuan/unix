#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
//#include "common.h"

#define MAX_BUFFER 2048
#define MY_PORT_NUM 19000

static void init_sctp_socket(int *socket_fd, struct sockaddr_in *client_ip, struct sctp_event_subscribe *evnts)
{

    client_ip->sin_addr.s_addr = htonl(INADDR_ANY);
    client_ip->sin_family = AF_INET;
    client_ip->sin_port = htons(MY_PORT_NUM);
    evnts->sctp_data_io_event = 1;
    *socket_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (*socket_fd < 0)
    {
        perror("Socket create failed.");
    }
    if (bind(*socket_fd, (const struct sockaddr *)client_ip, sizeof(*client_ip)) < 0)
    {
        perror("Bind failed.");
    }
    if (setsockopt(*socket_fd, IPPROTO_SCTP, SCTP_EVENTS, evnts, sizeof(*evnts)) < 0)
    {
        perror("Set socketopt failed.");
    }
    if (listen(*socket_fd, 1024) < 0)
    {
        perror("Listen failed.");
    }
}
int main(int argc, char **arg)
{
    int socketfd = 0;
    int stream_increment = 0;
    int msg_flag = 0;
    struct sockaddr_in client_ip = {0};
    struct sockaddr_in server_ip = {0};
    struct sctp_event_subscribe evnts = {0};
    struct sctp_sndrcvinfo sinfo = {0};
    if (argc == 2)
    {
        stream_increment = atoi(arg[1]);
    }
    init_sctp_socket(&socketfd, &server_ip, &evnts);
    int from_len = sizeof(client_ip);
    while (1)
    {
        char readbuf[MAX_BUFFER] = {0};
        if (stream_increment != 0)
        {
            sinfo.sinfo_stream++;
            if (sinfo.sinfo_stream >= 10)
            {
                sinfo.sinfo_stream = 0;
            }
        }
        int read_sz = sctp_recvmsg(socketfd, readbuf, MAX_BUFFER, (struct sockaddr *)&client_ip, (socklen_t *)&from_len, &sinfo, &msg_flag);

        if (read_sz == 0)
        {
            continue;
        }
        if (sctp_sendmsg(socketfd, readbuf, read_sz, (struct sockaddr *)&client_ip, sizeof(client_ip), sinfo.sinfo_ppid, sinfo.sinfo_flags, sinfo.sinfo_stream, 0, 0) < 0)
        {
            perror("Send message failed.");
        }
    }
}