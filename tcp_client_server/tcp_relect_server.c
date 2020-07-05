#include <unp.h>
#include <stdio.h>
#define handle_error(msg) do { \
    perror(msg);exit(EXIT_FAILURE);}while (0)
int main(int argc, char **arg)
{
    struct sockaddr_in client_ip = { 0 };

    client_ip.sin_addr.s_addr = htonl(INADDR_ANY);
    client_ip.sin_family = AF_INET;
    client_ip.sin_port = htons(SERV_PORT);
    
    int socketfd;
    
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Create socket failed!\r\n");
        return 0;
    }
    if (socketfd < 0)
    {
        printf("Server create socket failed.\r\n");
        return 0;
    }
    if (bind(socketfd, (struct sockaddr *)&client_ip, sizeof(client_ip)) == -1) {
        printf("bind failed!\r\n");
        handle_error("bind");
        return 0;
    }
    listen(socketfd, LISTENQ);
    while (1)
    {
        int connfd = accept(socketfd, (SA *)NULL, NULL);
        char buf[1024] = {0};
        int n = 0;
        while ((n = read(connfd, buf, 1024)) > 0)
        {
            write(connfd, buf, n);
        }
        close(connfd);
        
    }
    return 0;
}