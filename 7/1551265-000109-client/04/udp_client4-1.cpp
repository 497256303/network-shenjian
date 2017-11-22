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

#define BUFLEN 30  //Max length of buffer
  
int main(int argc,char* argv[])
{
    struct sockaddr_in si_other;
    int s, i;
    char buf[BUFLEN];
    char message[BUFLEN];
    int port=atoi(argv[2]);

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exit(-1);
    }
    socklen_t slen = sizeof(si_other);
    memset((char *) &si_other, 0, sizeof(si_other));

    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    si_other.sin_addr.s_addr = inet_addr(argv[1]); 

    int flags = fcntl(s, F_GETFL, 0);                        //��ȡ�ļ���flags1ֵ��
    fcntl(s, F_SETFL, flags | O_NONBLOCK);   //���óɷ�����ģʽ��
    memset(message,0,sizeof(message));
    while(1)
    {
        if (sendto(s, message, sizeof(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            exit(-1);
        }
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) >0)
        {
            cout<<"���ӳɹ�"<<endl;
            break;
        }
    }
    for(i=0;i<BUFLEN;i++)
        message[i]=i%10+'0';

    int send_len=sendto(s, message, sizeof(message) , 0 , (struct sockaddr *) &si_other, slen);
    cout<<"send:";
    for(i=0;i<BUFLEN;i++)
        cout<<message[i];
    cout<<endl;
          
    return 0;
}
