 //MyAlgo
/*--Indicators--*/
/*Red=0;
Green=1;
Blue=2;
Black=3;
Null=-1;

terminal nos.
termN=n-1; //0--right;1--left;
Sort0=4;//i.e. 0+4;
Sort1=5;//i.e 1+4;
*/
/*--Variables--*/
#include<stdio.h>
#include<conio.h>
int indicator[4];//indicator[0] contains the terminal no. associated vid red color;this vil be used in scan();
int color[4];//color[0] contains the color no. associated vid terminal 0;
int term[4][2];//initialise to -2;
int visited[4];//initialise to 0; when terminal visted make it 1;
int sort[2];//initialise to -1;
int ct,adj,nt;
int armCount=2; //indicates no. of free arms
int sortCount=2;//indicated no. of free sort;
int arm[2];//initialise to -1;
int total=8;
int sorted=0;
int count=0;
int cost;
int visitedCount=0;
//whenever v find a blank space or box belonging to dat space total dec by 1;and whenever v place box in terminal sort++;
/*--functions--*/
void turnRight()	//turns the robo right
{
}
void turnLeft()	//turns the robo left
{
}
void turn()	//turn robo by 180 degree
{
}
void traverse(int a,int b)//traverses from current terminal to next terminal
{
	int i;
	if(count==25)
	{
		for(i=0;i<4;i++)
			printf("%d %d\n",term[i][0],term[i][1]);
		printf("Sort 0=%d Sort 1=%d\nArm 0=%d Arm 1=%d ArmC=%d Sorted=%d\n",sort[0],sort[1],arm[0],arm[1],armCount,sorted);
		getch();
		exit(1);
	}
	else count++;
	if((a==4||a==5)&&(b==4||b==5))
		cost+=4;
	else cost+=1;
	printf("Move from %d to %d\n",a,b);
}

int scan()//return the color no.
{
}

void setIndicatorAndColor()
{
	int i;
	for(i=0;i<4;i++)
	{
		printf("Enter color for terminal %d:\n",i);
		scanf("%d",&color[i]);
		term[i][0]=term[i][1]=-2;
		visited[i]=0;
	}
	for(i=0;i<4;i++)
		indicator[color[i]]=i;
	sort[0]=sort[1]=-1;
	arm[0]=arm[1]=-1;
}

int adjCount(ct)
{
	return (ct+2)%4;
}
void terminalCheck()
{
	turnRight();
	printf("Enter term[%d][%d]\n",ct,0);
	scanf("%d",&term[ct][0]);
	if(term[ct][0]==-1||term[ct][0]==color[ct])
		total--;
	turn();
	printf("Enter term[%d][%d]\n",ct,1);
	scanf("%d",&term[ct][1]);
	if(term[ct][1]==-1||term[ct][1]==color[ct])
		total--;
	visited[ct]=1;
	visitedCount++;
}



void pick(int armNo)
{
	armCount--;
	
}

void pickNode(int armNo,int side)
{
	printf("Arm %d picked %d from term[%d][%d]\n",armNo,term[ct][side],ct,side);
	arm[armNo]=term[ct][side];
	if(armNo!=side)
		turn();
	pick(armNo);
	if(armNo!=side)
		turn();
	term[ct][side]=-1;
}

