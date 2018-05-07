 ///
 /// @file    send_n.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-04 20:43:22
 ///
 
#include "func.h"

int send_n(int sfd,char *p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}

int recv_n(int sfd,char *p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=recv(sfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}
