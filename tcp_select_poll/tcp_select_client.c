#include <unp.h>
#include <stdlib.h>
#define FILE_PATH "./file.txt"
#define SERVER_IP "127.0.0.1"
#define ERR 1
#define OK 0
int init_socket(int *sockfd)
{
    struct sockaddr_in server_ip = {0};
    struct in_addr remote_ip = {0};
    if (inet_pton(AF_INET,SERVER_IP, (void *)&remote_ip) <= 0) {
        printf("invalid input ip!");
        return ERR;
    };
    server_ip.sin_addr = remote_ip;
    server_ip.sin_port = htons(SERV_PORT);
    server_ip.sin_family =AF_INET;

    *sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if (*sockfd < 0) {
        perror("socket create failed!");
        return ERR;
    }
    if (connect(*sockfd, (SA *) &server_ip, sizeof(server_ip)) < 0) {
        perror("connect failed!");
        return ERR;
    }
    return OK;
}
int read_file_size(int *filesize)
{
    FILE *fp = fopen(FILE_PATH, "r");
    if ( fp == NULL) {
        printf("open file %s failed", FILE_PATH);
        return ERR;
    }
    fseek(fp, 0, SEEK_END);
    *filesize = ftell(fp);
    fclose(fp);
    return OK;
}
int read_file_content(char *buf, int size)
{
    FILE *fp = fopen(FILE_PATH, "r");
    if (fp == NULL) {
        printf("open file %s failed", FILE_PATH);
        return ERR;
    }
    fread(buf, sizeof(char),size, fp);
    fclose(fp); 
    return OK;
}
int main()
{
    int file_size = 2048;
    int socketfd = 0;
    init_socket(&socketfd);

    char file_content[2048] = { 0 };
    char show_buf[2048] = { 0 };
    if (read_file_content(file_content, file_size) != OK) {
        return 0;
    }
    if (write(socketfd, file_content, file_size) < 0) {
        perror("write failed:");
    };
    printf("write success\r\n");
    printf("%s\r\n",file_content);
    int n_byte = 0;
    if((n_byte = read(socketfd, show_buf,file_size)) > 0) {
        show_buf[n_byte] = 0;
        printf("File content is: \r\n");
        printf("%s", show_buf);
    }
    close(socketfd);
    return 0;
}