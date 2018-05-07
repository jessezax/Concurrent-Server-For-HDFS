#include <strings.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>

#define FILENAME "file"
typedef struct{
		int sfdbusy;
			int busy;
}business;
typedef struct{
	pid_t pid;//子进程的pid
	int tfds;//通过该管道传递内核控制信息
	business b;//标示进程是否忙碌
}child,*pchild;

typedef struct{
	int len;
	char buf[1000];
}train,*ptrain;
int flag_end;
void menu();
void send_data(int,char*);
void recv_data(int,char*);
int recv_n(int,char*,int);
int send_n(int,char*,int);
void ls(int);
void rm(int,char*);
void gets(int ,char*);
void pwd(int);
void put(int ,char*);
void cd(int,char*);
void collect_reserve(int ,const char*);
void collect_show(int);
void menu(int);
void loginmenu();