void pickSort(int armNo,int sortNo)
{
	printf("Arm %d picked %d from sort[%d]\n",armNo,sort[sortNo],sortNo);
	arm[armNo]=sort[sortNo];
	if(armNo==0)
		turnLeft();
	else turnRight();
	pick(armNo);
	if(armNo==0)
		turnRight();
	else turnLeft();
	sort[sortNo]=-1;
}
void pickup()
{
	if((term[ct][0]!=color[ct]&& term[ct][0]!=-1) || (term[ct][1]!=color[ct] && term[ct][1]!=-1))
	{
		if(armCount==2)
		{
			
			if(term[ct][0]==color[adj] || term[ct][1]==color[adj])
				if(term[ct][0]==color[adj])
					pickNode(0,0);
				else	pickNode(1,1);
			else	if((term[ct][1]!=color[ct])&& term[ct][1]!=-1)
				{
					if(sort[ct%2]==-1 || ((term[adj][0]==color[adj]|| term[adj][0]==-1) || (term[adj][1]==color[adj] || term[adj][1]==-1)))
						pickNode(1,1);
				}
			else	if(term[ct][0]!=color[ct]&& term[ct][0]!=-1)
				{
					if(sort[ct%2]==-1 || ((term[adj][0]==color[adj]|| term[adj][0]==-1) || (term[adj][1]==color[adj] || term[adj][1	]==-1)))
						pickNode(0,0);
				}
			else	if(term[ct][0]!=color[ct]&& term[ct][0]!=-1)
					pickNode(0,0);

		}
		if(armCount==1)
		{
			int a1,a2;
			if(arm[0]==-1)
				a1=0;
			else a1=1;
			a2=(a1+1)%2;

			if(sort[ct%2]==-1)
			{
				if(term[ct][a1]==color[adj])
					pickNode(a1,a1);
				else if(term[ct][a2]==color[adj])
						pickNode(a1,a2);
				else if((term[ct][a1]!=-1&&term[ct][a1]!=color[ct])&&(visited[adj]==1&&(term[adj][0]!=color[adj]&&term[adj][1]!=color[adj])))
						pickNode(a1,a1);
				else if((term[ct][a2]!=-1&&term[ct][a2]!=color[ct])&&(visited[adj]==1&&(term[adj][0]!=color[adj]&&term[adj][1]!=color[adj])))
						pickNode(a1,a2);
			}
			else
			{
				if(term[ct][0]==color[adj] || term[ct][1]==color[adj])
				{
					if((visited[adj]==1 && (term[adj][0]==-1 || term[adj][1]==-1)))
							if(term[ct][a1]==color[adj])
								pickNode(a1,a1);
							else pickNode(a1,a2);
				}
				else {
					if(sort[ct%2]!=color[ct]&&sort[ct%2]!=color[adj])
						if(visited[adj]==1 &&((term[adj][0]!=color[ct]) &&(term[adj][1]!=color[ct])))
							if(term[ct][a1]!=-1 && term[ct][a1]!=color[ct])
								pickNode(a1,a1);
							else	if(term[ct][a2]!=-1 && term[ct][a2]!=color[ct])
									pickNode(a1,a2);
				}
			}
		}
	}
}

void drop(int armNo)
{
	armCount++;	
}
void nodeDrop(int armNo,int side)
{
	printf("Arm %d dropped %d on term[%d][%d]\n",armNo,arm[armNo],ct,side);
	term[ct][side]=arm[armNo];
	if(armNo!=side)
		turn();
	drop(armNo);
	if(armNo!=side)
		turn();
	arm[armNo]=-1;
	sorted++;
}
void sortDrop(int armNo,int sortNo)
{
	printf("Arm %d dropped %d on sort[%d]\n",armNo,arm[armNo],sortNo);
	sort[sortNo]=arm[armNo];
	if(armNo==0)
		turnLeft();
	else turnLeft();
	drop(armNo);
	if(armNo==0)
		turnRight();
	else turnLeft();
	arm[armNo]=-1;
}

void dropPickup(int a1)
{	
	int a2=(a1+1)%2;
	if(term[ct][a2]==color[ct] && term[ct][a2]!=color[adj])
	{
		if(term[ct][a1]!=color[ct] && term[ct][a1]!=-1)
			pickNode(a2,a1);
	}
	else		if(term[ct][a2]!=color[ct]&&term[ct][a2]!=-1)
				pickNode(a2,a2);
}
void canDrop()
{
	if(visited[ct]==0)
		terminalCheck();
	if(armCount==2 || (arm[0]!=color[ct]&&arm[1]!=color[ct]))
		return;
	else if(armCount==0)
		{
			if(arm[0]==color[ct] && term[ct][0]==-1)
				nodeDrop(0,0);
			else if(arm[1]==color[ct] && term[ct][1]==-1)
				nodeDrop(1,1);
			else if(arm[0]==color[ct] && term[ct][1]==-1)
				nodeDrop(0,1);
			else if(arm[1]==color[ct] && term[ct][0]==-1)
				nodeDrop(1,0);
		}
	
	if(arm[0]==color[ct] || arm[1]==color[ct])
	{
		if(term[ct][0]!=-1 && term[ct][1]!=-1)
		{	
			if(arm[0]==color[ct])
				dropPickup(0);
			else dropPickup(1);
		}
		if(arm[0]==color[ct] && term[ct][0]==-1)
			nodeDrop(0,0);
		else if(arm[0]==color[ct] && term[ct][1]==-1)
			nodeDrop(0,1);
		else if(arm[1]==color[ct] && term[ct][0]==-1)
			nodeDrop(1,0);
		else if(arm[1]==color[ct] && term[ct][1]==-1)
			nodeDrop(1,1);
	}
	else return;
}

