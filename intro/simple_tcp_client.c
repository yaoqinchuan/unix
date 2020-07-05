#include "unp.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    int socketfd, n;
    int ret = 0;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    if (argc != 2)
    {
        printf("input is not a address!\r\n");
        return 0;
    }
    // AF_INET ip address famliy: 2
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket connet err!\r\n");
        return 0;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("invalid ip format!\r\n");
        return 0;
    }
    if (connect(socketfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error!\r\n");
        return 0;
    }
    if (n < 0)
    {
        printf("read invalid info.\r\n");
    }
    while ((n = read(socketfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;
        printf("%s", recvline);
    }
    close(socketfd);
    return 0;
}