///
/// @file    checkin.c
/// @author  jesse(821228705@qq.com)
/// @date    2017-09-14 19:26:30
///

#include "func.h"
#include <string.h>
int loginuser(int sfd,char *name,char *password)
{
	int ret;
	train t;
	bzero(&t,sizeof(t));
	t.len=strlen(name);
	strcpy(t.buf,name);
	send_n(sfd,(char*)&t,4+t.len);  //发送用户名

	bzero(&t,sizeof(t));
	t.len=strlen(password);
	strcpy(t.buf,password);
	send_n(sfd,(char*)&t,4+t.len);  //发送密码

	recv_n(sfd,(char*)&ret,4);
	printf("ret = %d\n",ret);
	
	return ret;
}


int adduser(int sfd,char *name,char *password)
{
int ret;
	train t;
	bzero(&t,sizeof(t));
	t.len=strlen(name);
	strcpy(t.buf,name);
	send_n(sfd,(char*)&t,4+t.len);  //发送用户名

	bzero(&t,sizeof(t));
	t.len=strlen(password);
	strcpy(t.buf,password);
	send_n(sfd,(char*)&t,4+t.len);  //发送密码

	recv_n(sfd,(char*)&ret,4);
	return ret;
}


int deleteuser(int sfd,char *name)
{
	int ret;
	train t;
	bzero(&t,sizeof(t));
	t.len=strlen(name);
	strcpy(t.buf,name);
	send_n(sfd,(char*)&t,4+t.len);  //发送用户名

	recv_n(sfd,(char*)&ret,4);
	return ret;
}

void menu(int sfd)
{
	int i;
	int rd[1]={0};
	while(1)
	{
		printf("login or do else....\n");
		printf("1.login\n");
		printf("2.register\n");
		printf("3.delete\n");
		char name[100]={0};
		char *password;
		int ret;
		memset(rd,0,sizeof(rd));
		read(0,rd,sizeof(rd));
		i=atoi((const char*)rd);
		if(i==1||i==2||i==3)
		send_n(sfd,(char*)&i,4);
		if(i==1)
		{
			printf("input your name:");
			scanf("%s",name);
			password = getpass("input your key:");
			ret=loginuser(sfd,name,password);
			if(!ret)
			{
				printf("login success\n");
				break;
			}
			else
			{
				printf("choose other choice\n");
			}
		}else if(i==2)
		{
			printf("input your name:");
			scanf("%s",name);
			password = getpass("input your key:");
			ret=adduser(sfd,name,password);
			if(!ret){
				printf("register success,you can login\n");
				}
			else{
				printf("choose other choice\n");
			}
		
		}else if(i==3)
		{
			printf("deleter user:");
			scanf("%s",name);
			ret=deleteuser(sfd,name);
			if(!ret)
			{
				printf("delete success,you can do other\n");
			}
			else
			{
				printf("choose other choice\n");
			}
		}
	}
}
