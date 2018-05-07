#include "func.h"
#include "md5.h"
#include <time.h>
//int fds_exit[2];
int sfd;
#if 1
void SignHandler(int i)
{
	printf("down/up load interrupt:%d\n",i);
	flag_end = 0;
	//char flag ='b';
	//write(fds_exit[1],&flag,sizeof(flag));
}
#endif
#if 0
train t;
bzero(&t,sizeof(t));
t.len=0;
send_n(sfd,(char*)&t,4+t.len);
}
#endif

int main(int argc,char* argv[])
{
	//pipe(fds_exit);
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	//int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	flag_end=1;
	signal(SIGINT,SignHandler);
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	char buf[1000]={0};
	menu(sfd);
	int epfd = epoll_create(1);
	if(-1==epfd)
	{
		perror("epoll_create");
		return -1;
	}
	int len;
	struct epoll_event event,earr[2];
	memset(&event,0,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=0;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	if(-1==ret)
	{
		perror("epoll_cat");
		return -1;
	}
	event.events=EPOLLIN;
	event.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	int j;
	int i;
	//train t;
	while(1)
	{
		bzero(buf,sizeof(buf));
		memset(earr,0,sizeof(earr));
		j=epoll_wait(epfd,earr,2,-1);
		for(i=0;i<j;i++)
		{
			if(earr[i].data.fd==0)
			{
				bzero(buf,sizeof(buf));
				scanf("%s",buf);

				if(!strcmp(buf,"md5")){
					char *md5;
					printf("input download file:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					md5 = MD5_file(buf,32);
					printf("md5:%s",md5);
				}
#if 1
				if(!strcmp(buf,"put"))
				{
					//signal(SIGINT,SignHandler);
					printf("input upload file:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					put(sfd,buf);
				}
#endif
				if(!strcmp(buf,"collection_gets"))
				{
					//signal(SIGINT,SignHandler);
					collect_show(sfd);
				}
				if(!strcmp(buf,"cd"))
				{
				}

				if(!strcmp(buf,"menu"))
				{
					loginmenu();
				}

				if(!strcmp(buf,"ls"))
				{
					ls(sfd);
				}

				if(!strcmp(buf,"pwd"))
				{
					pwd(sfd);
				}

				if(!strcmp(buf,"gets"))
				{
					//signal(SIGINT,SignHandler);
					printf("input download file:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					gets(sfd,buf);
				}
#if 1
				if(!strcmp(buf,"put"))
				{
					//signal(SIGINT,SignHandler);
					printf("input upload file:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					put(sfd,buf);
				}
#endif
				 if(!strcmp(buf,"hdfs"))
                 {
                    train t;
                    memset(&t,0,sizeof(t));
                    t.len=4;
                    strcpy(t.buf,"hdfs");
                    send_n(sfd,(char*)&t,4+t.len); //发送名字
                    printf("input download from hdfs file:\n");
                    bzero(buf,sizeof(buf));
                    scanf("%s",buf);
                    memset(&t,0,sizeof(t));
                    t.len=strlen(buf);
                    strcpy(t.buf,buf);
                    send_n(sfd,(char*)&t,4+t.len); //发送名字
                    char flag=1;
                    recv_n(sfd,&flag,sizeof(flag));//确定下载完成
                    printf("server exist file\n");
                   }
				  if(!strcmp(buf,"hdfstime"))
                 {
                    train t;
                    memset(&t,0,sizeof(t));
                    t.len=8;
                    strcpy(t.buf,"hdfstime");
                    send_n(sfd,(char*)&t,4+t.len); //·?????
                    printf("input download from hdfs file:\n");
                    bzero(buf,sizeof(buf));
                    scanf("%s",buf);
                    memset(&t,0,sizeof(t));
                    t.len=strlen(buf);
                    strcpy(t.buf,buf);
                    send_n(sfd,(char*)&t,4+t.len);
                    char flag=1;
                    recv_n(sfd,&flag,sizeof(flag));
                    printf("server exist file\n");
                  }


				if(!strcmp(buf,"collection_gets"))
				{
					//signal(SIGINT,SignHandler);
					collect_show(sfd);
				}
#if 1
				if(!strcmp(buf,"cd"))
				{
					printf("input road:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					cd(sfd,buf);
				}
#endif
				if(!strcmp(buf,"rm"))
				{
					printf("input delete file:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					rm(sfd,buf);
				}

				if(!strcmp(buf,"collect_reserve"))
				{
					printf("input reserve file:\n");
					bzero(buf,sizeof(buf));
					scanf("%s",buf);
					collect_reserve(sfd,buf);
				}else
				{
	//				memset(&t,0,sizeof(t));
	//				t.len=strlen(buf);
	//				strcpy(t.buf,buf);
	//				send_n(sfd,(char*)&t,4+t.len);	
				}
			}//end of if epfd == 0
			if(earr[i].data.fd==sfd)
			{
				memset(buf,0,sizeof(buf));
				recv_n(sfd,(char*)&len,sizeof(len));
				recv_n(sfd,buf,len);
				printf("%s\n",buf);
				system(buf);
			}
		}
	}
	close(sfd);
}
