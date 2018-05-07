#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<sys/select.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<sys/epoll.h>
#include<netdb.h>
#include<string.h>
#define FILENAME  "file"
typedef struct lru
{
	int sfd;
	int task_flag;
	lru *next;
}lru,*plru; //用于lru算法，设置客户连接公平机制

typedef struct{
	int sfdbusy;
	int busy;
}business;
typedef struct{
	pid_t pid;
	int tfds;  //通过管道传递内核控制信息
	business b;
}child,*pchild;


typedef struct{
	int len;
	char buf[1000];
}train,*ptrain;

typedef struct{
	int len;
	char buf[502];
}train3g,*ptrain3g;


void que_update(plru *,plru *,int );
int que_delete(plru *,plru *);
int que_check(plru *,plru *,plru*);
void que_insert(plru * ,plru * ,int );


void send_data(int, char*);
void recv_data(int,char*);
void make_child(pchild ,int);
void send_fd(int ,int);
void recv_fd(int ,int*);
int send_n(int ,char *,int);
int recv_n(int ,char *,int);
void collect_include(int);
int LoginUser(char*,char*);
int RegisterUser(char*,char*);
int DeleteUser(char*);
void menu(int);
char *MD5_file (char *path, int f_size);
void hdfs_down(const char*);