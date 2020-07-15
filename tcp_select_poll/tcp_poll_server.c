#include <unp.h>
#include <stdlib.h>
void init_tcp_server(int *sockfd)
{
    struct sockaddr_in ipadd = {0};
    ipadd.sin_family = AF_INET;
    ipadd.sin_port = htons(SERV_PORT);
    ipadd.sin_addr.s_addr = htonl(INADDR_ANY);

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        perror("socket create failed");
        return;
    }
    if(bind(*sockfd, (SA *)&ipadd, sizeof(ipadd)) < 0) {
        perror("bind failed");
    }
    if (listen(*sockfd, LISTENQ) < 0) {
        perror("listen");
    }
    return;
}
#define MAX_CONNECT_NUM 1024
int main()
{
    int sockfd , single_fd;
    struct pollfd client[MAX_CONNECT_NUM] = {0};
    struct timeval timeout = {0};
    for (int i = 0; i < MAX_CONNECT_NUM; i++)
    {
        client[i].fd = -1;
    }
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    init_tcp_server(&sockfd);
    client[0].fd = sockfd;
    client[0].events = POLLRDNORM;
    int maxi =0;
    while(1) {
        int connfd = 0;
        int nready = poll(client, maxi + 1, INFTIM);

        if(client[0].revents & POLLRDNORM) {
            connfd = accept(sockfd, (SA *)NULL, NULL);
            for (int i = 0; i < MAX_CONNECT_NUM; i++)
            {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    client[i].events = POLLRDNORM;
                }
                maxi = (maxi > i ? maxi:i);
            }
            
        }

        for (int i = 1; i <= maxi; i++)
        {
            if(client[i].fd < 0) {
                continue;
            }
            char show_buf[2048] = {0};
            int num_byte = 0;
            if (client[i].revents & POLLRDNORM) {
                if ((num_byte = read(client[i].fd, show_buf, 2048)) == 0) {
                    close(client[i].fd);
                    client[i].fd = -1;

                } else {
                    write(client[i].fd,show_buf, num_byte);
                }
                if (--nready <= 0) {
                    break;
                }
            }
        }
    }
}