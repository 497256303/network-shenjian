#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h> 
#include <time.h>


using namespace std;

#define BUFLEN 200
#define LEN 10000
#define port 80
#define IP "192.168.192.17"

//״̬����
char request1[] = "POST /include/auth_action.php HTTP/1.1\r\n\
Host: 192.168.192.17\r\n\
Connection: keep-alive\r\n\
Content-Length: 115\r\n\
Accept: */*\r\n\
Origin: http://192.168.192.17\r\n\
X-Requested-With: XMLHttpRequest\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Referer: http://192.168.192.17/srun_portal_pc.php?ac_id=7&userip=10.22.6.216&uservlan=600&userurl=http://www.msftconnecttest.com/redirect\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
Cookie: login=bQ0pOyR6IXU7PJaQQqRAcBPxGAvxAcrunljYBafnq%252BCGRngyKtQWFBTO1nYjg2lVJagdAbG6%252Fca5qolOFAWTgUB3xmwVq3T7P%252B0VxfDfDmD9BVvDNyXFF5VhnpyUHnC4oDO%252FluZ7t5T53HB4HebaOjeIt6KxU1rYb%252BrQEhJaiXqeYGCRpL0mPqDz; login=bQ0pOyR6IXU7PJaQQqRAcBPxGAvxAcrunljYBafnq%252BCGRngyKtQWFBTO1nYjg2lVJagdAbG6%252Fca5qolOFAWTgUB3xmwVq3T7P%252B0VxfDfDmD9BVvDNyXFF5VhnpyUHnC4oDO%252FluZ7t5T53HB4HebaOjeIt6KxU1rYb%252BrQEhJaiXqeYGCRpL0mPqDz\r\n\
action=login&username=1551265&password={B}MTU5MzU3&ac_id=7&nas_ip=&user_mac=&save_me=1&ajax=1";

//�˳�����
char request2[] = "GET /F.htm HTTP/1.1\r\n\
Host: 192.168.1.100\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
Referer: http://192.168.1.100/\r\n\
Accept-Encoding: gzip, deflate, sdch\r\n\
Accept-Language: zh-TW,zh;q=0.8,en-US;q=0.6,en;q=0.4\r\n\
Cookie: PHPSESSID=mnruqf0miqepp468c4u08gieo0\r\n\r\n\
";

char request3[] = "GET /srun_portal_pc.php?ac_id=7&userip=10.22.6.216&uservlan=600&userurl=http://www.msftconnecttest.com/redirect HTTP/1.1\r\n\
Host: 192.168.192.17\r\n\
Connection: keep-alive\r\n\
Cache-Control: max-age=0\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
Cookie: login=bQ0pOyR6IXU7PJaQQqRAcBPxGAvxAcrunljYBafnq%252BCGRngyKtQWFBTO1nYjg2lVJagdAbG6%252Fca5qolOFAWTgUB3xmwVq3T7P%252B0VxfDfDmD9BVvDNyXFF5VhnpyUHnC4oDO%252FluZ7t5T53HB4HebaOjeIt6KxU1rYb%252BrQEhJaiXqeYGCRpL0mPqDz\r\n\
\r\n";


void read_cfg(char username[], char password[], char retrytime[], char checktime[])
{
	ifstream fin;
	fin.open("/home/test/autologin.cfg", ios::in);
	if (!fin.is_open())
	{
		cout << "���ļ�ʧ��" << endl;
		exit(1);
	}
	char buf[BUFLEN];
	fin.getline(buf, BUFLEN, '\n');
	while (buf[0] != '\0')
	{
		if (buf[0] == '#')
		{
			fin.getline(buf, BUFLEN, '\n');
			continue;
		}
		//username
		char *p;
		int i = 0;
		p = strstr(buf, "username = ");
		if (p)
		{
			while (*p != '=')
				p++;
			p++;
			p++;
			while (*p != '#'&&*p != ' '&&*p != '\t')
			{
				username[i] = *p;
				p++;
				i++;
			}
			username[i] = '\0';
		}
		//password
		p = strstr(buf, "password = ");
		if (p)
		{
			while (*p != '=')
				p++;
			p++;
			p++;
			i = 0;
			while (*p != '#'&&*p != ' '&&*p != '\t')
			{
				password[i] = *p;
				p++;
				i++;
			}
			password[i] = '\0';
		}
		//retrytime
		p = strstr(buf, "retrytime = ");
		if (p)
		{
			while (*p != '=')
				p++;
			p++;
			p++;
			i = 0;
			while (*p != '#'&&*p != ' '&&*p != '\t')
			{
				retrytime[i] = *p;
				p++;
				i++;
			}
			retrytime[i] = '\0';
		}
		//checktime
		p = strstr(buf, "checktime = ");
		if (p)
		{
			while (*p != '=')
				p++;
			p++;
			p++;
			i = 0;
			while (*p != '#'&&*p != ' '&&*p != '\t')
			{
				checktime[i] = *p;
				p++;
				i++;
			}
			checktime[i] = '\0';
		}
		fin.getline(buf, BUFLEN, '\n');
	}
	fin.close();
}

void set_infor(char infor[], char username[], char password[])
{
	sprintf(infor, "POST / HTTP/1.1\r\n\
Host: 192.168.1.100\r\n\
Connection: keep-alive\r\n\
Content-Length: 34\r\n\
Cache-Control: max-age=0\r\n\
Origin: http://192.168.1.100\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\
Referer: http://192.168.1.100/\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-TW,zh;q=0.8,en-US;q=0.6,en;q=0.4\r\n\
Cookie: PHPSESSID=mnruqf0miqepp468c4u08gieo0s\r\n\
\r\n\
DDDDD=%s&upass=%s&0MKKey=\
", username, password);
}

