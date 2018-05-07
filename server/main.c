 ///
 /// @file    main.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-04 12:23:10
 ///
 //

#include "func.h"
int fds_exit[2];
int clientsum[100] ;    ////counting clientsum
int clientstatic = 0;

void sig(int signum)
{
	char flag='h';
	printf("server exit:%d\n",signum);
	write(fds_exit[1],&flag,sizeof(flag));
}

int main(int argc,char * argv[])
{
	if(argc!=4)
	{
		perror("args\n");
		return -1;
	}
	signal(SIGINT,sig);
	pipe(fds_exit);
	int child_num=atoi(argv[3]);
	pchild p=(pchild)calloc(child_num,sizeof(child));
	make_child(p,child_num); //创建子进程并且初始化有数据结构
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);//ipv4,TCP
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2])); //小端
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	int epfd=epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(child_num+2,sizeof(struct epoll_event));
	memset(&event,0,sizeof(event));
	event.events=EPOLLIN;//读
	event.data.fd=sfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	event.events=EPOLLIN;//读
	event.data.fd=fds_exit[0];
    epoll_ctl(epfd,EPOLL_CTL_ADD,fds_exit[0],&event);
	 event.events=EPOLLIN;//读
     event.data.fd=0;
    epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);

	int i;
	for(i=0;i<child_num;i++)  //父进程监控每一个tfds
	{
		event.events=EPOLLIN;
		event.data.fd=p[i].tfds;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].tfds,&event);
	}
	listen(sfd,child_num+2); //端口打开
	int ret1;
	int new_fd;
	int j;
	char flag =1; 
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(struct sockaddr));
	socklen_t addrlen=sizeof(struct sockaddr);
	plru head=NULL,tail=NULL;
	while(1)
	{
		//memset(evs,0,sizeof(*evs));
		ret1=epoll_wait(epfd,evs,child_num+2,-1);
		for(i=0;i<ret1;i++)
		{
			if(evs[i].data.fd==sfd)//网络到达
			{
				//new_fd=accept(sfd,NULL,NULL);
				new_fd=accept(sfd,(struct sockaddr*)&clientaddr,&addrlen);
				printf("%s connect success\n",inet_ntoa(clientaddr.sin_addr));
				clientsum[clientstatic] = new_fd;
                printf("%d \n",clientsum[clientstatic]);
                ++clientstatic;
				
				//printf("accept :%d\n",new_fd);
				//找非忙碌子进程
				for(j=0;j<child_num;j++)
				{
					if(p[j].b.sfdbusy==0)
					{
						send_fd(p[j].tfds,new_fd);//描述符发给子进程
						printf("connect fd %d\n",p[j].tfds);
						que_insert(&head,&tail,p[j].tfds);  //添加消息管道为监控
						p[j].b.sfdbusy=1;
						p[j].b.busy=1;
						printf("find a not busy process,send fd and insert tfd success\n");
						break;
					}
					if(j==child_num-1){
						plru pcur;
						 int ret = que_check(&head,&tail,&pcur);
						printf("delete ret = %d\n",ret);
						if(ret==0){
						printf("write fd %d\n",head->sfd);
						write(head->sfd,&flag,sizeof(flag));
						//printf("write sfd\n");
						send_fd(p[j].tfds,new_fd);//描述符发给子进程
						//printf("send sfd child\n");
						que_insert(&head,&tail,head->sfd);  //添加消息管道为监控
						//printf("insert sfd\n");
						que_delete(&head,&tail); //删除最长时间没发送命令的用户
						printf("find a not busy process,send fd success\n");}
						break;
					}
				}
			 //   close(new_fd);//引用计数交给子进程
			}
			for(j=0;j<child_num;j++)
			{
				if(evs[i].data.fd==p[j].tfds)
				{
					read(p[j].tfds,&flag,sizeof(flag));
					p[j].b.busy=0;
					que_update(&head,&tail,p[j].tfds); //有新消息就更新列表
					//printf("child task is free\n");
					break;
				}
			}
			if(evs[i].data.fd==fds_exit[0])
			{
				read(fds_exit[0],&flag,sizeof(flag));
				epoll_ctl(epfd,EPOLL_CTL_DEL,sfd,&event);
				close(sfd);
				for(i=0;i<child_num;i++)
				{
					while(p[i].b.busy)
					{
						read(p[i].tfds,&flag,sizeof(flag));
						p[i].b.busy=0;
					}
					event.events=EPOLLIN;
					event.data.fd=p[i].tfds;
					epoll_ctl(epfd,EPOLL_CTL_DEL,p[i].tfds,&event);
					close(p[i].tfds);
				}
				kill(0,SIGKILL);
			}
			if(evs[i].data.fd==0){
                        char buf[1000]="";
						char bufn[1000]="";
                        char filename[100] = "";
                        string message;
                        read(0,buf,1000);
						strncpy(bufn,buf,strlen(buf)-2);


                        printf("send  filename:\n");
                        scanf("%s",filename);

                        for(int i=0;i<clientstatic;i++){
							message = bufn;
                              message = message + std::to_string(clientstatic) ;
                             message = message + " ";
                          message = message + std::to_string(i+1);
                            message = message + " ";
                              message = message + filename;
                                printf("%s\n",message.data());
                                  printf("%d\n",clientsum[i]);
                                    train t;
                              memset(&t,0,sizeof(t));
                        strcpy(t.buf,message.data());
                                 printf("%s\n",t.buf);
                                  t.len=message.length();
                              send_n(clientsum[i],(char*)&t,4+t.len);
                                                       }

                        }

		}
	}
	return 0;
}
