///
/// @file    lru.c
/// @author  jesse(821228705@qq.com)
/// @date    2017-11-12 15:18:01
///
#include "func.h"
#include <iostream>
using std::cout;
using std::endl;


void que_insert(plru* phead,plru* ptail,int sfd)
{
	plru pnew=(plru)calloc(1,sizeof(lru));
	pnew->sfd=sfd;
	pnew->task_flag=1;
	if(NULL==*ptail)
	{
		*phead=pnew;
		*ptail=pnew;
	}else{
		(*ptail)->next=pnew;
		*ptail=pnew;
	}
}

int que_check(plru *phead,plru *ptail,plru *pcur) //获取空闲节点
{
	*pcur = *phead;
	if((*pcur)->task_flag==0){  //如果已经任务完成
	return 0;
	}
	if((*phead)==NULL)
	{
		return -1;
	}
	if((*phead)->next==NULL){
		return -1;
	}

	if((*pcur)->task_flag==1)  //任务还在进行中
	{
		while((*pcur)->next->task_flag!=0) //找到没有任务的
		{(*pcur)=(*pcur)->next;
		if((*pcur)->next==NULL){break;return -1;}	
		}
		return 0;
	}
	return -1;
}

int que_delete(plru *phead,plru *ptail)
{
	plru pcur = *phead;
	if(pcur->task_flag==0){  //如果已经任务完成
	*phead=pcur->next;
	if(*phead==NULL){
		*ptail=NULL;
	}
	return 0;
	}
	if((*phead)==NULL)
	{
		return -1;
	}
	if((*phead)->next==NULL){
		return -1;
	}

	if(pcur->task_flag==1)  //任务还在进行中
	{
		while(pcur->next->task_flag!=0) //找到没有任务的
		{pcur=pcur->next;
		if(pcur->next==NULL){break;return -1;}	
		}
		pcur->next=pcur->next->next;	
		return 0;
	}
	return -1;
}


void que_update(plru *phead,plru *ptail,int sfd)
{
	if((*phead)==NULL)
	{
		return;
	}
	if((*phead)->next==NULL){
		(*phead)->task_flag=0;
		return;
	}

	plru iterator = *phead;
	while(iterator->next->sfd!=sfd)
	{
		iterator=iterator->next;
	}
	if(iterator==*ptail)
	{
		return;	
	}
	que_insert(phead,ptail,sfd);
	iterator->next=iterator->next->next;	
	(*ptail)->task_flag=0;
}


// int main(){
//	plru head=NULL,tail=NULL;
//	for(int i=0;i<1;i++)
//	{
//		que_insert(&head,&tail,i);
//	}
//
//	plru iterator = head;
//	while(iterator!=NULL)
//	{
//		cout<< iterator->sfd<<endl;
//		iterator=iterator->next;
//	}
//	
//	que_update(&head,&tail,1);
//	que_delete(&head,&tail);
//	que_insert(&head,&tail,1);
//	
//	iterator = head;
//	while(iterator!=NULL)
//	{
//		cout<< iterator->sfd<<endl;
//		iterator=iterator->next;
//	}
//
//}
