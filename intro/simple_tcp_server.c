#include "unp.h"
#include <sys/socket.h>
#include <time.h>
int main()
{
    int listenfd, connnfd;
    struct sockaddr_in serveraddr;
    char buff[MAXLINE];
    time_t ticks;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        printf("Server create socket failed.\r\n");
        return 0;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(13);
    serveraddr.sin_family = AF_INET;
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) {
        printf("bind failed!");
    };
    listen(listenfd, LISTENQ); // LISTENQ等待连接队列的最大长度
    for ( ; ; ) {
        struct sockaddr_in remote_ip_main = {0};
        socklen_t remote_ip_main_len = INET_ADDRSTRLEN;
        char *remote_ip_main_s = (char *)malloc((INET_ADDRSTRLEN + 1) * sizeof(char));
        (void)memset(remote_ip_main_s, 0, (INET_ADDRSTRLEN + 1) * sizeof(char));
        connnfd = accept(listenfd, (SA *)&remote_ip_main, &remote_ip_main_len); // accept()是在一个套接口接受的一个连接,从s的等待连接队列中抽取第一个连接，创建一个与s同类的新的套接口并返回句柄
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connnfd, buff, strlen(buff));
        if (inet_ntop(AF_INET, (void *)&remote_ip_main.sin_addr.s_addr, remote_ip_main_s, INET_ADDRSTRLEN) == NULL)
        {
            printf("Switch ip num to ip string failed.");
            return 0;
        }
        printf("remote ip is %s.\r\n", remote_ip_main_s);
        printf("remote ip version is %s.\r\n", (remote_ip_main.sin_family == AF_INET ? "4" : "6"));
        close(connnfd);
    }
    
}