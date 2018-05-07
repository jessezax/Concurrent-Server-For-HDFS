#include "func.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void child_handle(int fdr)
{
	while(1){
	char flag=1;
	int new_fd;
	int len;
	char buf[1000]={0};
	train t;
	char itoa[200]={0};
	recv_fd(fdr,&new_fd);//从父进程接收任务
	printf("i can receive new_fd\n");
	menu(new_fd);

	int ret;
	int epfd=epoll_create(1);
	struct epoll_event event,evs[4];
	memset(&event,0,sizeof(event));
	event.data.fd=STDIN_FILENO;
	event.events=EPOLLIN;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);//注册标准输入
	if(-1==ret)
	{
		perror("epoll_ctl");
		return ;
	}
	event.data.fd=new_fd;
	event.events=EPOLLIN;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&event);//注册sfd
	if(-1==ret)
	{
		perror("epoll_ctl");
		return ;
	}
	event.data.fd=fdr;
	event.events=EPOLLIN;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fdr,&event);//注册管道
	if(-1==ret)
	{
		perror("epoll_ctl");
		return ;
	}
	int i;
	int ret1;

	int break_flag=0;
	while(1)
	{
		bzero(buf,sizeof(buf));
		memset(evs,0,sizeof(evs));
		ret1=epoll_wait(epfd,evs,4,-1);
		for(i=0;i<ret1;i++)
		{
			bzero(buf,sizeof(buf));
			if(evs[i].data.fd==new_fd){
				bzero(buf,sizeof(buf));
				bzero(&t,sizeof(t));
				recv_n(new_fd,(char*)&len,sizeof(len));
				recv_n(new_fd,buf,len);

				if(!strcmp(buf,"ls")){
		//			printf("this is ls\n");
					DIR *pDir;
					struct dirent *pDirInfo;
					struct stat statbuf;
					pDir=opendir(".");
					if(NULL==pDir){
						perror("opendir");
						return;
					}
					while((pDirInfo=readdir(pDir))!=NULL){
						bzero(buf,sizeof(buf));
						if(!(strcmp(".",pDirInfo->d_name)==0)&&!(strcmp("..",pDirInfo->d_name)==0)){ 		
							stat(pDirInfo->d_name,&statbuf);
							strcpy(buf,pDirInfo->d_name);
							strncpy(buf+strlen(pDirInfo->d_name),"    ",4);
							sprintf(itoa,"%ld",statbuf.st_size);
							strncpy(buf+strlen(pDirInfo->d_name)+4,itoa,strlen(itoa));
							t.len=strlen(buf);
							strcpy(t.buf,buf);
							send_n(new_fd,(char*)&t,4+t.len);
						}
					}
					t.len=0;
					send_n(new_fd,(char*)&t,4+t.len);	
					closedir(pDir);
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}


				if(!strcmp(buf,"pwd")){
					t.len=strlen(getcwd(NULL,0));
					strcpy(t.buf,getcwd(NULL,0));
					send_n(new_fd,(char*)&t,4+t.len);
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}

				if(!strcmp(buf,"gets")){
					printf("%s\n",buf);
					bzero(buf,sizeof(buf));
					recv_n(new_fd,(char*)&len,sizeof(len));
					recv_n(new_fd,buf,len);
					send_data(new_fd,buf);
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}

				if(!strcmp(buf,"collection_gets")){
					printf("%s\n",buf);
					bzero(buf,sizeof(buf));
					recv_n(new_fd,(char*)&len,sizeof(len));
					recv_n(new_fd,buf,len);
					//bzero(buf,sizeof(buf));
					//strcpy(buf,"collection.txt");
					send_data(new_fd,buf);
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}

				if(!strcmp(buf,"cd")){
					printf("%s\n",buf);
					bzero(buf,sizeof(buf));
					recv_n(new_fd,(char*)&len,sizeof(len));
					recv_n(new_fd,buf,len);
					printf("%s\n",buf);
					chdir(buf);    //修改本进程路径
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}

				if(!strcmp(buf,"put")){
					printf("%s\n",buf);
					bzero(buf,sizeof(buf));
					recv_n(new_fd,(char*)&len,sizeof(len));
					recv_n(new_fd,buf,len);
					recv_data(new_fd,buf);
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}

				if(!strcmp(buf,"rm"))
				{
					printf("%s\n",buf);
					bzero(buf,sizeof(buf));
					recv_n(new_fd,(char*)&len,sizeof(len));
					recv_n(new_fd,buf,len);
					unlink(buf);	
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}

				if(!strcmp(buf,"hdfs")){
                     printf("%s\n",buf);
                     bzero(buf,sizeof(buf));
                     recv_n(new_fd,(char*)&len,sizeof(len));
                     recv_n(new_fd,buf,len);
                     hdfs_down((const char*)buf);
                     printf("down finish\n");
                     send_n(new_fd,&flag,sizeof(flag));
                     write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
                  }

				if(!strcmp(buf,"hdfstime")){
                                 printf("%s\n",buf);
                                 bzero(buf,sizeof(buf));
                                 recv_n(new_fd,(char*)&len,sizeof(len));
                                 recv_n(new_fd,buf,len); //收到文件名
                                 char a[200]="hadoop jar FileWithTime.jar hbase.time.FileWithTime ";
                                 strcat(a,buf);
                                 strcat(a," ");
                                 strcat(a,"./time.txt");
                                 printf("%s\n",a);
                                 system(a);
                                 printf("down finish\n");
                                 send_n(new_fd,&flag,sizeof(flag));
                                 write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
                }



				if(!strcmp(buf,"collect_reserve")){
					printf("%s\n",buf);
					collect_include(new_fd);
					write(fdr,&flag,sizeof(flag));//通知父进程，完成发送任务
				}
				else
				{
			//		printf("%s\n",buf);
				}
			}
			if(evs[i].data.fd==0) //服务器与客户端通讯
			{
				memset(buf,0,sizeof(buf));
				memset(&t,0,sizeof(t));
				read(0,buf,sizeof(buf));
				strcpy(t.buf,buf);
				t.len=strlen(buf)-1;
				send_n(new_fd,(char*)&t,4+t.len);
			}
			if(evs[i].data.fd==fdr)
			{
				break_flag=1;
				printf("fdr\n");
				read(fdr,&flag,sizeof(flag));//知道父进程要终止自己去接待别的客户
				printf("recv exit\n");
				printf("recv exit\n");
				close(new_fd);
				break;
			}

		}
		if(break_flag==1)break;
		}
	}
}

void make_child(pchild p,int num)
{
	int i;
	int fds[2];
	pid_t pid;
	for(i=0;i<num;i++)//创建多个子进程
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(0==pid)
		{
			close(fds[1]);
			child_handle(fds[0]);
		}//让子进程永远不能从这个括号走出来
		close(fds[0]);
		p[i].pid=pid;//子进程的pid
		p[i].tfds=fds[1];//拿到管道的一端
		p[i].b.busy=0;//子进程处于空闲状态
		p[i].b.sfdbusy=0;//子进程处于空闲状态
	}
}
