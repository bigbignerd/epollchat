/**
 * epoll 聊天室头文件
 */
#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

#define EPOLL_SIZE 1000

#define TRUE 1
#define FALSE 0

/* 向epoll instance 添加监听文件描述符 */
void addfd(int epfd, int fd)
{
    struct epoll_event ev;    
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    printf("fd:%d add to epoll instance.\n", fd);
}


