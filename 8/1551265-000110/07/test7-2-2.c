#include <unistd.h>  
#include <sys/file.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
 

int lock_set(int fd, int type)  
{  
    struct flock old_lock, lock;  
    lock.l_whence = SEEK_SET;  
    lock.l_start = 0;  
    lock.l_len = 0;  
    lock.l_type = type;  
    lock.l_pid = -1;  
      
    // �ж��ļ��Ƿ��������   
    fcntl(fd, F_GETLK, &lock);  
      
    if (lock.l_type != F_UNLCK)  
    {  
        //�ж��ļ�����������ԭ�� 
        if (lock.l_type == F_RDLCK) 
            printf("Read lock already set by %d\n", lock.l_pid);  
        else if (lock.l_type == F_WRLCK)   
            printf("Write lock already set by %d\n", lock.l_pid);  
    }  
      
    lock.l_type = type;  
    //���ݲ�ͬ��typeֵ��������ʽ��������� 
    if ((fcntl(fd, F_SETLKW, &lock)) < 0)  
    {  
        printf("Lock failed:type = %d\n", lock.l_type);  
        return 1;  
    }  
          
    switch(lock.l_type)  
    {  
        case F_RDLCK:  
            printf("Read lock set by %d\n", getpid());  
            break;  
  
        case F_WRLCK:  
            printf("Write lock set by %d\n", getpid());  
            break;  
  
        case F_UNLCK:  
            printf("Release lock by %d\n", getpid());  
            return 1;  
        default:  
            break;  
    }
    return 0;  
}  




int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    int fd;  
		   
	fd = open("test7",O_RDWR | O_CREAT, 0666);  
	if(fd < 0)  
	{  
		printf("Open file error\n");  
		exit(1);  
	}  
	  
	fcntl(fd,F_SETFL,fcntl(fd,F_GETFL)|O_NONBLOCK);
	//fcntl(fd,F_SETFL,O_NONBLOCK);

	/* ���ļ��϶����� */  
	lock_set(fd, F_RDLCK); 

	int ret;
	char buf[100] = {0};
	ret = read(fd, buf, 20);
    if (ret < 0)
        printf("readʧ��\n");
    else
        printf("read:%d\n%s\n", ret, buf);

	/* ���ļ����� */  
	lock_set(fd, F_UNLCK);    
	close(fd);  

	while(1)  
        sleep(1);
} 