void get_time(char t[])
{
	bzero(t, 20);
	struct tm *ptm;
	long ts;
	int y, m, d, h, n, s;
	ts = time(NULL);
	ptm = localtime(&ts);
	y = ptm->tm_year + 1900;
	m = ptm->tm_mon + 1;
	d = ptm->tm_mday;
	h = ptm->tm_hour;
	n = ptm->tm_min;
	s = ptm->tm_sec;
	sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d", y, m, d, h, n, s);
	t[19] = 0;
}

int if_login(int sockfd)
{
	fd_set rfds, wfds, master;
	int maxfd = sockfd;
	FD_ZERO(&master);
	FD_SET(sockfd, &master);
	int len, flag = 0;
	char buf[LEN];
	char time[20];
	while (1)
	{
		rfds = master;
		if (flag)
		{
			FD_ZERO(&wfds);
		}
		else
		{
			wfds = master;
			flag = 1;
		}
		if (select(maxfd + 1, &rfds, &wfds, NULL, NULL) == -1)
		{
			cout << "select����" << endl;
			exit(1);
		}
		if (FD_ISSET(sockfd, &rfds))
		{
			len = read(sockfd, buf, LEN);
			if (len < 0)
			{
				cout << "read����" << endl;
				close(sockfd);
				exit(1);
			}
			cout << "���յ�״̬" << endl;
			buf[len] = '\0';
			cout << buf << endl;
			return 0;
			char *login = strstr(buf, "Login</font>");
			char *logout = strstr(buf, "Logout</font>");
			if (login)
			{
				get_time(time);
				cout << time << " δ��¼" << endl;
				return 0;
			}
			else if (logout)
			{
				get_time(time);
				cout << time << " �ѵ�¼" << endl;
				return 1;
			}
		}
		if (FD_ISSET(sockfd, &wfds))
		{
			len = write(sockfd, request3, strlen(request3));
			if (len <= 0)
			{
				cout << "write���ͳ���" << endl;
				exit(1);
			}
			else
			{
				get_time(time);
				cout << time << " ������������״̬..." << endl;
			}
		}
	}
}

void login(int sockfd,char request[])
{
	fd_set rfds, wfds, master;
	int maxfd = sockfd;
	FD_ZERO(&master);
	FD_SET(sockfd, &master);
	int len, flag = 0;
	char buf[LEN];
	char time[20];
	while (1)
	{
		rfds = master;
		if (flag)
		{
			FD_ZERO(&wfds);
		}
		else
		{
			wfds = master;
			flag = 1;
		}
		if (select(maxfd + 1, &rfds, &wfds, NULL, NULL) < 0)
		{
			cout << "select����" << endl;
			exit(1);
		}
		if (FD_ISSET(sockfd, &rfds))
		{
			len = read(sockfd, buf, LEN);
			if (len < 0)
			{
				cout << "read����" << endl;
				close(sockfd);
				exit(1);
			}
			buf[len] = '\0';
			cout << buf << endl;
			return;
			char *p = strstr(buf, "You have successfully logged into our system");
			if (p)
			{
				get_time(time);
				cout << time << " ��¼�ɹ�" << endl;
				return;
			}
			p = strstr(buf, "ldap auth error");
			if (p)
			{
				get_time(time);
				cout << time << " ��¼ʧ��" << endl;
				return;
			}
		}
		if (FD_ISSET(sockfd, &wfds))
		{
			len = write(sockfd, request1, strlen(request1));//��ʱ�޸�
			if (len <= 0)
			{
				cout << "write���ͳ���" << endl;
				exit(1);
			}
			else
			{
				get_time(time);
				cout << time << " ���ڵ�¼..." << endl;
			}
		}
	}
}

int main()
{
	daemon(0, 1);
	signal(SIGCHLD, SIG_IGN);

	//��ȡ�����ļ�
	char username[20];
	char password[20];
	char retrytime[10];
	char checktime[10];
	read_cfg(username, password, retrytime, checktime);
	int retry = atoi(retrytime);
	int check = atoi(checktime);

	//������������
	char request[LEN];
	set_infor(request, username, password);

	int sockfd;
	struct sockaddr_in serv_addr;
	char time[20];
	while (1)
	{
		//����socket
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			cout << "socket����" << endl;
			exit(1);
		}
		memset(&serv_addr, 0, sizeof(serv_addr));  //ÿ���ֽڶ���0���
		serv_addr.sin_family = AF_INET;  //ʹ��IPv4��ַ
		serv_addr.sin_addr.s_addr = inet_addr(IP);
		serv_addr.sin_port = htons(port);
		get_time(time);
		cout << time << " ������֤������..." << endl;
		sleep(1);

		while (1)
		{
			if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
			{
				get_time(time);
				cout << time << " ������֤������ʧ�ܣ��ȴ�" << retry << "����������..." << endl;
				sleep(retry);
			}
			else
			{
				get_time(time);
				cout << time << " ������֤�������ɹ�" << endl;
				break;
			}
		}
		if (if_login(sockfd))
		{
			get_time(time);
			cout << time << " ���紦������״̬" << endl;
		}
		else
		{
			get_time(time);
			cout << time << " ���紦�ڶϿ�״̬" << endl;
			cout << time << " ��������..." << endl;
			login(sockfd, request1);
		}
		close(sockfd);
		get_time(time);
		cout << time << " �ȴ�" << check << "������¼������״̬" << endl;
		sleep(check);
	}
	return 0;
}