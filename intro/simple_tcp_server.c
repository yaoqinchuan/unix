#include "unp.h"
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
    bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ); // LISTENQ等待连接队列的最大长度
    for ( ; ; ) {
        connnfd = accept(listenfd, (SA *)NULL, NULL); // accept()是在一个套接口接受的一个连接,从s的等待连接队列中抽取第一个连接，创建一个与s同类的新的套接口并返回句柄
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connnfd, buff, strlen(buff));
        close(connnfd);
    }
    
}