void newSort()
{
	int t1,t2,a1,a2;
	if(arm[0]!=-1)
		a1=indicator[arm[0]];
	if(arm[1]!=-1)
		a2=indicator[arm[1]];
	
	t1=(ct+1)%2;
	t2=t1+2;
	if(arm[0]!=-1 ||arm[1]!=-1)	
	{	
		ct=a1;
		if(arm[0]!=-1&&(term[a1][0]==-1 || term[a1][1]==-1))
		{
			if(arm[1]==-1)
			{
				if(sort[t1]==color[t1] || sort[t1]==color[t2])
					pickSort(1,a1);
			}
		}
		else	if(arm[0]!=-1&&(sort[t1]==-1 && arm[1]!=-1))
					sortDrop(1,t1);
		else if(arm[1]!=-1)
		{
			ct=a2;	
			if(term[a2][0]==-1 ||term[a2][1]==-1)
			{
				if(arm[0]==-1)
				{
					if(sort[t1]==color[t1] && sort[t1]==color[t2])
						pickSort(1,t1);
				}
			}
			else	if(sort[t1]==-1&&arm[0]!=-1)
					sortDrop(1,t1);
		}
	}
	else
	{
		if((sort[t1]==color[t1] || sort[t1]==color[t2])&& sort[t1]!=-1)
			ct=indicator[sort[t1]];
		else if(sort[t1]!=-1)
		{
			pickSort(0,t1);
			ct=(t1+1)%2+4;
		}
		else if(visited[t1]==1 && (term[t1][0]==color[t1]||term[t1][0]==-1) && (term[t1][1]==color[t1]||term[t1][1]==-1))
				ct=t2;
		else ct=t1;
			
	}
	adj=adjCount(ct);
	traverse((t1+4)%6,ct);
}
void sortCheck()
{
	int term1=ct,a1,a2;
	if((sort[ct%2]==color[ct]||sort[ct%2]==color[adj]))
	{
		if(armCount>0)
		{	
			ct=indicator[sort[ct%2]];
			if(arm[0]==-1)
				pickSort(0,ct%2);
			else if(arm[1]==-1)
					pickSort(1,ct%2);
			if(armCount!=1 && term[ct][0]!=-1 && term[ct][1]!=-1 && sort[ct%2]==-1)
				if(arm[0]==color[ct])
					sortDrop(1,ct%2);
				else if(arm[0]==color[ct])
						sortDrop(0,ct%2);
		}
		else ct=adj;
	}
	else if(arm[0]!=color[adj]&&arm[1]!=color[adj])
		{	if((armCount>=1||sort[ct%2]==-1) && (visited[adj]==0 ||(visited[adj]==1&&((term[adj][0]!=color[adj]&&term[adj][0]!=-1)||(term[adj][1]!=-1&&term[adj][1]!=color[adj])))))
			{
				if(arm[0]!=-1 && sort[ct%2]==-1)
					sortDrop(0,ct%2);
				else if(arm[1]!=-1 && sort[ct%2]==-1)
						sortDrop(1,ct%2);
				ct=adj;
			}
			else
			{
				if(sort[ct%2]!=-1)
					if(arm[0]==-1)
						pickSort(0,ct%2);
					else if(arm[1]==-1)
							pickSort(1,ct%2);
				traverse(ct%2+4,(ct+1)%2+4);
				newSort();
				if(ct==4 || ct==5)
					newSort();
				return;
			}
		}	
	else
	{
		if(armCount!=1&&(term[adj][0]!=-1&&term[adj][1]!=-1)&&sort[ct%2]==-1)
			if(arm[0]==color[adj])
				sortDrop(1,ct%2);
			else sortDrop(0,ct%2);
		ct=adj;
	}
		adj=adjCount(ct);
		traverse(term1%2+4,ct);
}

void predict()
{
	if(term[0][0]==color[0]&& term[0][1]==color[0] && term[1][0]==color[1]&& term[1][1]==color[1] &&term[ct][0]==color[ct]&& term[ct][1]==color[ct]&&sort[0]==-1&&sort[1]==-1&&arm[0]==-1&&arm[1]==-1)
		sorted=total;
	
}
void main()
{
	int i;
	clrscr();
	setIndicatorAndColor();
	ct=0;adj=2;
	while(sorted<total)
	{
		canDrop();
		if(visitedCount==3)
			predict();
		if(sorted==total)
			break;
		pickup();
		traverse(ct,ct%2+4);
		sortCheck();
	}
	for(i=0;i<4;i++)
		printf("%d %d\n",term[i][0],term[i][1]);
	printf("Sort 0=%dSort 1=%d\nArm 0=%dArm 1=%d\n",sort[0],sort[1],arm[0],arm[1]);
	printf("Cost=%d\nSorted!!!!!",cost+7);
	getch();
}
