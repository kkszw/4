#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct PCB{
	char name[10];
	int flag;               //判断是否被查找过
	int over;
	int proprity;           //优先级
	int start_time;         //开始时间
	int arrival_time;		//到达时间
	int burst_time;		    //服务时间
	int finished_time;	    //结束运行时间
	int runned_time;		//已运行时间
	int T;
	float W;
	struct PCB *next;
}PCB;
struct PCB *ready,*blocked,*running,*finished;
int Available[10]={3,3,2},Max[10][10],Allocation[10][10],Need[10][10];
char process[10][10];
int allsum=0,flag[6]={0},top=0;
void add(struct PCB *head, struct PCB *process)
{
   struct PCB *tmp=head;
   while(tmp->next!=NULL)
       tmp=tmp->next;
   tmp->next=process;
   process->next=NULL;
}
void CreateProcess()
{
    PCB *mypcb=(PCB *)malloc(sizeof(PCB));
    mypcb->finished_time=0;
    mypcb->start_time=0;
    mypcb->runned_time=0;
    mypcb->proprity=0;
    mypcb->flag=0;
    mypcb->over=0;
    mypcb->T=0;
    mypcb->W=0;
    mypcb->next=NULL;
    printf("请输入进程的名称：");
    scanf("%s",mypcb->name);
    printf("请输入进程%s的到达时间：",mypcb->name);
    scanf("%d",&mypcb->arrival_time);
    printf("请输入进程%s的服务时间：",mypcb->name);
    scanf("%d",&mypcb->burst_time);
    printf("请输入进程%s的优先级：",mypcb->name);
    scanf("%d",&mypcb->proprity);
    add(ready,mypcb);
    allsum+=mypcb->burst_time;
}

