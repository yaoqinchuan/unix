#include "unp.h"
#include <time.h>
void get_time(char *buf, int len)
{
    time_t ticks = time(NULL);
    snprintf(buf, len, "%.24s\r\n", ctime(&ticks));
    return;
}
int main(int argc, char **arg)
{
    if (argc != 2)
    {
        printf("Please specify total connet count!\r\n");
        return 0;
    }
    int listenfd, connnfd;
    struct sockaddr_in serveraddr;
    char buff[MAXLINE];
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        printf("Server create socket failed.\r\n");
        return 0;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(13);
    serveraddr.sin_family = AF_INET;
    bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ); // LISTENQ等待连接队列的最大长度
    for (int i = 0; i < atoi(arg[1]); i++)
    {                                                 // 限制总的可连接次数，不想主进程一直占用后台
        connnfd = accept(listenfd, (SA *)NULL, NULL); // accept()是在一个套接口接受的一个连接,从s的等待连接队列中抽取第一个连接，创建一个与s同类的新的套接口并返回句柄
        pid_t pid = fork();
        if (pid != 0)
        {
            close(listenfd);
            get_time(buff, MAXLINE);
            write(connnfd, buff, strlen(buff));

            struct sockaddr_in remote_ip = {0};
            struct sockaddr_in local_ip = {0};
            socklen_t remote_ip_len = INET_ADDRSTRLEN; // addrlen在初始化的时候一定要设置为返回的地址的长度
            socklen_t local_ip_len = INET_ADDRSTRLEN;  // addrlen在初始化的时候一定要设置为返回的地址的长度

            if (getpeername(connnfd, (struct sockaddr *)&remote_ip, &remote_ip_len) < 0)
            {
                printf("Get remote ip by socket failed!");
                return 0;
            }
            if (getsockname(connnfd, (struct sockaddr *)&local_ip, &local_ip_len) < 0)
            {
                printf("Get remote ip by socket failed!");
                return 0;
            }

            char *remote_ip_s = (char *)malloc((INET_ADDRSTRLEN + 1) * sizeof(char));
            (void)memset(remote_ip_s, 0, (INET_ADDRSTRLEN + 1) * sizeof(char));
            char *local_ip_s = (char *)malloc((INET_ADDRSTRLEN + 1) * sizeof(char));
            (void)memset(local_ip_s, 0, (INET_ADDRSTRLEN + 1) * sizeof(char));

            if (inet_ntop(AF_INET, (void *)&remote_ip.sin_addr.s_addr, remote_ip_s, INET_ADDRSTRLEN) == NULL)
            {
                printf("Switch ip num to ip string failed.");
                return 0;
            }
            if (inet_ntop(AF_INET, (void *)&local_ip.sin_addr.s_addr, local_ip_s, INET_ADDRSTRLEN) == NULL)
            {
                printf("Switch ip num to ip string failed.");
                return 0;
            }
            printf("Remote ip is %s.\r\n", remote_ip_s);
            printf("Remote ip version %s.\r\n", (remote_ip.sin_family == AF_INET ? "4" : "6"));
            printf("Remote port is %d.\r\n", ntohs(remote_ip.sin_port));

            printf("local ip is %s.\r\n", local_ip_s);
            printf("local ip version %s.\r\n", (local_ip.sin_family == AF_INET ? "4" : "6"));
            printf("local port is %d.\r\n", ntohs(local_ip.sin_port));
            close(connnfd);
            return 0;
        }
        close(connnfd);
    }
    return 0;
}