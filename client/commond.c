///
/// @file    commond.c
/// @author  jesse(821228705@qq.com)
/// @date    2017-09-05 10:20:00
///

#include "md5.h"
#include "func.h"
#include <time.h>
#include <sys/mman.h>
void loginmenu()
{
	printf("You have privilege to do those:\n");
	printf("1. cd     2.ls     3.pwd\n");
	printf("4. gets     5.put     6.rm\n");
	printf("7.collection_gets   8.collect_reserve\n");
}



char* fun_mmap(char *filename,long f_size)
{
	int fd;
	char *pmmap;
	fd=open(filename,O_RDWR);
	pmmap=(char *)mmap(NULL,f_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);
	return pmmap;
}
long file_stat(char *file)
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
	return f_size;
}
void ls(int sfd)
{
	char buf[1000]={0};
	train t;
	size_t len;
	t.len=2;
	strcpy(t.buf,"ls");
	send_n(sfd,(char*)&t,4+t.len);
	bzero(buf,sizeof(buf));
	while(1){
		recv_n(sfd,(char*)&len,sizeof(len));
		if(len>0)
		{
			memset(buf,0,sizeof(buf));
			recv_n(sfd,buf,len);
			printf("%s\n",buf);
		}else{
			break;
		}
	}
}
#if 1
void cd(int sfd,char *road)
{
	train t;
	t.len=2;
	strcpy(t.buf,"cd");
	send_n(sfd,(char*)&t,4+t.len);
	bzero(&t,sizeof(t));
	t.len=strlen(road);
	strcpy(t.buf,road);
	send_n(sfd,(char*)&t,4+t.len);
}
#endif
#if 1
void put(int sfd,char *filename)
{
	train t;
	t.len=3;
	strcpy(t.buf,"put");
	send_n(sfd,(char*)&t,4+t.len);
	send_data(sfd,filename);
}
#endif

void gets(int sfd,char *filename)
{		
	size_t len,l=0;
	char buf[1000]={0};
	char itoa[32]={0};
	int net;
	//int location;
	int fd;
	long cur_size=0;
	train t;
	printf("please choose the way you want to connect:\n");
	printf("1.WI-FI  2.4G  3.3G  \n");
	scanf("%d",&net);
	//printf("please choose  you want location:\n");
	//printf("1.China  2.Province\n");
	//scanf("%d",&location);
	memset(&t,0,sizeof(t));
	short file_flag;
	t.len=4;
	strcpy(t.buf,"gets");
	send_n(sfd,(char*)&t,4+t.len);
	memset(&t,0,sizeof(t));
	t.len=strlen(filename);
	strcpy(t.buf,filename);
	send_n(sfd,(char*)&t,4+t.len); //发送名字
	send_n(sfd,(char*)&net,sizeof(net));//发送网络类型
	//recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,(char*)&file_flag,2);  //是否有此文件
	if(file_flag==1)
	{
		printf("NO SUCH FILE\n");
		return;
	}
	fd=open(filename,O_CREAT|O_WRONLY,0666);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	int flag_md5;
	char *md5;
	
	cur_size=file_stat(filename);
	printf("cur_size=%ld\n",cur_size);
	sprintf(itoa,"%ld",cur_size);
	strcpy(t.buf,itoa);
	t.len=strlen(t.buf);
	send_n(sfd,(char*)&t,4+t.len);  //发送当前文件大小
	
	md5 = MD5_file(filename,cur_size);
	memset(&t,0,sizeof(t));
	t.len=strlen(md5);
	strcpy(t.buf,md5);
	printf("nowcmd5: %s\n",md5);
	send_n(sfd,(char*)&t,4+t.len);//发送MD5码
	recv_n(sfd,(char*)&flag_md5,sizeof(int));
	lseek(fd,cur_size,SEEK_SET);
	printf("flag_md5 = %d\n",flag_md5);
	if(flag_md5==0)
	{
		close(fd);
		unlink(filename);
		printf("unlink file\n");
		fd=open(filename,O_CREAT|O_RDWR,0666);
		if(-1==fd)
		{
			perror("open");
			return;
		}
	}
	long big;
	recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,(char*)&big,len); //收到还需要接受的文件大小
	time_t now,check,test,temp; 
	time(&now);
	check=now;
	test = now;
	temp = test;
#if 0
	char* cur_mmap;
	char* pmmap;
	int flag_mmap=0;
	if(big>102400)
	{
		ftruncate(fd,big+cur_size);//造文件空
		pmmap=fun_mmap(filename,cur_size+big);
		close(fd);
		/*return;*/
		cur_mmap=pmmap;
		cur_mmap=cur_mmap+cur_size;
		flag_mmap=1;
	}
