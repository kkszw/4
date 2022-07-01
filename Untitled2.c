#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct PCB{
	char name[10];
	int flag;               //�ж��Ƿ񱻲��ҹ�
	int over;
	int proprity;           //���ȼ�
	int start_time;         //��ʼʱ��
	int arrival_time;		//����ʱ��
	int burst_time;		    //����ʱ��
	int finished_time;	    //��������ʱ��
	int runned_time;		//������ʱ��
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
    printf("��������̵����ƣ�");
    scanf("%s",mypcb->name);
    printf("���������%s�ĵ���ʱ�䣺",mypcb->name);
    scanf("%d",&mypcb->arrival_time);
    printf("���������%s�ķ���ʱ�䣺",mypcb->name);
    scanf("%d",&mypcb->burst_time);
    printf("���������%s�����ȼ���",mypcb->name);
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
    printf("����\tMax\tAllocation\tNeed\n");
    while(temp!=NULL)
    {
        printf("%s\t",temp->name);
        printf("%d,%d,%d\t",Max[i][0],Max[i][1],Max[i][2]);
        printf("%d,%d,%d\t\t",Allocation[i][0],Allocation[i][1],Allocation[i][2]);
        printf("%d,%d,%d\n",Need[i][0],Need[i][1],Need[i][2]);
        i++;
        temp=temp->next;
    }
    printf("��������Դ:%d,%d,%d\n",Available[0],Available[1],Available[2]);
}
void Banker()
{
    PCB *head=ready->next;
    int i=0,j=0;
    while(head!=NULL)
    {
        strcpy(process[i],head->name);
        printf("���������%s��MAX��",head->name);
        scanf("%d%d%d",&Max[i][0],&Max[i][1],&Max[i][2]);
        printf("���������%s��Allocation��",head->name);
        scanf("%d%d%d",&Allocation[i][0],&Allocation[i][1],&Allocation[i][2]);
        printf("���������%s��Need��",head->name);
        scanf("%d%d%d",&Need[i][0],&Need[i][1],&Need[i][2]);
        i++;
        head=head->next;
    }
    Show2();
    printf("����\tWork\tNeed\tAllocation\tWork+Allocation\n");
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
PCB *search_min(int end)//���ҽ������������ʱ����̵�
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
void reset()//�����н�����flag��over���¸�Ϊ0
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
            printf("search_min��������Ϊ��\n");
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
void changeci()//�������Ϊѭ������
{
    PCB *temp=ready->next;
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=ready->next;
    running=temp;
}
void rechange()//��ѭ������Ļ�
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
    printf("������ʱ��q:");
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
        if(temp->flag==0)//��һ�ο�ʼѭ��
        {
            temp->start_time=pre;
            temp->flag=1;
        }
        if(q+temp->runned_time>=temp->burst_time&&temp->over==0)//�����ʱ��Ƭ�н���
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
PCB *search_max(int end )//���ҽ������������ȼ���ߵ�
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
void Priority()//���ݺ����ҵ����ȼ�����
{
    int pre=0;
    PCB *temp,*head=ready->next;
    while(head!=NULL)
    {
        temp=search_max(pre);
        if(temp==NULL)
        {
            printf("search_max��������Ϊ��\n");
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
    printf("����\t����\t����\t��ʼ\t���\tT\tW\n");
    while(temp!=NULL)
    {
        t_sum+=temp->T;
        w_sum+=temp->W;
        sum++;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.1f\n",temp->name,temp->arrival_time,temp->burst_time,temp->start_time,temp->finished_time,temp->T,temp->W);
        temp=temp->next;
    }
    printf("Tƽ��:%.1f\n",t_sum/sum);
    printf("Wƽ��:%.1f\n",w_sum/sum);
}
void Menu()
{
    int a=1;
    while(a!=0)
    {
        printf("1.��������\n");
        printf("2.���м��㷨\n");
        printf("3.����Ȩ�����㷨\n");
        printf("4.�����ȷ���FCFS\n");
        printf("5.����ҵ����SJF\n");
        printf("6.ʱ��Ƭ��ת\n");
        printf("7.��ʾ������Ϣ\n");
        printf("8.��ʾ���м��㷨������Ϣ\n");
        printf("0.��������\n");
        printf("���������ѡ��:");
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
            printf("������ѡ��!\n");
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
