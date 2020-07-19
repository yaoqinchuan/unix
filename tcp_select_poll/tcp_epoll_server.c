#include <unp.h>
#include <stdio.h>
#include <sys/epoll.h>

#define ERR -1
#define OK 0
#define MAX_CONNECT_COUNT 1000
#define PORT SERV_PORT
#define BUF_MAX_LEN 2048
int init_socket(int *socketfd)
{
    struct sockaddr_in ipaddr = {0};
    ipaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(PORT);
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*socketfd < 0)
    {
        perror("socket create failed:");
        return ERR;
    }
    if (bind(*socketfd, (SA *)&ipaddr, sizeof(ipaddr)) < 0)
    {
        perror("bind failed:");
        return ERR;
    };
    if (listen(*socketfd, MAX_CONNECT_COUNT) < 0)
    {
        perror("connect failed:");
        return ERR;
    };
    return OK;
}

void fd_process(int connectfd)
{
    char buf[BUF_MAX_LEN] = {0};
    int read_len = 0;
    while ((read_len = read(connectfd, buf, BUF_MAX_LEN)) > 0)
    {
        write(connectfd, buf, read_len);
    }
    close(connectfd); // only can tranform 1 time;
}

int main()
{
    int sockfd = 0;
    int epoll_fd = 0;
    int timeout = 5;
    struct epoll_event event, events[MAX_CONNECT_COUNT];
    if (init_socket(&sockfd) != OK)
    {
        return 0;
    }
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sockfd;
    epoll_fd = epoll_create(MAX_CONNECT_COUNT);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) < 0)
    {
        perror("epoll ctl failed:");
    };
    while (1)
    {
        int count = epoll_wait(epoll_fd, events, MAX_CONNECT_COUNT, timeout);
        for (int i = 0; i < count; i++)
        {
            if (events[i].data.fd == sockfd)
            {
                int connfd = accept(sockfd, (SA *)NULL, NULL);
                if (connfd < 0)
                {
                    continue;
                }
                event.data.fd = connfd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event) < 0)
                {
                    perror("add epoll failed:");
                };
            }
            else
            {
                fd_process(events[i].data.fd);
            }
        }
    }
    close(sockfd);
    close(epoll_fd);
    return 0;
}