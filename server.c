/**
 * epoll 聊天室 server 端
 */
#include "common.h"

//最大缓冲连接数 
#define CONNECTION_PENDING_LIMIT 10
//epoll size
#define EPOLL_SIZE 1000
//欢迎语buff size
#define BUFFER_SIZE 100

int main(int argc, char *argv[])
{
    int i, sfd;
    //存储socket绑定地址的结构
    struct sockaddr_in sockAddr, clientAddr;
    //客户端连接地址长度信息
    socklen_t client_addr_length;
    //客户端连接文件描述符
    int clientFd;
    //socket
    int listener;
    //epoll instance
    int epfd;
    //存储返回的ready list文件描述符
    static struct epoll_event events[EPOLL_SIZE];
    //epoll_wait返回的ready list数量
    int epoll_events_count;
    //指定Internet domain socket IPv4连接方式
    sockAddr.sin_family = AF_INET;
    //指定server IP
    sockAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //指定监听端口(转换为network order)
    sockAddr.sin_port = htons(SERVER_PORT);
    //欢迎消息buffer
    char message[BUFFER_SIZE];

    //创建socket()
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        printf("create socket fail.\n");
        exit(-1);
    }
    printf("create server socket...\n");

    //绑定地址
    if (bind(listener, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        printf("bind address fail.\n");
        exit(-1);
    }
    //开始监听新加入的连接
    if (listen(listener, CONNECTION_PENDING_LIMIT) < 0) {
        printf("listen socket connect fail.\n");    
    }
    printf("start to listen:%s:%d\n", SERVER_IP, SERVER_PORT);

    //创建epoll instance
    epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        printf("epoll instance create fail.\n");    
        exit(-1);
    }
    //向epoll instance 添加监听事件
    addfd(epfd, listener);
    
    //死循环 等待事件发生
    while (1) {
        epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);          
        if (epoll_events_count < 0) {
            printf("epoll wait error.\n");
            break;
        }
        printf("get %d events...\n", epoll_events_count);
        //循环处理每一个事件
        for (i = 0; i < epoll_events_count; i++) {
            sfd = events[i].data.fd;
            //创建的socket listener 上有新的事件（新的客户端连接）
            if (sfd == listener) {
                //接受客户端连接
                client_addr_length = sizeof(struct sockaddr_in);
                clientFd = accept(listener, (struct sockaddr *)&clientAddr, &client_addr_length);
                printf("new client,ip:%s port:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                //客户端连接文件描述符加入epoll interest list
                addfd(epfd, clientFd);

                //发送服务端欢迎语
                sprintf(message, "welcome %d", clientFd);
                if (send(clientFd, message, BUFFER_SIZE, 0) < 0) {
                    printf("send welcome message error.\n");    
                    break;
                }
            } else {
                printf("client message\n");    
            }
        }
    }
    close(listener);
    close(epfd);

    return 0;
}
