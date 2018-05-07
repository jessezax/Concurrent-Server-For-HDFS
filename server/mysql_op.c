///
/// @file    mysql_op.c
/// @author  jesse(821228705@qq.com)
/// @date    2017-09-14 14:57:40
///
#include "func.h"
#include <mysql/mysql.h>
int LoginUser(char* name,char*password1)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="jhp";
	char query[100]={0};
	sprintf(query,"%s%s%s","select * from client where name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))!=NULL)
			{	
				if(strcmp((const char*)name,(const char*)row[0]))
				{
					printf("login fail %s\n",row[0]);
				}
				if(!strcmp((const char*)password1,(const char*)row[1]))
				{
					printf("login success\n");
					return 0;
				}else
				{
					printf("login fail\n");
					return -1;
				}
			}
		}else
		{
			return -1;
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	printf("login fail %s\n",row[0]);
	return -1;
}


int RegisterUser(char* name,char* password1)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="jhp";
	char query[200];
	int t;
	//FILE *fp;
	//fp=fopen(argv[1],"r");
	//if(NULL==fp)
	//{
	//	perror("fopen");
	//	return -1;
	//}
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}

	//int personID=atoi(argv[2]);
	//while(memset(name,0,sizeof(name)),fgets(name,sizeof(name),fp)!=NULL)
	{
		//name[strlen(name)-1]='\0';
		memset(query,0,sizeof(query));
		sprintf(query,"%s%s%s%s%s","insert into client(name,password)values('",name,"',",password1,")");
		puts(query);
		t=mysql_query(conn,query);
		if(t)
		{
			printf("Error making query:%s\n",mysql_error(conn));
			return -1;
		}else{
			printf("insert success\n");
		}
	}			
	//fclose(fp);
	mysql_close(conn);
	return 0;
}


int DeleteUser(char* name)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="jhp";
	char query[100];
	sprintf(query,"%s%s%s","delete from client where name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("delete success\n");
	}
	mysql_close(conn);
	return 0;
}

