#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <arpa/inet.h>
using namespace std;
//#define port  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

int main(int argc,char* argv[])
{
    ///����sockfd
    int server_sockfd = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	perror("setsockopt(SO_REUSEADDR) failed");
    ///����sockaddr_in
    int port=atoi(argv[1]);
    struct sockaddr_in server_sockaddr,client_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    //int flags1 = fcntl(server_sockfd, F_GETFL, 0);                        //��ȡ�ļ���flags1ֵ��
    //fcntl(server_sockfd, F_SETFL, flags1 | O_NONBLOCK);   //���óɷ�����ģʽ��

    ///bind���ɹ�����0��������-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen���ɹ�����0��������-1
    // if(listen(server_sockfd,QUEUE) == -1)
    // {
    //     perror("listen");
    //     exit(1);
    // }

    ///�ͻ����׽���
    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    int slen = sizeof(client_sockaddr);


    ///�ɹ����طǸ������֣�������-1
    // int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    
    // int flags2 = fcntl(conn, F_GETFL, 0);                        //��ȡ�ļ���flags2ֵ��
    // fcntl(conn, F_SETFL, flags2 | O_NONBLOCK);   //���óɷ�����ģʽ��

 //    if(conn<0)
 //    {
 //        perror("connect");
 //        exit(1);
 //    }
 //    else
	// cout<<"���ӳɹ�!"<<endl;
    fflush(stdout);
    memset(sendbuf,0,sizeof(sendbuf));
    while(1)
    {
        if(sendto(server_sockfd, sendbuf, BUFFER_SIZE, 0, (struct sockaddr*) &client_sockaddr, slen) == -1)
            return -1;
        if(recvfrom(server_sockfd,recvbuf,BUFFER_SIZE,0,(struct sockaddr*) &client_sockaddr, &slen)>0)
        {
            cout<<"���ӳɹ�"<<endl;
            break;
        }
    }
    while(1)
    {
        memset(recvbuf,0,sizeof(recvbuf));
        recvfrom(server_sockfd,recvbuf,BUFFER_SIZE,0,(struct sockaddr*) &client_sockaddr, &slen);
    }
    // close(conn);
    close(server_sockfd);
    return 0;
}
