 ///
 /// @file    send_file.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-04 17:08:27
 ///
 
#include "func.h"
#include <sys/stat.h>
void send_data(int new_fd,char *filename)
{
	int len=0;
	long send_size=0;
	long recv_size=0;
	char buf[1000]={0};
	train t;
	memset(&t,0,sizeof(t));
	strcpy(t.buf,filename);
	t.len=strlen(t.buf);
	//发送文件名给对端
	int ret;
	ret=send_n(new_fd,(char*)&t,4+t.len);
	if(-1==ret)
	{
		perror("send");
		t.len=0;
		//发送空，标志结束
		send(new_fd,&t,4+t.len,0);
		return;
	}
	int fd;
	fd=open(filename,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	struct stat f;
	bzero(&buf,sizeof(buf));
	fstat(fd,&f);
	memset(&t,0,sizeof(t));
	 recv_n(new_fd,(char*)&len,sizeof(len));
     recv_n(new_fd,buf,len);
     recv_size=atoi(buf);    //接受已发送的文件大小
     send_size=f.st_size-recv_size;

	memcpy(t.buf,&f.st_size,sizeof(f.st_size)); //文件长度
	t.len=sizeof((long)f.st_size);
	send_n(new_fd,(char*)&t,4+t.len);//发送文件长度
	lseek(fd,recv_size,SEEK_SET);
	//文件内容
	while(memset(&t,0,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	   send_n(new_fd,(char*)&t,4+t.len);
#if 0
	while(memset(&t,0,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	{
	   memset(&t,0,sizeof(t));
	   if(flag_end==0){
	   t.len=1;
	   strcpy(t.buf,"0");
	   send_n(new_fd,(char*)&t,4+t.len);
	   close(new_fd);
		break;
		}else{
	   t.len=1;
	   strcpy(t.buf,"1");
	   send_n(new_fd,(char*)&t,4+t.len);
	   memset(&t,0,sizeof(t));
	   if((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
		   send_n(new_fd,(char*)&t,4+t.len);
	   else
	   {
		break;
	   }
		}
	}
#endif
	t.len=0;
	//发送空，标志结束
	send(new_fd,&t,4+t.len,0);
	//close(new_fd);
}
