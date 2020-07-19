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
    fd_set fdset, rset;
    struct timeval timeout = {0};
    int fd_arr[MAX_CONNECT_NUM] = { 0 };
    for (int i = 0; i < MAX_CONNECT_NUM; i++)
    {
        fd_arr[i] = -1;
    }
    timeout.tv_sec = 30;
    timeout.tv_usec = 0;
    int maxi = 0;
    init_tcp_server(&sockfd);
    FD_ZERO(&fdset);
    int max_fd = sockfd;
    fd_arr[0] = sockfd;
    FD_SET(sockfd, &fdset);
    while(1) {
        rset =fdset;
        int connfd = 0;
        int nready = select(max_fd + 1, &rset, NULL, NULL, &timeout);

        //  new connect is comming
        if (FD_ISSET(sockfd, &rset)) {
            connfd = accept(sockfd, (SA *)NULL, NULL);
            if (connfd < 0) {
                perror("accept");
                continue;
            }
             // set fd_set
            for (int i = 0; i < MAX_CONNECT_NUM; i++) {
                if (fd_arr[i] < 0) {
                    fd_arr[i] = connfd;
                    maxi = (i > maxi ? i:maxi);
                    max_fd = (max_fd > connfd ? max_fd:connfd);
                    break;
                }
            }
            // set all fd
            for (int i = 0; i <= maxi; i++) {
                if (fd_arr[i] < 0) {
                    continue;
                }
                FD_SET(fd_arr[i], &fdset);
            } 
        }
        // read message
        for (int i = 0; i <= maxi; i++)
        {
            if (fd_arr[i] < 0) {
                continue;
            }
            char show_buf[2048] = {0};
            int num_byte = 0;
            if (FD_ISSET(fd_arr[i], &rset)) {
                if ((num_byte = read(fd_arr[i], show_buf, 2048)) == 0) {
                    close(fd_arr[i]);
                    FD_CLR(fd_arr[i], &fdset);
                    fd_arr[i] = -1;

                } else {
                    write(fd_arr[i],show_buf, num_byte);
                }
                if (--nready <= 0) {
                    break;
                }
            }
        }
    }
}