int find()
{
    while(flag[5]!=1)
    {
        if(flag[top]!=1&&Available[0]-Need[top][0]>=0&&Available[1]-Need[top][1]>=0&&Available[2]-Need[top][2]>=0)
        {
            flag[top]=1;
            return top;
        }
        if(flag[0]==1&&flag[1]==1&&flag[2]==1&&flag[3]==1&&flag[4]==1)
            flag[5]=1;
        top++;
        top=top%5;
    }
    return -1;
}
void Show2()
{
    PCB *temp=ready->next;
    int i=0;
    printf("进程\tMax\tAllocation\tNeed\n");
    while(temp!=NULL)
    {
        printf("%s\t",temp->name);
        printf("%d,%d,%d\t",Max[i][0],Max[i][1],Max[i][2]);
        printf("%d,%d,%d\t\t",Allocation[i][0],Allocation[i][1],Allocation[i][2]);
        printf("%d,%d,%d\n",Need[i][0],Need[i][1],Need[i][2]);
        i++;
        temp=temp->next;
    }
    printf("可利用资源:%d,%d,%d\n",Available[0],Available[1],Available[2]);
}
void Banker()
{
    PCB *head=ready->next;
    int i=0,j=0;
    while(head!=NULL)
    {
        strcpy(process[i],head->name);
        printf("请输入进程%s的MAX：",head->name);
        scanf("%d%d%d",&Max[i][0],&Max[i][1],&Max[i][2]);
        printf("请输入进程%s的Allocation：",head->name);
        scanf("%d%d%d",&Allocation[i][0],&Allocation[i][1],&Allocation[i][2]);
        printf("请输入进程%s的Need：",head->name);
        scanf("%d%d%d",&Need[i][0],&Need[i][1],&Need[i][2]);
        i++;
        head=head->next;
    }
    Show2();
    printf("进程\tWork\tNeed\tAllocation\tWork+Allocation\n");
    for(i=0;i<5;i++)
    {
        j=find();
        printf("%s\t",process[j]);
        printf("%d,%d,%d\t",Available[0],Available[1],Available[2]);
        printf("%d,%d,%d\t",Need[j][0],Need[j][1],Need[j][2]);
        printf("%d,%d,%d\t\t",Allocation[j][0],Allocation[j][1],Allocation[j][2]);
        Available[0]+=Allocation[j][0];
        Available[1]+=Allocation[j][1];
        Available[2]+=Allocation[j][2];
        printf("%d,%d,%d\n",Available[0],Available[1],Available[2]);
    }
}
void FCFS()
{
    PCB *temp=ready;
    int pre;
    while(temp!=NULL)
    {
        if(temp->arrival_time==-1)
        {
            pre=0;
        }
        else
        {
            temp->start_time=pre;
            temp->finished_time=temp->start_time+temp->burst_time;
            pre=temp->finished_time;
            temp->T=temp->finished_time-temp->arrival_time;
            temp->W=temp->T/(temp->burst_time*1.0);
        }
        temp=temp->next;
    }
}
PCB *search_min(int end)//查找进程链表里服务时间最短的
{
    PCB *temp=ready->next,*goal=NULL;
    int min=999;
    while(temp!=NULL)
    {
        if(temp->flag==0&&temp->arrival_time<=end&&temp->burst_time<min)
        {
            min=temp->burst_time;
            goal=temp;
        }
        temp=temp->next;
    }
    goal->flag=1;
    return goal;
}
void reset()//将所有进程中flag和over重新赋为0
{
    PCB *temp=ready->next;
    while(temp!=NULL)
    {
        temp->flag=0;
        temp->over=0;
        temp=temp->next;
    }
}
void SJF()
{
    int pre=0;
    PCB *temp,*head=ready->next;
    while(head!=NULL)
    {
        temp=search_min(pre);
        if(temp==NULL)
        {
            printf("search_min函数返回为空\n");
            return;
        }
        temp->start_time=pre;
        temp->finished_time=temp->start_time+temp->burst_time;
        pre=temp->finished_time;
        temp->T=temp->finished_time-temp->arrival_time;
        temp->W=temp->T/(temp->burst_time*1.0);
        head=head->next;
    }
    reset();
}
void changeci()//将链表变为循环链表
{
    PCB *temp=ready->next;
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=ready->next;
    running=temp;
}
void rechange()//将循环链表改回
{
    PCB *temp=ready->next;
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=ready->next;
    running=temp;
}
void Time()//0_ABCDEABCD_9EAB_12CEA_15CE_17C_18
{
    int q=0;
    printf("请输入时间q:");
    scanf("%d",&q);
    PCB *temp=ready->next;
    changeci();
    int pre=0;
    while(pre<allsum)
    {
        if(temp->over==1)
        {
            temp=temp->next;
            continue;
        }
        if(temp->flag==0)//第一次开始循环
        {
            temp->start_time=pre;
            temp->flag=1;
        }
        if(q+temp->runned_time>=temp->burst_time&&temp->over==0)//在这个时间片中结束
        {
            temp->finished_time=pre+temp->burst_time-temp->runned_time;
            pre=temp->finished_time;
            temp->over=1;
        }
        else
        {
            temp->runned_time+=q;
            pre+=q;
        }
        temp=temp->next;
    }
    running->next=NULL;
    temp=ready->next;
    while(temp!=NULL)
    {
        temp->T=temp->finished_time-temp->arrival_time;
        temp->W=temp->T/(temp->burst_time*1.0);
        temp=temp->next;
    }
    reset();
}
PCB *search_max(int end )//查找进程链表里优先级最高的
{
    PCB *temp=ready->next,*goal=NULL;
    int max=-999;
    while(temp!=NULL)
    {
        if(temp->flag==0&&temp->arrival_time<=end&&temp->proprity>max)
        {
            max=temp->proprity;
            goal=temp;
        }
        temp=temp->next;
    }
    goal->flag=1;
    return goal;
}
void Priority()//根据函数找到优先级最大的
{
    int pre=0;
    PCB *temp,*head=ready->next;
    while(head!=NULL)
    {
        temp=search_max(pre);
        if(temp==NULL)
        {
            printf("search_max函数返回为空\n");
            return;
        }
        temp->start_time=pre;
        temp->finished_time=temp->start_time+temp->burst_time;
        pre=temp->finished_time;
        temp->T=temp->finished_time-temp->arrival_time;
        temp->W=temp->T/(temp->burst_time*1.0);
        head=head->next;
    }
}
void Show1()
{
    PCB *temp=ready->next;
    int sum=0;
    float t_sum=0,w_sum=0;
    printf("进程\t到达\t服务\t开始\t完成\tT\tW\n");
    while(temp!=NULL)
    {
        t_sum+=temp->T;
        w_sum+=temp->W;
        sum++;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.1f\n",temp->name,temp->arrival_time,temp->burst_time,temp->start_time,temp->finished_time,temp->T,temp->W);
        temp=temp->next;
    }
    printf("T平均:%.1f\n",t_sum/sum);
    printf("W平均:%.1f\n",w_sum/sum);
}
void Menu()
{
    int a=1;
    while(a!=0)
    {
        printf("1.创建进程\n");
        printf("2.银行家算法\n");
        printf("3.优先权调度算法\n");
        printf("4.先来先服务FCFS\n");
        printf("5.短作业优先SJF\n");
        printf("6.时间片轮转\n");
        printf("7.显示进程信息\n");
        printf("8.显示银行家算法进程信息\n");
        printf("0.结束程序\n");
        printf("请输入你的选择:");
        scanf("%d",&a);
        switch(a)
        {
        case 0:
            break;
        case 1:
            CreateProcess();
            break;
        case 2:
            Banker();
            break;
        case 3:
            Priority();
            break;
        case 4:
            FCFS();
            break;
        case 5:
            SJF();
            break;
        case 6:
            Time();
            break;
        case 7:
            Show1();
            break;
        case 8:
            Show2();
            break;
        default:
            printf("请重新选择!\n");
        }
    }
}

void main()
{
    ready=(PCB *)malloc(sizeof(PCB));
    ready->arrival_time=-1;
    ready->next=NULL;
    running=(PCB *)malloc(sizeof(PCB));
    running->next=NULL;
    Menu();
}
/*
1
A
0
4
1
1
B
1
3
2
1
C
2
5
3
1
D
3
2
4
1
E
4
4
5
7
4
7
5
7
6
1
7
3
7
2
7 5 3
0 1 0
7 4 3
3 2 2
2 0 0
1 2 2
9 0 2
3 0 2
6 0 0
2 2 2
2 1 1
0 1 1
4 3 3
0 0 2
4 3 1

*/
