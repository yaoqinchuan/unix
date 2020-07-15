#include <unp.h>
#include <stdio.h>
int main(int argc, char **arg)
{
    int sockfd = 0;
    char remote_ip_s[] = "127.0.0.1";
    struct in_addr remote_ip = { 0 };
    if (inet_pton(AF_INET, remote_ip_s, (void *)&remote_ip) <= 0) {
        printf("Please input invalid ip address!\r\n");
        return 0;
    }

    struct sockaddr_in server_ip = { 0 };
    server_ip.sin_addr = remote_ip;
    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(SERV_PORT);
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Create socket failed!\r\n");
        return 0;
    }

    if (connect(sockfd, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0) {
        printf("connetc to server failed!\r\n");
    }

    char temp_char[1024] = {0};
    printf("Please input string:\r\n");
    while(fgets(temp_char, 1024, stdin)!= NULL) {
        if (write(sockfd, temp_char, strlen(temp_char)) == -1) {
            printf("Error:Send message to server failed!\r\n");
        }
        int n =0;
        char buf[1024] = {0};
        if ((n = read(sockfd,buf, 1024)) > 0) {
            buf[n] = 0;
            printf("%s", buf);
        }
    }
    printf("end");
    close(sockfd);
    exit(0);
}