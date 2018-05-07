 ///
 // @file    checkin.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-14 15:09:54
 ///
 
#include "func.h"

int check_in(int sfd)
{
	char buf[100]={0};
	int len;
	char name[100]={0};
	char password[100]={0};
	recv_n(sfd,(char*)&len,4);
	recv_n(sfd,buf,len);   //接收到用户名
	strcpy(name,buf);

	memset(buf,0,sizeof(buf));
	recv_n(sfd,(char*)&len,4);
	recv_n(sfd,buf,len);   //接收到密码
	strcpy(password,buf);
	int ret;
	ret=LoginUser(name,password);
	send_n(sfd,(char*)&ret,4);
	return ret;
}

int adduser(int sfd)
{
	char buf[100]={0};
	int len;
	char name[100]={0};
	char password[100]={0};
	recv_n(sfd,(char*)&len,4);
	recv_n(sfd,buf,len);   //接收到用户名
	strcpy(name,buf);

	memset(buf,0,sizeof(buf));
	recv_n(sfd,(char*)&len,4);
	recv_n(sfd,buf,len);   //接收到密码
	strcpy(password,buf);
	int ret;
	ret=RegisterUser(name,password);
	send_n(sfd,(char*)&ret,4);
	return ret;
}

int deleteuser(int sfd)
{
	char buf[100]={0};
	int len;
	char name[100]={0};
	recv_n(sfd,(char*)&len,4);
	recv_n(sfd,buf,len);   //接收到用户名
	strcpy(name,buf);
	int ret;
	ret=DeleteUser(name);
	send_n(sfd,(char*)&ret,4);
	return ret;
}

void menu(int sfd)
{
	int i;
	int ret;
	while(1)
	{
		recv_n(sfd,(char*)&i,4);
		if(i==1)
		{
			ret=check_in(sfd);
			if(ret==0)
			{
				break;
			}
		}
		if(i==2)
		{
			ret=adduser(sfd);
			if(ret==0)
			{
				break;
			}
		}
		if(i==3)
		{
			ret=deleteuser(sfd);
			if(ret==0)
			{
				break;
			}
		}
	}

}
