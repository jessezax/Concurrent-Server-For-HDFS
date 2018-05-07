//
/// @file    send_file.c
/// @author  jesse(821228705@qq.com)
/// @date    2017-09-04 17:08:27
///
//#include "md5.h"
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iterator>
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::fstream;
using std::pair;
using namespace std;
#include "func.h"
#include <sys/stat.h>
#include <time.h>
#include <sys/mman.h>
void word()	
{
	fstream fst("collection.txt");
	if(!fst.good())
	{
		cout<<"fst"<<endl;
		return ;
	}
	string temp1;
	string temp2;
	string temp3;
	map< string,map<string,string> > msmss;
	while(fst>>temp1,fst>>temp2,fst>>temp3)
	{
		msmss[temp1].insert(pair<string,string>(temp2,temp3));
		msmss[temp1][temp2]=temp3;
	}
	//map<string,map<string,string> >::iterator it = msmss.begin();
	//while(it != msmss.end())
	fst.close();
	fstream fst2("collection.txt",ios::trunc|ios::in|ios::out);
	if(!fst2.good())
	{
		cout<<"fst"<<endl;
		return ;
	}
	for(auto i:msmss)
	{
		for(auto j:i.second)
		{
			fst2 << i.first 
				<< " "
				<< j.first
				<< " "
				<< j.second
				<<endl;
			;
		}
	}
	fst2.close();
	return;
}
char * fun_mmap(char *filename,long f_size)
{
	int fd;
	char *pmmap;
	fd=open(filename,O_RDWR);
	pmmap=(char*)mmap(NULL,f_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	//close(fd);
	return pmmap;
}

long file_stat(const char *file)
{
	int fd;
	struct stat p;
	memset(&p,0,sizeof(p));
	off_t f_size;
	fd=open(file,O_RDONLY);
	if(fd==-1)
	{
		f_size=0;
	}else{
		fstat(fd,&p);
		f_size=p.st_size;
	}
	close(fd);
	return f_size;
}

void send_data(int new_fd, char *filename)
{
	long f_size=0;
	long send_size=0;
	long recv_size=0;
	int len;
	int flag_end=0;
	char buf[1024]={0};
	int net;
	recv_n(new_fd,(char*)&net,sizeof(net));
	printf("net = %d\n",net);
	train t;
	short file_flag;
	memset(&t,0,sizeof(t));
	//strcpy(t.buf,filename);
	//t.len=strlen(t.buf);
	f_size=file_stat(filename);
	//发送文件名给对端
	char *md5;
	int ret;
	//	if(-1==ret)
	//	{
	//		perror("send");
	//		return;
	//	}
	int fd;
	int flag_md5;
	fd=open(filename,O_RDWR);
	if(-1==fd)
	{
		file_flag = 1;
		ret=send_n(new_fd,(char*)&file_flag,2);
		perror("open");
		return;
	}
	file_flag = 0;
	ret=send_n(new_fd,(char*)&file_flag,2);
	//	fd1=open(filename1,O_RDWR|O_CREAT,0666);
	//	if(-1==fd1)
	//	{
	//		perror("open");
	//		return;
	//	}
	memset(&t,0,sizeof(t));
	recv_n(new_fd,(char*)&len,sizeof(len));
	recv_n(new_fd,buf,len);
	recv_size=atoi(buf);    //接受文件大小
	md5 = MD5_file(filename,recv_size);
	memset(&t,0,sizeof(t));
	memset(buf,0,sizeof(buf));
	recv_n(new_fd,(char*)&len,sizeof(len));
	recv_n(new_fd,buf,len);  //接受MD5
	printf("recmd5: %s\n",buf);
	int recmd5 = atoi(buf);
	printf("nowcmd5: %s\n",md5);
	flag_md5 = (recmd5 == atoi(md5));
	send_n(new_fd,(char*)&flag_md5,sizeof(int));//发送文件是否一样


	if(flag_md5){
		send_size=f_size-recv_size;
		memcpy(t.buf,&send_size,sizeof(send_size)); 
		t.len=sizeof(send_size);
		send_n(new_fd,(char*)&t,4+t.len); //发送需要发送的文件长度
#if 0
		char *pmmap,*cur_mmap;
		int flag_mmap=1;
		//if(send_size>102400)
		//{
		pmmap=fun_mmap(filename,f_size);
		cur_mmap=pmmap;
		cur_mmap=cur_mmap+recv_size;
		flag_mmap=1;
		//}else{
#endif
		lseek(fd,recv_size,SEEK_SET);
		// }
	}else{
		memcpy(t.buf,&f_size,sizeof(f_size)); 
		t.len=sizeof(f_size);
		send_n(new_fd,(char*)&t,4+t.len); //发送需要发送的文件长度

		lseek(fd,0,SEEK_SET);
	}
#if 0
}
#endif
//文件内容
//
#if 0
while(1)
{
	memset(&t,0,sizeof(t));
	if(flag_mmap==0)
	{
		if((t.len=read(fd,t.buf,sizeof(t.buf)))==0)
		{
			break;
		}
	}
	else{
		if(send_size<=0)
		{
			t.len=0;
			break;
		}
		t.len=0;
		while(cur_mmap!=NULL&&t.len<999)
		{
			t.len=t.len+1;
			cur_mmap=cur_mmap+1;
		}
		cur_mmap=cur_mmap-t.len;
		memcpy(t.buf,cur_mmap,t.len);
		cur_mmap=cur_mmap+t.len;
		send_size=send_size-t.len;
	}
	send_n(new_fd,(char*)&t,4+t.len);
}
#endif
train3g t3g;
long n;
long m;
off_t length;
short mmap_break=0;
while(1)
{
//	memset(buf,0,sizeof(buf));
//	recv_n(new_fd,(char*)&len,sizeof(len));//接收是否继续下载的请求
//	recv_n(new_fd,buf,len);
//	flag_end=atoi(buf);
//	if(flag_end==0)
//	{
//		break;
//	}
	memset(&t,0,sizeof(t));
	memset(&t3g,0,sizeof(t3g));
	if(net==3){
	memset(buf,0,sizeof(buf));
	recv_n(new_fd,(char*)&len,sizeof(len));//接收是否继续下载的请求
	recv_n(new_fd,buf,len);
	flag_end=atoi(buf);
	if(flag_end==0)
	{
		break;
	}
		if((t.len=read(fd,t3g.buf,sizeof(t3g.buf)))<=0)
			break;
		flag_end=0;
		send_n(new_fd,(char*)&t,4+t.len);
	}else{
		if(recv_size==0){
		n=f_size/1000;
		m=f_size%1000;
		length = f_size;
	char *p;
	p=(char *)mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p==(char *)-1)
	{
		perror("mmap");
		printf("映射失败%p\n",p);
		return ;
	}	
	printf("映射成功%p\n",p);
	long seek=f_size-send_size;
	printf("seek=%ld\n",seek);
	int k;
	for(k=0;k<n;k++)
	{
	memset(buf,0,sizeof(buf));
	recv_n(new_fd,(char*)&len,sizeof(len));//接收是否继续下载的请求
	recv_n(new_fd,buf,len);
	flag_end=atoi(buf);
	if(flag_end==0)
	{
		munmap(p,f_size);
		mmap_break=1;
		break;
	}
		memset(&t,0,sizeof(t));
		memcpy(t.buf,p+seek,1000);
		t.len=1000;
		seek=seek+1000;
		send_n(new_fd,(char*)&t,4+t.len);
	}
	if(mmap_break==0){
	memset(&t,0,sizeof(t));
	memcpy(t.buf,p+seek,m);
	t.len=m;
	send_n(new_fd,(char*)&t,8+t.len);
	munmap(p,length);
	break;}else{break;}
	}
	else{
	memset(buf,0,sizeof(buf));
	recv_n(new_fd,(char*)&len,sizeof(len));//接收是否继续下载的请求
	recv_n(new_fd,buf,len);
	flag_end=atoi(buf);
	if(flag_end==0)
	{
		break;
	}
		if((t.len=read(fd,t.buf,sizeof(t.buf)))<=0)
			break;
		flag_end=0;
		send_n(new_fd,(char*)&t,4+t.len);
	}
	}
}
t.len=0;
//发送空，标志结束
send(new_fd,&t,4+t.len,0);
//close(new_fd);
#if 0
if(flag_mmap==1)
{
	munmap(pmmap,f_size);
}else
#endif
{
	close(fd);
}
}

