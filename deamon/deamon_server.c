#include "unp.h"
#include <syslog.h>
#include <time.h>
#define MAXFD 64
#define FILE_PATH "/home/yqc/code/unix/unix/deamon/log.log"
#define SERVER_PORT 13
#define MAX_CONNECT_COUNT 1024
#define MAX_LINE 2048
static int deamon_init(const char *pname, int facility, int *file_fd)
{
    int i;
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        return -1;
    }
    else if (pid)
    {
        _exit(0);
    }
    // become the session leader
    if (setsid() < 0)
    {
        return -1;
    }
    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) < 0)
    {
        return -1;
    }
    else if (pid)
    {
        _exit(0);
    }
    // not the session leader
    chdir("/");
    //clear all father file descriptors it inherites from
    for (int i = 0; i < MAXFD; i++)
    {
        close(i);
    }

    *file_fd = open(FILE_PATH, O_RDWR, 0777);

    // redirect stdin,stdout,stderr to FILE_PATH
    for (int i = 0; i < 3; i++)
    {
        dup2(*file_fd, i);
    }
    if (*file_fd < 0)
    {
        return -1;
    }
    return 0;
}
static int server_init(int *sockfd)
{
    struct sockaddr_in ip = {0};
    ip.sin_addr.s_addr = htonl(INADDR_ANY);
    ip.sin_family = AF_INET;
    ip.sin_port = htons(SERVER_PORT);
    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Create socket failed.\r\n");
        fflush(stdout);
        return -1;
    }
    if (bind(*sockfd, (SA *)&ip, sizeof(ip)) < 0)
    {
        printf("Bind socket failed.\r\n");
        fflush(stdout);
        return -1;
    }
    if (listen(*sockfd, MAX_CONNECT_COUNT) < 0)
    {
        printf("Listen socket failed.\r\n");
        fflush(stdout);
        return -1;
    }
    return 0;
}

static void prinrf_cli_ip(struct sockaddr_in *cli_ip)
{
    char show_buf[MAX_LINE] = {0};
    fflush(stdout);
    printf("New connect:\r\n");
    printf("Client ip is %s\r\n", inet_ntop(AF_INET, &(cli_ip->sin_addr), show_buf, sizeof(show_buf)));
    printf("Client port is %d\r\n", cli_ip->sin_port);
    printf("\r\n");
    fflush(stdout);
}
static void process_req(int sockfd)
{
    char buff[MAX_LINE] = {0};
    time_t ticks;
    for (;;)
    {
        struct sockaddr_in cli_ip = {0};
        socklen_t cli_ip_len = sizeof(cli_ip);
        int connfd = accept(sockfd, (SA *)&cli_ip, &cli_ip_len);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        memset(buff, 0, sizeof(buff));
        prinrf_cli_ip(&cli_ip);
    }
}
int main(int argc, char **arg)
{
    int sock_fd = 0;
    int file_fd = 0;
    if (deamon_init(arg[0], 0, &file_fd) < 0)
    {
        return 0;
    }
    if (server_init(&sock_fd) != 0)
    {
        return 0;
    }
    process_req(sock_fd);
    close(file_fd);
    return 0;
}
