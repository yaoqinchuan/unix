#include <unp.h>

#define ERR 1
#define OK 0
#define TCP_WAIT_COUNT 1024
#define SELECT_COUNT 2048
#define MAX_BUF_LEN 2048
void sig_child(int signo)
{
    pid_t pid;
    int stat;
    if ((pid = waitpid(pid, &stat, WNOHANG) > 0)) {
        printf("child %d terminated.\r\n",pid);
    }
    return;
}
Sigfunc *signal(int signo, Sigfunc *func) {
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}
int init_tcp_udp_socket_fd(int *tcp_socket_fd, int *udp_socket_fd)
{
    struct sockaddr_in client_ip = {0};
    client_ip.sin_addr.s_addr = htonl(INADDR_ANY);
    client_ip.sin_family = AF_INET;
    client_ip.sin_port = htons(SERV_PORT);
    *tcp_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*tcp_socket_fd < 0)
    {
        perror("Create tcp socket filed.\r\n");
        return ERR;
    }
    // ip and port can be resued.
    setsockopt(*tcp_socket_fd, SO_REUSEADDR, AF_INET, (const void *)NULL, 0);
    *udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*udp_socket_fd < 0)
    {
        perror("Create udp socket filed.\r\n");
        return ERR;
    }
    if (bind(*tcp_socket_fd, (SA *)&client_ip, sizeof(client_ip)) < 0)
    {
        perror("TCP Bind failed.\r\n");
        return ERR;
    };
    if (bind(*udp_socket_fd, (SA *)&client_ip, sizeof(client_ip)) < 0)
    {
        perror("UDP Bind failed.\r\n");
        return ERR;
    };
    if (listen(*tcp_socket_fd, TCP_WAIT_COUNT) < 0)
    {
        perror("Listen failed.\r\n");
        return ERR;
    }
    return OK;
}
int main()
{
    int map_index = 0;
    int tcp_socket_fd = 0;
    int udp_socket_fd = 0;
    fd_set all_fd_set, r_fd_set;
    int flag[SELECT_COUNT] = {0}; //save select fd to set
    for (int i = 0; i < SELECT_COUNT; i++)
    {
        flag[i] = -1;
    }
    if (init_tcp_udp_socket_fd(&tcp_socket_fd, &udp_socket_fd) != OK)
    {
        return OK;
    }
    struct timeval timeout = {0};
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    FD_SET(tcp_socket_fd, &all_fd_set);
    FD_SET(udp_socket_fd, &all_fd_set);
    int index = 1;
    flag[0] = tcp_socket_fd;
    map_index = (tcp_socket_fd > udp_socket_fd ? tcp_socket_fd : udp_socket_fd);
    (void)signal(SIGCHLD, sig_child);
    while (1)
    {
        r_fd_set = all_fd_set;
        int n = select(map_index + 1, &r_fd_set, NULL, NULL, NULL);
        // new tcp connect;
        if (FD_ISSET(tcp_socket_fd, &r_fd_set))
        {
            int conn_fd = accept(tcp_socket_fd, (SA *)NULL, NULL);
            if (conn_fd < 0)
            {
                perror("Tcp connect failed.\r\n");
            }
            for (int j = 0; j < SELECT_COUNT; j++)
            {
                if (flag[j] == -1)
                {
                    flag[j] = conn_fd;
                    map_index = (map_index > conn_fd ? map_index : conn_fd);
                    index = (index > j ? index : j);
                    break;
                }
            }
        }

        for (int i = 0; i <= index; i++)
        {
            if (flag[i] > 0)
            {
                FD_SET(flag[i], &all_fd_set);
            }
        }

        // for tcp proc
        for (int i = 0; i <= index; i++)
        {
            if (flag[i] < 0)
            {
                continue;
            }
            if (FD_ISSET(flag[i], &r_fd_set))
            {
                if (fork() == 0)
                {
                    char r_buf[MAX_BUF_LEN] = {0};
                    int number = read(flag[i], r_buf, MAX_BUF_LEN);
                    if (number > 0)
                    {
                        if (write(flag[i], r_buf, number) < 0)
                        {
                            perror("tcp write failed.\r\n");
                        };
                    }
                    close(flag[i]);
                    FD_CLR(flag[i], &all_fd_set);
                    flag[i] = -1;
                    exit(0);
                }
            }
        }

        // for udp use
        if (FD_ISSET(udp_socket_fd, &r_fd_set))
        {
            char r_buf[MAX_BUF_LEN] = {0};
            struct sockaddr client_ip = {0};
            socklen_t ip_len = sizeof(client_ip);
            int number = recvfrom(udp_socket_fd, r_buf, MAX_BUF_LEN, 0, (SA *)&client_ip, &ip_len);
            if (number > 0)
            {
                sendto(udp_socket_fd, r_buf, number, 0, (SA *)&client_ip, ip_len);
            }
        }
    }
}