void recv_data(int sfd,char *filename)
{
	long len,l=0;
	char buf[1000]={0};
	int fd;
	//char itoa[32]={0};
	long cur_size=0;
	train t;
	memset(&t,0,sizeof(t));
	t.len=strlen(filename);
	strcpy(t.buf,filename);
	fd=open(filename,O_CREAT|O_WRONLY,0666);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	cur_size=file_stat(filename);
	lseek(fd,cur_size,SEEK_SET);
	printf("cur_size=%ld\n",cur_size);

	sprintf(t.buf,"%ld",cur_size);
	t.len=strlen(t.buf);
	send_n(sfd,(char*)&t,4+t.len); //发送当前文件大小

	recv_n(sfd,(char*)&len,sizeof(len));
	long big;
	recv_n(sfd,(char*)&big,sizeof(len)); //接受整体文件大小

	time_t now,check;
	time(&now);
	check=now;
	while(1){							//接受文件内容
		recv_n(sfd,(char *)&len,sizeof(len));
		l=l+len;
		if(len>0)
		{
			memset(buf,0,sizeof(buf));
			recv_n(sfd,buf,len);
			write(fd,buf,len);
			time(&now);
			if(now>=check+1)
			{
				printf("Now Data: %5.2f%s\r",(double)l*100/big,"%");
				fflush(stdout);
				check=now;
			}
		}else
		{
			printf("Now Data: %5.2f%s\n",(double)l*100/big,"%");
			printf("down load success\n");
			break;
		}
	}
	close(fd);
}

void collect_include(int sfd)
{
	char buf[1000]={0};
	int len;
	bzero(buf,sizeof(buf));
	recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,buf,len);    //接收需要下载的文件名
	FILE *fp=fopen("collection.txt","ab+");
	printf(" %s\n",buf);
	fprintf(fp,"%s",buf);
	bzero(buf,sizeof(buf)); //写入文件地址
	strcpy(buf,getcwd(NULL,0));
	fprintf(fp," %s",buf);
	bzero(buf,sizeof(buf));
	recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,buf,len);    //接收文件评论
	fprintf(fp," %s\n",buf);
	fclose(fp);
	word();
}
