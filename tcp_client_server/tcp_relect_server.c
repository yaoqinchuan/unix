#include <unp.h>
#include <stdio.h>
#define handle_error(msg) do { \
    perror(msg);exit(EXIT_FAILURE);}while (0)

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("child %d terminated\n",pid);
    }
    return;
}
Sigfunc * signal(int signo, Sigfunc *func) 
{
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
    }else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}
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
    (void) signal(SIGCHLD, sig_chld);
    while (1)
    {
        int connfd = accept(socketfd, (SA *)NULL, NULL);
        if (fork() == 0) {
            char buf[1024] = {0};
            int n = 0;
            while ((n = read(connfd, buf, 1024)) > 0)
            {
                write(connfd, buf, n);
            }
            close(connfd);
            exit(0);
        }
        close(connfd);
    }
    return 0;
}