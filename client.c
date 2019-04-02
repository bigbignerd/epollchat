/**
 * epoll 聊天室 client端
 */
#include "common.h"

#define BUFFER_SIZE 500

int main(int argc, char *argv[])
{ 
    int i;
    int pid;
    //管道
    int pipe_fd[2];
    //socket实例
    int sock;
    char message[BUFFER_SIZE];

    //server 地址信息结构体
    struct sockaddr_in sockAddr;
    //连接方式、ip、端口
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    sockAddr.sin_port = htons(SERVER_PORT);

    //epoll实例
    int epfd;
    //ready list event数量
    int epoll_events_count;
    //epoll监听events
    static struct epoll_event events[2];

    int isClientWork = TRUE;

    //创建socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("create socket fail.\n");
        exit(-1);
    }
    //连接 server socket
    if (connect(sock, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        printf("connect server fail.\n");
        exit(-1);
    }
    //创建管道用于子进程写数据，父进程读取数据
    if (pipe(pipe_fd) < 0) {
        printf("create pipe fail.\n");
        exit(-1);
    }
    //创建epoll instance
    epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        printf("create epoll instance fail.\n");    
        exit(-1);
    }
    //添加sockt 和 管道读文件描述符到 interest list   
    addfd(epfd, sock);
    addfd(epfd, pipe_fd[0]);
    
    pid = fork();
    if (pid < 0) {
        printf("fork fail.\n");    
        exit(-1);
    } else if (pid == 0) {
        //这里是子进程    
        //子进程关闭读端
        close(pipe_fd[0]);
        printf("input 'exit' if you want quit chat room.\n");
        while (isClientWork) {
            bzero(&message, BUFFER_SIZE);
            //从标准输入读取数据
            fgets(message, BUFFER_SIZE, stdin);
            //判断是否输入了'exit'
            if (strncasecmp(message, "exit", 4) == 0) {
                isClientWork = FALSE;    
            } else {
                //从标准输入读取的数据写入管道    
                if (write(pipe_fd[1], message, strlen(message) - 1) < 0) {
                    printf("client write pip fail.\n");
                    exit(-1);
                }
            }
        }
    } else {
        //这里是父进程    
        //父进程关闭写端
        close(pipe_fd[1]);
        while (isClientWork) {
            epoll_events_count = epoll_wait(epfd, events, 2, -1);               
            //处理事件
            for (i = 0; i < epoll_events_count; i++) {
                bzero(&message, BUFFER_SIZE);
                //服务端发来的消息    
                if (events[i].data.fd == sock) {
                    if (recv(sock, message, BUFFER_SIZE, 0) == 0) {
                        printf("server error\n");
                        isClientWork = FALSE;
                        close(sock);
                    } else {
                        printf("%s\n", message); 
                    }   
                } else {
                    //子进程写管道事件    
                    //从管道中读取数据
                    if (read(events[i].data.fd, message, BUFFER_SIZE) == 0) {
                        printf("parent read pipe fail\n");
                        isClientWork = FALSE;
                    } else {
                        send(sock, message, BUFFER_SIZE, 0);    
                    }
                }
            }
        }
    }
    if (pid > 0) {
        close(pipe_fd[0]);
        close(sock);
    } else {
        close(pipe_fd[1]);    
    }
    return 0;
}