#endif
	flag_end=1;
					FILE* fcount;
					long country;
				//	char buf1[1000]={0};
				//	long province;
					fcount = fopen("count.txt","rb+");
				//	memset(buf,0,sizeof(buf1));
					fscanf(fcount,"%ld",&country);
				//	country=atoi(buf);
					fclose(fcount);
	while(1){
		memset(&t,0,sizeof(t));
		if(flag_end==0){
		t.len=1;
		strcpy(t.buf,"0");
		send_n(sfd,(char*)&t,4+t.len); //发送不再下载的命令
		//close(sfd);
		break;
		}else{
		t.len=1;
		strcpy(t.buf,"1");
		send_n(sfd,(char*)&t,4+t.len);
		}

		recv_n(sfd,(char *)&len,sizeof(len));
		l=l+len;
		if(len>0)
		{
			memset(buf,0,sizeof(buf));
			recv_n(sfd,buf,len);
#if 0
			if(flag_mmap==1)
			{
				memcpy((void*)cur_mmap,(void*)buf,len);
				cur_mmap=cur_mmap+len;
			}else
			{
#endif
				write(fd,buf,len);
#if 0
			}
#endif
			time(&now);
			if(now>=check+1)
			{
				printf("Now Data: %5.2f%s\r",(double)l*100/big,"%");
				fflush(stdout);
	//				memset(buf,0,sizeof(buf));
	//				fscanf(fcount,"%s",cmoney);
	//				province = atoi(cmoney);
	//				if(province<=0&&country<=0)
	//				{
	//					printf("you should be equipped with enough money\n");
	//					fprintf(fcount,"%ld",country);
	//					fprintf(fcount," %ld",province);
	//					break;
	//				}
	//				if(location==1)
	//				{
	//					country=country-len;
	//					if(country<=0)
	//					{
	//						if(province>=0)
	//						{
	//							province=province-len;
	//						}
	//						if(province<0)
	//						{
	//						printf("you should be equipped with enough money\n");
	//					    fprintf(fcount,"%ld",country);
	//					    fprintf(fcount," %ld",province);
	//						break;
	//						}
	//					}
	//				}else{
	//					province=province-len;
	//					if(province<0)
	//					{
	//					 printf("you should be equipped with enough money\n");
	//					 fprintf(fcount,"%ld",country);
	//					 fprintf(fcount," %ld",province);
	//					 break;
	//					}
	//				}
			//	  fprintf(fcount,"%ld",country);
				//  fprintf(fcount," %ld",province);
			//	}
				check=now;
			}
		}else
		{
			printf("Now Data:   %s\n","100%");
			time(&test);
			printf("Now time:   %ld\n",test-temp);
			printf("down or update success\n");
			break;
		}
				if(net==2)
				{
					country=country-len;
					if(country<0)
					{
						printf("you should be equipped with enough money\n");
						break;
					}
				}
	}
#if 0
	if(flag_mmap==1)
	{
		munmap(pmmap,cur_size+big);
		flag_mmap=0;
	}else
#endif
		fcount = fopen("count.txt","wb+");
		fprintf(fcount,"%ld",country);
		fclose(fcount);
		close(fd);
}
#if 1
void rm(int sfd,char *filename)
{
	train t;
	memset(&t,0,sizeof(t));
	t.len=2;
	strcpy(t.buf,"rm");
	send_n(sfd,(char*)&t,4+t.len);
	memset(&t,0,sizeof(t));
	t.len=strlen(filename);
	strcpy(t.buf,filename);
	send_n(sfd,(char*)&t,4+t.len);
}
#endif 
#if 1
void pwd(int sfd)
{
	char buf[1000]={0};
	train t;
	size_t len;
	t.len=3;
	strcpy(t.buf,"pwd");
	send_n(sfd,(char*)&t,4+t.len);
	bzero(buf,sizeof(buf));
	recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,buf,len);
	printf("%s\n",buf);
}
#endif

void collect_show(int sfd)
{
	char file[]="collection.txt";
	gets(sfd,file);
	char buf[1000]={0};
	FILE *fp=fopen("collection.txt","rb+");
	bzero(buf,sizeof(buf));
	while(bzero(buf,sizeof(buf)),fscanf(fp,"%s",buf)!=EOF)
	{
		printf("%s",buf);
		bzero(buf,sizeof(buf));
		fscanf(fp,"%s",buf);
		printf(" %s",buf);
		bzero(buf,sizeof(buf));
		fscanf(fp,"%s",buf);
		printf(" %s\n",buf);
	}
	fclose(fp);
	//send_n(sfd,buf,strlen(buf));
}

void collect_reserve(int sfd,const char *filename)
{
	char buf[1000]={0};
	unlink("collection.txt");
	//int len;
	train t;
	memset(&t,0,sizeof(t));
	bzero(buf,sizeof(buf));
	t.len=15;
	strcpy(t.buf,"collect_reserve");
	send_n(sfd,(char*)&t,4+t.len);
	memset(&t,0,sizeof(t));
	bzero(buf,sizeof(buf));
	t.len=strlen(filename);
	strcpy(t.buf,filename);
	send_n(sfd,(char*)&t,4+t.len);  //发送需要收藏的文件名
	memset(&t,0,sizeof(t));
	bzero(buf,sizeof(buf));
	printf("please comment on the document:\n");
	scanf("%s",buf);
	t.len=strlen(buf);
	strcpy(t.buf,buf);
	send_n(sfd,(char*)&t,4+t.len);  //发送对文件的评论
//	bzero(buf,sizeof(buf));
//	recv_n(sfd,(char*)&len,sizeof(len));
//	recv_n(sfd,buf,len);
}

#if 0
void collect_down(int sfd,const char *filename)
{
	

}
#endif 
