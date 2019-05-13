/*
1. Two classes - AC and SLEEPER

2. While filling the form, the one filled first wil be given higher preference for berth.
    - Take care that you filled the correct berth preference for particular age group

3. Preference Order
    - Seats will be alloted in the coach, in which maximum number of people with preffered berth can be seated.
    - remaining people will be seated in the same coach(as much seats are available) nearest to the first person allocated in that coach
    - remaining people will be seated in the nearest coach where seats are available( if preference is under age group then they will be given preference for lower berth in these coaches)

4. Classes -
    L : Lower;
    M : Middle;
    U : Upper;
    A : Side_Upper;
    B : Side_Lower

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#define MAX 5



typedef enum status{false,true} bool;

/*
typedef struct btree_tag
{
	int keys[MAX];
	struct btree_tag* a[MAX+1];
	int c;
	bool isleaf;
}btree;
*/
typedef struct passenger_tag
{
    int age,BerthPreference,id;
    char name[50];
    struct passenger_tag *next;
}passenger;

typedef struct ticket_tag
{
	int ticket_id;
	passenger *lower,*middle,*upper,*supper,*slower,*rem,*waiting;
}ticket;

typedef struct ptree_tag
{
    ticket keys[MAX];
    struct ptree_tag* a[MAX+1];
    int c;
    bool isleaf;
}ptree;

typedef struct seat_tag
{
    bool booked;
    char type;
    int seatNo;
    passenger *p;
}seat;

typedef struct coach_tag
{
    char id[3];
    seat seats[40];
    char class[4];
    int c1,c2,c3,c4,c5;
    passenger *pinfo;
}coach;

typedef struct ctree_tag
{
	coach keys[MAX];
	struct ctree_tag* a[MAX+1];
	int c;
	bool isleaf;
}ctree;

typedef struct train_tag
{
    int id,nsl,nac;   //n is number of coaches;
    char name[50];
    ctree *slroot;
    ctree *acroot;
    ptree *troot;
    passenger *sllist;
    passenger *aclist;
}train;

int tick_cnt=0,min=40;
coach *flagptr;

ctree* insert(ctree *root,coach ct);
ctree* split(ctree *root,ctree *ptr);

ptree* insert_tik(ptree *root,ticket ct);
ptree* split_tik(ptree *root,ptree *ptr);

void initialise(train **t, int n)
{
    for(int i=0;i<n;i++)
    {
        t[i]=NULL;
    }
}


ctree * intialise_ctree(ctree *node)
{
	node->c=0;
	node->isleaf=true;
	int i=0,j=0,y;
	for(j=0;j<MAX;j++)
	{
		node->a[j]=NULL;

		for(i=0;i<40;i++)
		{
		        seat st=(node->keys[j]).seats[i];
        		(node->keys[j]).seats[i].seatNo=i;
	        	(node->keys[j]).seats[i].booked=0;
	        	y=(i+1)%8;
	        	if(y==1 || y==4)
	        	{
	        	    (node->keys[j]).seats[i].type='L';
	        	}
	        	if(y==2 || y==5)
	        	{
	        	    (node->keys[j]).seats[i].type='M';
	        	}
	        	if(y==3 || y==6)
	        	{
	        	    (node->keys[j]).seats[i].type='U';
	        	}
	        	if(y==7) //side_upper
	        	{
	        	    (node->keys[j]).seats[i].type='A';
	        	}
	        	if(y==0 && i<25) //side_lower
	        	{
	        	    (node->keys[j]).seats[i].type='B';
	        	}
	        	else if(y==0 && i>25) //side_lower as RAC
	        	{
	        	    (node->keys[j]).seats[i].type='R';
	        	}
		}
		(node->keys[j]).id[0]='/0';
		(node->keys[j]).class[0]='/0';
		(node->keys[j]).c1=(node->keys[j]).c2=(node->keys[j]).c3=10;
		(node->keys[j]).c4=5;
		(node->keys[j]).c5=3;

		(node->keys[j]).pinfo=NULL;
	}
	//(node->keys[0]).c1=(node->keys[0]).c2=(node->keys[0]).c3=1;
	//	(node->keys[0]).c4=1;
	//	(node->keys[0]).c5=1;
	node->a[j]=NULL;
	return node;
}

ptree * intialise_ptree(ptree *node)
{
	node->c=0;
	node->isleaf=true;
	int i=0;
	for(i=0;i<MAX;i++)
	{
		node->a[i]=NULL;
		(node->keys[i]).lower=(node->keys[i]).middle=(node->keys[i]).upper=(node->keys[i]).supper=(node->keys[i]).slower=NULL;
		(node->keys[i]).ticket_id=0;
	}
	node->a[i]=NULL;
	return node;
}

void addTrain(train *t[])
{
    int id_no;
    printf("Enter Train Number :");
    scanf("%d",&id_no);
    if(t[id_no-1]==NULL)
    {
        t[id_no-1]=(train*)malloc(sizeof(train));
        t[id_no-1]->id=id_no;

        char name[50];
        printf("Enter Train Name :");
        scanf("%s",name);
        strcpy(t[id_no-1]->name,name);

        (t[id_no-1]->slroot)=NULL;
        (t[id_no-1]->acroot)=NULL;
        (t[id_no-1]->troot)=NULL;
        (t[id_no-1]->nsl)=0;
        (t[id_no-1]->nac)=0;
        (t[id_no-1]->sllist)=NULL;
        (t[id_no-1]->aclist)=NULL;

        printf("Train %s added SUCCESFULLY!!!\n\n\n",name);
    }
    else
    {
        printf("Error in adding train\n\n\n");  //if id_no exceeds max number of trains
    }
}

int id_to_key(char a[])
{
	int i=0;
	i=(a[1]-'0')*10+(a[2]-'0');
	return i;
}

ctree * makecoachnode()
{
	ctree *cnptr;
	cnptr=(ctree *)malloc(sizeof(ctree));
	cnptr=intialise_ctree(cnptr);
	return cnptr;

}

ptree * maketicketnode()
{
	ptree *pnptr;
	pnptr=(ptree *)malloc(sizeof(ptree));
	pnptr=intialise_ptree(pnptr);
	return pnptr;

}

ctree* search_pos_coach(ctree *root,char cid,int id,int *d)
{

	ctree *ptr;
	ptr=root;
	int cnt,i,flag=0;
	ctree *retval;
	*d=1;
	if(ptr->isleaf==true)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if(id_to_key((ptr->keys[i]).id)>=id)
			{

				flag=1;
				if(id_to_key((ptr->keys[i]).id)==id)
				{
					*d=0;
					printf("element found at leaf\n");
				}
			}
		}
		retval=ptr;
	}
	if(ptr->isleaf==false)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if(id_to_key((ptr->keys[i]).id)>=id)
			{
				flag=1;
				if(id_to_key((ptr->keys[i]).id)==id)
				{
					*d=0;
					retval=ptr;
					printf("element found \n");
				}
			}
		}
		if(flag==1) i--;
		if(*d==1)
		{
			retval=search_pos_coach(ptr->a[i],cid,id,d);
		}

	}
	return retval;
}


void add_array(coach keys[],char cid,int id,int c)
{
	int i=c-1;
	int j;
	char SL[3]="SL",AC[2]="AC";
	while(i>=0 && id_to_key((keys[i]).id)>id)
	{
		keys[i+1]=keys[i];
		i--;
	}
	keys[i+1].id[0]=cid;
	keys[i+1].id[1]=(id/10)+'0';
	keys[i+1].id[2]=(id%10)+'0';
	printf("Added %s\n",keys[i+1].id);
	if(cid=='S') strcpy(keys[i+1].class,SL);
	if(cid=='A') strcpy(keys[i+1].class,AC);
	keys[i+1].pinfo=NULL;
}


void add_array_ins(coach keys[],coach ct,int c)
{
	int i=c-1;
	int j;
	while(i>=0 && id_to_key(keys[i].id)>id_to_key(ct.id))
	{
		keys[i+1]=keys[i];
		i--;
	}
	keys[i+1]=ct;
}


ctree* findparent(ctree *root,coach *ptr)
{
	int flag=0,i=0;
	ctree *retval=NULL;
	if(root->isleaf==false)
	{
		for(i=0 ; i<root->c+1 && flag==0; )
		{

			if(root->a[i]!=ptr)
			{
				retval=findparent(root->a[i],ptr);
				i++;

			}
			else
			{
				flag=1;
				retval=root;
			}
		}
	}
	return retval;
}

ctree* add_coach(ctree *cptr,char cid,int id)
{
	ctree *cnptr,*ptr;
	int d=1;
	char SL[3]="SL",AC[2]="AC";
	if(cptr==NULL)
	{
		cptr=makecoachnode();
		(cptr->keys[cptr->c]).id[0]=cid;
		(cptr->keys[cptr->c]).id[1]=(id/10)+'0';
		(cptr->keys[cptr->c]).id[2]=(id%10)+'0';
		printf("Added %s\n",(cptr->keys[cptr->c]).id);
		if(cid=='S') strcpy((cptr->keys[cptr->c]).class,SL);
	    	if(cid=='A') strcpy((cptr->keys[cptr->c]).class,AC);
		(cptr->c)++;
	}
	else
	{
		ptr=search_pos_coach(cptr,cid,id,&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array(ptr->keys,cid,id,ptr->c);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array(ptr->keys,cid,id,ptr->c);
				ptr->c++;
				cptr=split(cptr,ptr);
			}
		}
	}
	return cptr;
}

void display(ctree *ptr, int blanks)
{
  if (ptr)
    {
      int i;
      for (i = 1; i <= blanks; i++)
	  printf (" ");
      for (i = 0; i < (ptr->c); i++)
      printf ("%s ", ptr->keys[i].id);
      printf ("\n");
      for (i = 0; i <= (ptr->c); i++)
      display (ptr->a[i], blanks + 10);
    }				/*End of if */
}


void addCoach(train *t[], int train_id)
{
    coach *cptr;
    char class[3],id[3],SL[3]="SL",AC[3]="AC";
    int y;

    printf("Class of the coach :");
    scanf("%s",class);

    if(strcmp(SL,class)==0)
    {
        (t[train_id-1]->nsl)+=1;
        t[train_id-1]->slroot=add_coach(t[train_id-1]->slroot,SL[0],t[train_id-1]->nsl);
    }
    else
    {
        (t[train_id-1]->nac)+=1;
        t[train_id-1]->acroot=add_coach(t[train_id-1]->acroot,AC[0],t[train_id-1]->nac);
    }

    display(t[train_id-1]->slroot,0);
    printf("Coach added SUCCESFULLY to train %s \n\n",t[train_id-1]->name);
}

void traverse_ctree(ctree *node, int pref[],int *d)
{
	int i,c,c1,c2,c3,c4,c5,flag;
	ctree *retval;

	if(node->isleaf==true && *d!=1)
	{
		c=node->c;
		for(i=0;i<c && *d!=1;i++)
		{
			c1=pref[0]-node->keys[i].c1;
			if(c1<0) c1=0;
			c2=pref[1]-node->keys[i].c2;
			if(c2<0) c2=0;
			c3=pref[2]-node->keys[i].c3;
			if(c3<0) c3=0;
			c4=pref[3]-node->keys[i].c4;
			if(c4<0) c4=0;
			c5=pref[4]-node->keys[i].c5;
			if(c5<0) c5=0;
			flag=c1+c2+c3+c4+c5;
			if(flag==0)
			{
				min=flag;
				*d=1;
				flagptr=&(node->keys[i]);

			}
			else if(flag<min)
			{
				min=flag;
				flagptr=&(node->keys[i]);

			}
		}
	}
	else if(*d!=1)
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			traverse_ctree(node->a[i],pref,d);
			if(i < c)
			{
				c1=pref[0]-node->keys[i].c1;
				if(c1<0) c1=0;
				c2=pref[1]-node->keys[i].c2;
				if(c2<0) c2=0;
				c3=pref[2]-node->keys[i].c3;
				if(c3<0) c3=0;
				c4=pref[3]-node->keys[i].c4;
				if(c4<0) c4=0;
				c5=pref[4]-node->keys[i].c5;
				if(c5<0) c5=0;
				flag=c1+c2+c3+c4+c5;
				if(flag==0)
				{
					min=flag;
					*d=1;
					flagptr=&(node->keys[i]);

				}
				else if(flag<min)
				{
					min=flag;
					flagptr=&(node->keys[i]);

				}
			}
		}
	}
}


passenger* link(passenger *lower,passenger *middle,passenger *upper,passenger *supper,passenger *slower,passenger *lptr,passenger *mptr,passenger *uptr,passenger *suptr,passenger *slptr)
{
    passenger *result;
    int count=0;
    result=(passenger*)malloc(sizeof(passenger));
    if(lower!=NULL)
    {
        result=lower;
        if(middle!=NULL)
        {
            lptr->next=middle;
            if(upper!=NULL)
            {
                mptr->next=upper;
                if(supper!=NULL)
                {
                    uptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        uptr->next=slower;
                    }
                }
            }
            else
            {
                if(supper!=NULL)
                {
                    mptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        mptr->next=slower;
                    }
                }
            }
        }
        else
        {
            if(upper!=NULL)
            {
                lptr->next=upper;
                if(supper!=NULL)
                {
                    uptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        uptr->next=slower;
                    }
                }
            }
            else
            {
                if(supper!=NULL)
                {
                    lptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        lptr->next=slower;
                    }
                }
            }
        }
    }
    else
    {
        if(middle!=NULL)
        {
            result=middle;
            if(upper!=NULL)
            {
                mptr->next=upper;
                if(supper!=NULL)
                {
                    uptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        uptr->next=slower;
                    }
                }
            }
            else
            {
                if(supper!=NULL)
                {
                    mptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        mptr->next=slower;
                    }
                }
            }
        }
        else
        {
            if(upper!=NULL)
            {
                result=upper;
                if(supper!=NULL)
                {
                    uptr->next=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        uptr->next=slower;
                    }
                }
            }
            else
            {
                if(supper!=NULL)
                {
                    result=supper;
                    if(slower!=NULL)
                    {
                        suptr->next=slower;
                    }
                }
                else
                {
                    if(slower!=NULL)
                    {
                        result=slower;
                    }
                    else
                    {
                        result=NULL;
                    }
                }
            }
        }
    }

    return result;
}

passenger *rem_seats(ctree *node, passenger *remaining, int *flag)
{
	int i,c,x,te;
	ctree *retval;
	passenger *ptr;

	if(node->isleaf==true && *flag!=0)
	{
		c=node->c;
		for(i=0;i<c && *flag!=0;i++)
		{

			x=0;
			while(x<40 && *flag!=0)
            {
                	    if(x<40 && !((node->keys[i]).seats[x]).booked && x!=31 && x!=39 ) // ((node->keys[i]).seats[x]).type!='R')
                	    {
                	        te=(x+1)%8;
                	        if(te==1 || te==4) (node->keys[i]).c1--;
                	        if(te==2 || te==5) (node->keys[i]).c2--;
                	        if(te==3 || te==6) (node->keys[i]).c3--;
                	        if(te==7) (node->keys[i]).c4--;
                	        if(te==0) (node->keys[i]).c5--;
                	        *flag=*flag-1;
                	        ((node->keys[i]).seats[x]).booked=1;

                	        ptr=remaining;
                	        if(remaining!=NULL) remaining=remaining->next;

                	        ((node->keys[i]).seats[x]).p=ptr;
                	        free(ptr);
                	    }
                	    x++;
			}
		}
		return remaining;
	}
	else if(*flag!=0)
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			remaining=rem_seats(node->a[i],remaining,flag);
			if(i < c && *flag!=0)
			{
				x=0;
				while(x<40 && *flag!=0)
            			{
                	    	    if(x<40 && !((node->keys[i]).seats[x]).booked && x!=31 && x!=39 ) // ((node->keys[i]).seats[x]).type!='R')
                	    	    {
                	    	        te=(x+1)%8;
                	    	        if(te==1 || te==4) (node->keys[i]).c1--;
                	    	        if(te==2 || te==5) (node->keys[i]).c2--;
                	    	        if(te==3 || te==6) (node->keys[i]).c3--;
                	    	        if(te==7) (node->keys[i]).c4--;
                	    	        if(te==0) (node->keys[i]).c5--;
                	    	        *flag=*flag-1;;
                	    	        ((node->keys[i]).seats[x]).booked=1;

                	    	        ptr=remaining;
                	    	        if(remaining!=NULL) remaining=remaining->next;

                	    	        ((node->keys[i]).seats[x]).p=ptr;
                	    	        free(ptr);
                	    	    }
                	    	    x++;
				}
			}
		}
		return remaining;
	}
}

ptree* search_pos_ticket(ptree *root,int id,int *d)
{

	ptree *ptr;
	ptr=root;
	int cnt,i,flag=0;
	ptree *retval;
	*d=1;
	if(ptr->isleaf==true)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if((ptr->keys[i]).ticket_id>=id)
			{
				flag=1;
				if((ptr->keys[i]).ticket_id==id)
				{
					*d=0;
					printf("element found at leaf\n");
				}
			}
		}
		retval=ptr;
	}
	if(ptr->isleaf==false)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if((ptr->keys[i]).ticket_id>=id)
			{
				flag=1;
				if((ptr->keys[i]).ticket_id==id)
				{
					*d=0;
					retval=ptr;
					printf("element found \n");
				}
			}
		}
		if(flag==1) i--;
		if(*d==1)
		{
			retval=search_pos_ticket(ptr->a[i],id,d);
		}

	}
	return retval;
}

void add_array_tik(ticket keys[],int id,int c,ticket tik)
{
	int i=c-1;
	int j;
	while(i>=0 && (keys[i]).ticket_id>id)
	{
		keys[i+1]=keys[i];
		i--;
	}
	keys[i+1].ticket_id=id;
	keys[i+1]=tik;
	printf("Added ticket of id %d\n",keys[i+1].ticket_id);
}


void add_array_ins_tik(ticket keys[],ticket ct,int c)
{
	int i=c-1;
	int j;
	while(i>=0 && (keys[i].ticket_id)>ct.ticket_id)
	{
		keys[i+1]=keys[i];
		i--;
	}
	keys[i+1]=ct;
}


ptree* findparent_tik(ptree *root,ticket *ptr)
{
	int flag=0,i=0;
	ptree *retval=NULL;
	if(root->isleaf==false)
	{
		for(i=0 ; i<root->c+1 && flag==0; )
		{

			if(root->a[i]!=ptr)
			{
				retval=findparent_tik(root->a[i],ptr);
				i++;

			}
			else
			{
				flag=1;
				retval=root;
			}
		}
	}
	return retval;
}

ptree* add_ticket(ptree *tptr,int id,ticket tik)
{
	ptree *pnptr,*ptr;
	int d;
	if(tptr==NULL)
	{
		tptr=maketicketnode();
		(tptr->keys[tptr->c]).ticket_id=id;
		(tptr->keys[tptr->c])=tik;

		printf("Added ticket of id %d\n",(tptr->keys[tptr->c]).ticket_id);
		(tptr->c)++;
	}
	else
	{
		ptr=search_pos_ticket(tptr,id,&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array_tik(ptr->keys,id,ptr->c,tik);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array_tik(ptr->keys,id,ptr->c,tik);
				ptr->c++;
				tptr=split_tik(tptr,ptr);
			}
		}
	}
	return tptr;
}

void display_tik(ptree *ptr, int blanks)
{
  if (ptr)
    {
      int i;
      for (i = 1; i <= blanks; i++)
	  printf (" ");
      for (i = 0; i < (ptr->c); i++)
      printf ("%d ", ptr->keys[i].ticket_id);
      printf ("\n");
      for (i = 0; i <= (ptr->c); i++)
      display (ptr->a[i], blanks + 10);
    }				/*End of if */
}


void Form(train *t[])
{
    int TrainNo,i,p_id,pref[5]={0,0,0,0,0},j,k,min=1000,p,d=0;
    long int PhoneNo,l;
    char address[100],class[3],SL[3]="SL",AC[3]="AC";
    coach *c_max;
    passenger *lower=NULL,*middle=NULL,*upper=NULL,*supper=NULL,*slower=NULL,*ptr,*lptr,*mptr,*uptr,*suptr,*slptr,*remaining;


    FILE *in;
    char *filename="tickets";
    in=fopen(filename,"a");
    if (in == NULL){
        printf("Could not open file %s\n\n",filename);
    }

    FILE *fp;
    char *file="test";

    char name[50];
    int age,bpref[i],prefcount=0;

    int choice =1;
    printf("1 To give input\n2 T use default input");
    scanf("%d",&choice);
    if(choice==2)
    {

    fp=fopen(file,"r");
    if (fp == NULL){
        printf("Could not open file %s\nGive input using console\n",file);
        return 1;
    }
    printf("Fill The Form :\n");

    printf("Enter Train Number :"); scanf("%d",&TrainNo);
    printf("Preffered Class :"); scanf("%s",class);

    printf("\n\n");
    printf("Phone Number :"); fscanf(fp,"%ld",&PhoneNo);
    printf("Address :"); fscanf(fp,"%s",address);

    l=PhoneNo;
    p_id=++tick_cnt;

    printf("\nPassenger ID is : %d\n",p_id);
    printf("Note it down for future reference\n");

    printf("Number of Passengers :");
    fscanf(fp,"%d",&i);

    fflush(stdin);
    fprintf(in,"TICKET INFORMATION :\n");
    fflush(stdin);
    fprintf(in,"\tName\t\tAge\t\tSeat\t\tCoach\n");

    for(j=0;j<i;j++)
    {
        printf("Passenger %d :\n",j+1);
        printf("Name :"); fscanf(fp,"%s",name);
        printf("Age :"); fscanf(fp,"%d",&age);
        printf("Birth Preference :\n");
        printf("L:1, M:2, U:3, SU:4, SL:5 :"); fscanf(fp,"%d",&k);
        pref[k-1]+=1;
        if((age<=15 || age>=60) && k==1)
        {
            prefcount++;
        }
        printf("\n");

        ptr=(passenger*)malloc(sizeof(passenger));
        strcpy(ptr->name,name);
        ptr->age=age;
        ptr->BerthPreference=k;
        ptr->id=p_id;

        ptr->next=NULL;

        if(k==1)
        {
            if(lower==NULL)
            {
                lower=ptr;
                lptr=lower;
            }
            else
            {
                lptr->next=ptr;
                lptr=ptr;
            }
        }
        if(k==2)
        {
            if(middle==NULL)
            {
                middle=ptr;
                mptr=middle;
            }
            else
            {
                mptr->next=ptr;
                mptr=ptr;
            }
        }
        if(k==3)
        {
            if(upper==NULL)
            {
                upper=ptr;
                uptr=upper;
            }
            else
            {
                uptr->next=ptr;
                uptr=ptr;
            }
        }
        if(k==4)
        {
            if(supper==NULL)
            {
                supper=ptr;
                suptr=supper;
            }
            else
            {
                suptr->next=ptr;
                suptr=ptr;
            }
        }
        if(k==5)
        {
            if(slower==NULL)
            {
                slower=ptr;
                slptr=slower;
            }
            else
            {
                slptr->next=ptr;
                slptr=ptr;
            }
        }
    }
    printf("\n");
    }
    else if(choice ==1)
    {

    fp=fopen(file,"a");
    if (fp == NULL){
        printf("Could not open file %s\nGive input using console\n",file);
        return 1;
    }
     printf("Fill The Form :\n");

    printf("Enter Train Number :"); scanf("%d",&TrainNo);
    printf("Preffered Class :"); scanf("%s",class);

    printf("\n\n");
    printf("Phone Number :"); scanf("%ld",&PhoneNo);
    fflush(stdin);
    fprintf(fp,"%d\n",PhoneNo);
    printf("Address :"); scanf("%s",address);
    fflush(stdin);
    fprintf(fp,"%s\n",address);

    l=PhoneNo;
    p_id=++tick_cnt;

    printf("\nPassenger ID is : %d\n",p_id);
    printf("Note it down for future reference\n");

    printf("Number of Passengers :");
    scanf("%d",&i);
    fflush(stdin);
    fprintf(fp,"%d\n",i);

    fflush(stdin);
    fprintf(in,"TICKET INFORMATION :\n");
    fflush(stdin);
    fprintf(in,"\tName\t\tAge\t\tSeat\t\tCoach\n");

    for(j=0;j<i;j++)
    {
        printf("Passenger %d :\n",j+1);
        printf("Name :"); scanf("%s",name);
        fflush(stdin);
    	fprintf(fp,"%s\n",name);
        printf("Age :"); scanf("%d",&age);
        fflush(stdin);
    	fprintf(fp,"%d\n",age);
        printf("Birth Preference :\n");
        printf("L:1, M:2, U:3, SU:4, SL:5 :"); scanf("%d",&k);
        fflush(stdin);
    	fprintf(fp,"%d\n",k);
        pref[k-1]+=1;
        if((age<=15 || age>=60) && k==1)
        {
            prefcount++;
        }
        printf("\n");

        ptr=(passenger*)malloc(sizeof(passenger));
        strcpy(ptr->name,name);
        ptr->age=age;
        ptr->BerthPreference=k;
        ptr->id=p_id;

        ptr->next=NULL;

        if(k==1)
        {
            if(lower==NULL)
            {
                lower=ptr;
                lptr=lower;
            }
            else
            {
                lptr->next=ptr;
                lptr=ptr;
            }
        }
        if(k==2)
        {
            if(middle==NULL)
            {
                middle=ptr;
                mptr=middle;
            }
            else
            {
                mptr->next=ptr;
                mptr=ptr;
            }
        }
        if(k==3)
        {
            if(upper==NULL)
            {
                upper=ptr;
                uptr=upper;
            }
            else
            {
                uptr->next=ptr;
                uptr=ptr;
            }
        }
        if(k==4)
        {
            if(supper==NULL)
            {
                supper=ptr;
                suptr=supper;
            }
            else
            {
                suptr->next=ptr;
                suptr=ptr;
            }
        }
        if(k==5)
        {
            if(slower==NULL)
            {
                slower=ptr;
                slptr=slower;
            }
            else
            {
                slptr->next=ptr;
                slptr=ptr;
            }
        }
    }
    printf("\n");
    }



    int c0,c1,c2,c3,c4,c5,x,y,flag1,flag,count,fx,te;
    passenger *p1,*p2,*p3,*p4,*p5,*p6,*p7;
    ctree ct;


    if(strcmp(class,SL)==0) traverse_ctree(t[TrainNo-1]->slroot,pref,&d);
    else if(strcmp(class,AC)==0) traverse_ctree(t[TrainNo-1]->acroot,pref,&d);
    c_max=flagptr;
    //printf("%s",c_max->id);
    ticket tik;
    tik.lower=tik.middle=tik.upper=tik.supper=tik.slower=tik.rem=tik.waiting=NULL;
    tik.ticket_id=p_id;


    if(d==1)                      //flag1=0 means all can be seated in same coach with thier preffered berths.
    {
    	tik.lower=lower;
    	tik.middle=middle;
    	tik.upper=upper;
    	tik.supper=supper;
    	tik.slower=slower;
        c1=pref[0];
        c2=pref[1];
        c3=pref[2];
        c4=pref[3];
        c5=pref[4];
        for(x=0;x<40;x++)
        {
            y=(x+1)%8;
            if((y==1 || y==4) && !(c_max->seats[x]).booked && c1!=0)
            {
                (c_max->seats[x]).booked=1;
                c1--;
                c_max->c1--;

                ptr=lower;
                if(lower!=NULL) lower=lower->next;

                (c_max->seats[x]).p=ptr;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==2 || y==5) && !(c_max->seats[x]).booked && c2!=0)
            {
                (c_max->seats[x]).booked=1;
                c2--;
                c_max->c2--;

                ptr=middle;
                if(middle!=NULL) middle=middle->next;

                (c_max->seats[x]).p=ptr;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==3 || y==6) && !(c_max->seats[x]).booked && c3!=0)
            {
                (c_max->seats[x]).booked=1;
                c3--;
                c_max->c3--;

                ptr=upper;
                if(upper!=NULL) upper=upper->next;

                (c_max->seats[x]).p=ptr;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==7) && !(c_max->seats[x]).booked && c4!=0)
            {
                (c_max->seats[x]).booked=1;
                c4--;
                c_max->c4--;

                ptr=supper;
                if(supper!=NULL) supper=supper->next;

                (c_max->seats[x]).p=ptr;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==0) && !(c_max->seats[x]).booked && c5!=0 && (c_max->seats[x]).type!='R')
            {
                (c_max->seats[x]).booked=1;
                c5--;
                c_max->c5--;

                ptr=slower;
                if(slower!=NULL) slower=slower->next;

                (c_max->seats[x]).p=ptr;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
        }
        flag1=c1+c2+c3+c4+c5;
        if(flag1==0)
        {
            printf("Every member is seated SUCCESFULLY with preffered Berths!!!!! in coach %s\n",c_max->id);
        }
    }

     else if(d==0)                     //flagptr is the coach in which maximum of the memnbers are going to be seated
    {
    	p1=tik.lower;
    	p2=tik.middle;
    	p3=tik.upper;
    	p4=tik.supper;
    	p5=tik.slower;
    	p6=tik.rem;
    	p7=tik.waiting;
        count=1;
        c1=pref[0];
        c2=pref[1];
        c3=pref[2];
        c4=pref[3];
        c5=pref[4];
        c0=prefcount;
        for(x=0;x<40;x++)
        {
            y=(x+1)%8;
            if((y==1 || y==4) && !(c_max->seats[x]).booked && c1!=0)
            {
                (c_max->seats[x]).booked=1;
                if(count==1)
                {
                    fx=x;
                    count++;
                }
                c1--;
                c0--;
                c_max->c1--;

                ptr=lower;
              /* */
                if(lower!=NULL) lower=lower->next;

                (c_max->seats[x]).p=ptr;
                if(p1==NULL)
                {
                	tik.lower=ptr;
                	p1=tik.lower;
                }
                else
                {
                	p1->next=ptr;
                	p1=p1->next;
                }
                if(p1!=NULL) p1->next=NULL;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==2 || y==5) && !(c_max->seats[x]).booked && c2!=0)
            {
                (c_max->seats[x]).booked=1;
                if(count==1)
                {
                    fx=x;
                    count++;
                }
                c2--;
                c_max->c2--;

                ptr=middle;
               /*  */
                if(middle!=NULL) middle=middle->next;

                (c_max->seats[x]).p=ptr;
                if(p2==NULL)
                {
                	tik.middle=ptr;
                	p2=tik.middle;
                }
                else
                {
                	p2->next=ptr;
                	p2=p2->next;
                }
                if(p2!=NULL) p2->next=NULL;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==3 || y==6) && !(c_max->seats[x]).booked && c3!=0)
            {
                (c_max->seats[x]).booked=1;
                if(count==1)
                {
                    fx=x;
                    count++;
                }
                c3--;
                c_max->c3--;

                ptr=upper;
              /*   */
                if(upper!=NULL) upper=upper->next;

                (c_max->seats[x]).p=ptr;
                if(p3==NULL)
                {
                	tik.upper=ptr;
                	p3=tik.upper;
                }
                else
                {
                	p3->next=ptr;
                	p3=p3->next;
                }
                if(p3!=NULL) p3->next=NULL;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==7) && !(c_max->seats[x]).booked && c4!=0)
            {
                (c_max->seats[x]).booked=1;
                if(count==1)
                {
                    fx=x;
                    count++;
                }
                c4--;
                c_max->c4--;

                ptr=supper;
               /*  */
                if(supper!=NULL) supper=supper->next;

                (c_max->seats[x]).p=ptr;
                if(p4==NULL)
                {
                	tik.supper=ptr;
                	p4=tik.supper;
                }
                else
                {
                	p4->next=ptr;
                	p4=p4->next;
                }
                if(p4!=NULL) p4->next=NULL;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
            if((y==0) && !(c_max->seats[x]).booked && c5!=0 && x!=31 && x!=39 ) // (c_max->seats[x]).type!='R')
            {
                (c_max->seats[x]).booked=1;
                if(count==1)
                {
                    fx=x;
                    count++;
                }
                c5--;
                c_max->c5--;

                ptr=slower;
              /*   */
                if(slower!=NULL) slower=slower->next;

                (c_max->seats[x]).p=ptr;
                if(p5==NULL)
                {
                	tik.slower=ptr;
                	p5=tik.slower;
                }
                else
                {
                	p5->next=ptr;
                	p5=p5->next;
                }
                if(p5!=NULL) p5->next=NULL;
                fflush(stdin);
                fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                free(ptr);
            }
        }
        flag=flag1=c1+c2+c3+c4+c5;

        if(flag!=0)     //remaining members to be seated in the same coach nearest to the first allocated person in the coach.
        {
            remaining=link(lower,middle,upper,supper,slower,lptr,mptr,uptr,suptr,slptr);
            x=fx-1;
            y=fx+1;
            while((x>-1 || y<40) && flag!=0)
            {
                if(x>-1 && !(c_max->seats[x]).booked && x!=31 && x!=39 ) //(c_max->seats[x]).type!='R')
                {
                    te=(x+1)%8;
                    if(c0!=0)
                    {
                        c0--;
                    }
                    if(te==1 || te==4) c_max->c1--;
                    if(te==2 || te==5) c_max->c2--;
                    if(te==3 || te==6) c_max->c3--;
                    if(te==7) c_max->c4--;
                    if(te==0) c_max->c5--;
                    flag--;
                    (c_max->seats[x]).booked=1;

                    ptr=remaining;
                    if(remaining!=NULL) remaining=remaining->next;

                    (c_max->seats[x]).p=ptr;
                    if(p6==NULL)
                	{
                		tik.rem=ptr;
                		p6=tik.slower;
                	}
                	else
                	{
                		p6->next=ptr;
                   		p6=p6->next;
                   	}
                    if(p6!=NULL) p6->next=NULL;
                    fflush(stdin);
                    fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,x+1,c_max->id);
                    free(ptr);
                    }
                if(y<40 && !(c_max->seats[y]).booked && flag!=0 && y!=31 && y!=39 ) //(c_max->seats[y]).type!='R')
                {
                    te=(y+1)%8;
                    if(c0!=0)
                    {
                        c0--;
                    }
                    if(te==1 || te==4) c_max->c1--;
                    if(te==2 || te==5) c_max->c2--;
                    if(te==3 || te==6) c_max->c3--;
                    if(te==7) c_max->c4--;
                    if(te==0) c_max->c5--;
                    flag--;
                    (c_max->seats[y]).booked=1;

                    ptr=remaining;
                    if(remaining!=NULL) remaining=remaining->next;

                    (c_max->seats[y]).p=ptr;
                    if(p6==NULL)
                	{
                		tik.rem=ptr;
                		p6=tik.slower;
                	}
                	else
                	{
                		p6->next=ptr;
                   		p6=p6->next;
                   	}
                    if(p6!=NULL) p6->next=NULL;
                    fflush(stdin);
                    fprintf(in,"\t%s\t\t%d\t\t%d\t\t%s\n",ptr->name,ptr->age,y+1,c_max->id);
                    free(ptr);
                }
                if(flag==0)
                {
                    printf("Everyone is seated SUCCESFULLY!!!!!! in coach %s\n",c_max->id);
                }
                x--;
                y++;
            }
        }
        if(flag!=0)
        {
        	if(strcmp(class,SL)==0) remaining=rem_seats(t[TrainNo-1]->slroot,remaining,&flag);
    		else if(strcmp(class,AC)==0) remaining=rem_seats(t[TrainNo-1]->slroot,remaining,&flag);
    		if(flag!=0)
    		{
    			tik.waiting=remaining;
    		    if(class[0]=='S')
    		    {
                    if(t[TrainNo-1]->sllist==NULL)
                    {
                        t[TrainNo-1]->sllist=remaining;
                    }
                    else
                    {
                        ptr=t[TrainNo-1]->sllist;
                        while(ptr->next!=NULL) ptr=ptr->next;
                        ptr->next=remaining;
                    }
                }
                else
                {
                    if(t[TrainNo-1]->aclist==NULL)
                    {
                        t[TrainNo-1]->aclist=remaining;
                    }
                    else
                    {
                        ptr=t[TrainNo-1]->aclist;
                        while(ptr->next!=NULL) ptr=ptr->next;
                        ptr->next=remaining;
                    }
                }
                printf("%d members are not allocated seat\n",flag);
                fflush(stdin);
                fprintf(in,"%d members are not allocated seat\n",flag);
    		}
    		else printf("Every member is allocated seats\n");
        }
    }
    t[TrainNo-1]->troot=add_ticket(t[TrainNo-1]->troot,p_id,tik);
    display_tik(t[TrainNo-1]->troot,0);
    fclose(in);
    fclose(fp);
}


coach* search_coach(ctree *node, char cid[], int *d)
{
	int i,c,flag;
	coach *retval;

	if(node->isleaf==true && *d!=1)
	{
		c=node->c;
		for(i=0;i<c && *d!=1;i++)
		{
			if(id_to_key(cid)==id_to_key(node->keys[i].id))
			{
				*d=1;
				return &(node->keys[i]);
			}
		}
	}
	else if(*d!=1)
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			retval=search_coach(node->a[i],cid,d);
			if(i < c)
			{
				if(id_to_key(cid)==id_to_key(node->keys[i].id))
				{
					*d=1;
					return &(node->keys[i]);
				}
			}
			return retval;
		}
	}
}

void display_coach(coach *cptr)
{
	printf("\n****************************************************\n****************************************************\n");
        printf("*   Class - %s ,Coach - %s\n", cptr->class,cptr->id);
        for(int i=0;i<40;i++)
        {
            printf("**   Seat Number :          %d\n",i+1);
            printf("**   Booking Status :       %d\n",(cptr->seats[i]).booked);
            printf("**   Seat Type :            %c\n",(cptr->seats[i]).type);

            if((cptr->seats[i]).type!='R' && (cptr->seats[i]).booked)
            {
                printf("**   Passenger Name :       %s\n",((cptr->seats[i]).p)->name);
                printf("**   Passenger ID :         %d\n",((cptr->seats[i]).p)->id);
            }
            else if((cptr->seats[i]).type=='R' && (cptr->seats[i]).booked)
            {
                printf("**   Passenger 1 Name :     %s\n",((cptr->seats[i]).p)->name);
                printf("**   Passenger 1 ID :       %d\n",((cptr->seats[i]).p)->id);
                if((cptr->seats[i]).p->next!=NULL)
                {
                    printf("**   Passenger 2 Name :     %s\n",((cptr->seats[i]).p->next)->name);
                    printf("**   Passenger 2 ID :       %d\n",((cptr->seats[i]).p->next)->id);
                }
            }
            printf("\n");
        }

        printf("*****************************************************\n");
}

void display_coach_data(train *t[], int train_no)
{
    char coach_id[3];
    int d=0;
    passenger *p1,*p2;
    int k1=(t[train_no-1])->nsl,k2=(t[train_no-1])->nac;
    k1=2*k1;
    k2=2*k2;
    printf("Enter Coach ID to display data :"); scanf("%s",coach_id);
    coach *cptr;
    if(coach_id[0]=='S') cptr=search_coach(t[train_no-1]->slroot,coach_id,&d);
    else if(coach_id[0]=='A') cptr=search_coach(t[train_no-1]->acroot,coach_id,&d);

    if(d==1)
    {
        display_coach(cptr);
    }
    else
    {
        printf("ERORR in finding Coach!!!!\n");
    }
    p1=(t[train_no-1])->sllist;
    p2=(t[train_no-1])->aclist;
    while(p1!=NULL)
    {
        if(k1!=0)
        {
            printf("RAC SL-%s\n",p1->name);
            if(p1->next!=NULL)
            {
                p1=p1->next;
                printf("RAC SL-%s\n",p1->name);
            }
            k1--;
        }
        else
        {
            printf("WL SL-%s\n",p1->name);
        }
        p1=p1->next;
    }
    while(p2!=NULL)
    {
        if(k2=0)
        {
            printf("RAC AC-%s\n",p1->name);
            if(p1->next!=NULL)
            {
                p1=p1->next;
                printf("RAC AC-%s\n",p1->name);
            }
            k2--;
        }
        else
        {
            printf("WL AC-%s\n",p1->name);
        }
        p2=p2->next;
    }
}

void range_traverse(ctree *node, char cid1[], char cid2[])
{
	int i,c,flag;

	if(node->isleaf==true)
	{
		c=node->c;
		for(i=0;i<c;i++)
		{
			if(id_to_key(node->keys[i].id)>=id_to_key(cid1) && id_to_key(node->keys[i].id)<=id_to_key(cid2))
			{
				display_coach(&node->keys[i]);
			}
		}
	}
	else
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			range_traverse(node->a[i],cid1,cid2);
			if(id_to_key(node->keys[i].id)>=id_to_key(cid1) && id_to_key(node->keys[i].id)<=id_to_key(cid2))
			{
				display_coach(&node->keys[i]);
			}
		}
	}
}

void Range_display_coach_data(train *t[], int train_no)
{
	char coach_id1[3],coach_id2[3];
    	int d=0;
    	printf("Enter Range\nFrom :"); scanf("%s",coach_id1);
    	printf("To :"); scanf("%s",coach_id2);
    	coach *cptr;
    	if(coach_id1[0]=='S') range_traverse(t[train_no-1]->slroot,coach_id1,coach_id2);
    	else if(coach_id1[0]=='A') range_traverse(t[train_no-1]->acroot,coach_id1,coach_id2);

}

void cancelTicket(train *t[], int train_no)
{
    int p_id, seat_no, booking_status,d;
    char class[3],coach_id[3],name[50],AC[]="AC",SL[]="SL";
    coach *kptr;
    passenger *p,*ptr;

    d=0;
    printf("Name of the Passenger :"); scanf("%s",name);
    printf("Passenger id :"); scanf("%d",&p_id);
    printf("Bokking Status :\n Booked : 1, RAC/WL : 0 :"); scanf("%d",&booking_status);

    if(booking_status==1)
    {
        printf("Coach ID :"); scanf("%s",coach_id);
        if(coach_id[0]=='S') kptr=search_coach(t[train_no-1]->slroot,coach_id,&d);
    	else if(coach_id[0]=='A') kptr=search_coach(t[train_no-1]->acroot,coach_id,&d);

        printf("Seat Number :"); scanf("%d",&seat_no);

        if(d==1)
        {
            p=(kptr->seats[seat_no-1]).p;
            if(strcmp(p->name,name)==0 )//&& p->id==p_id)
            {

                if(kptr->id[0]=='S' && t[train_no-1]->sllist!=NULL)
                {
                    p=t[train_no-1]->sllist;
                    t[train_no-1]->sllist=t[train_no-1]->sllist->next;
                    p->next=NULL;
                    (kptr->seats[seat_no-1]).p=p;
                    printf("Ticket Cancelled SUCCESFULLY!!!\n");
                }
                else if(kptr->id[0]=='A' && t[train_no-1]->aclist!=NULL)
                {
                    p=t[train_no-1]->aclist;
                    t[train_no-1]->aclist=t[train_no-1]->aclist->next;
                    p->next=NULL;
                    (kptr->seats[seat_no-1]).p=p;
                    printf("Ticket Cancelled SUCCESFULLY!!!\n");
                }
                else
                {
                    (kptr->seats[seat_no-1]).p=NULL;
                    (kptr->seats[seat_no-1]).booked=0;
                    printf("Ticket Cancelled SUCCESFULLY!!!\n");
                }
            }
            else
            {
                printf("ERORR in finding passenger!!!!\n");
            }
        }
        else
        {
            printf("ERORR in finding Coach!!!!\n");
        }
    }
    if(booking_status==0)
    {
        printf("Booking Class :"); scanf("%s",class);
        if(class[0]=='S' && t[train_no-1]->sllist!=NULL)
        {
            ptr=p=t[train_no-1]->sllist;
            while(p!=NULL && p->id!=p_id)
            {
                ptr=p;
                p=p->next;
            }
            if(p!=NULL)
            {
                while(p!=NULL && strcmp(p->name,name)!=0)
                {
                    ptr=p;
                    p=p->next;
                }
                if(p!=NULL)
                {
                    printf("Ticket Cancelled SUCCESFULLY!!!\n");
                    ptr->next=p->next;
                    printf("%d %s",p->id,p->name);
                    free(p);
                }
            }
            else
            {
                printf("ERORR in finding passenger!!!!\n");
            }
        }
        if(class[0]=='A' && t[train_no-1]->aclist!=NULL)
        {
            ptr=p=t[train_no-1]->aclist;
            while(p!=NULL && p->id!=p_id)
            {
                ptr=p;
                p=p->next;
            }
            if(p!=NULL)
            {
                while(p!=NULL && strcmp(p->name,name)!=0)
                {
                    ptr=p;
                    p=p->next;
                }
                if(p!=NULL)
                {
                    printf("Ticket Cancelled SUCCESFULLY!!!\n");
                    ptr->next=p->next;
                    printf("%d %s",p->id,p->name);
                    free(p);
                }
            }
            else
            {
                printf("ERORR in finding passenger!!!!\n");
            }
        }
    }
}

void freeze(coach *cptr, train *t)
{
	passenger *p;
	int k;
	if(cptr->id[0]=='A' && t->aclist!=NULL)
        {
            k=31;
            while(k<40)
            {
                p=t->aclist;
                (cptr->seats[k]).p=p;
                (cptr->seats[k]).booked=1;

                t->aclist=(t->aclist)->next;
                p->next=NULL;
                free(p);
                if(t->aclist!=NULL)
                {
                    p=t->aclist;
                    (cptr->seats[k]).p->next=p;
                    t->aclist=(t->aclist)->next;
                    p->next=NULL;
                    free(p);
                }
                k+=8;
            }
            t->nac--;
        }
        if(cptr->id[0]=='S' && t->sllist!=NULL)
        {
            k=31;
            while(k<40)
            {
                p=t->sllist;
                (cptr->seats[k]).p=p;
                (cptr->seats[k]).booked=1;

                t->sllist=(t->sllist)->next;
                p->next=NULL;
                free(p);
                if(t->sllist!=NULL)
                {
                    p=t->sllist;
                    (cptr->seats[k]).p->next=p;
                    t->sllist=(t->sllist)->next;
                    p->next=NULL;
                    free(p);
                }
                k+=8;
            }
            t->nsl--;
        }
}

void freeze_booking_traverse(ctree *node, train *t)
{
	int i,c,flag;

	if(node->isleaf==true)
	{
		c=node->c;
		for(i=0;i<c;i++)
		{
			freeze(&node->keys[i],t);
		}
	}
	else
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			freeze_booking_traverse(node->a[i],t);
			if(i<c)
			{
				freeze(&node->keys[i],t);
			}
		}
	}
}


void freeze_booking(train *t[], int train_no)
{
    coach *cptr;
    passenger *p;
    int k;


    if(t[train_no-1]->slroot!=NULL) freeze_booking_traverse(t[train_no-1]->slroot,t[train_no-1]);
    if(t[train_no-1]->acroot!=NULL) freeze_booking_traverse(t[train_no-1]->acroot,t[train_no-1]);

    printf("Booking Freezed!!!!\n");
}


int main()
{

    printf("\t\t=================================================\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|           TRAIN TICKET RESR. SYSTEM           |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t=================================================\n\n\n");

    int ch =1,i,choice,flag=0;


    train t[10];
    initialise(&t,10);

	while(ch!=0)
	{
    	printf("\n\nDo you want to - \n 1. Add Train \n 2. Add Coach \n 3. Book tickets? \n 4. Cancel Booking \n 5. Display Coach Details\n 6. Range Display Coach Details \n 0. Exit \n\n");
    	scanf("%d", &choice);
    	switch(choice)
    	{
    		case 1 :  addTrain(&t);
    	                  break;

    	    case 2:
    		      printf("Enter Train Number :"); scanf("%d",&i);
    		      addCoach(&t,i);
    	              break;

    	    case 3:   Form(&t);
    	              break;

    	    case 4:   printf("Enter Train Number :"); scanf("%d",&i);
    	              cancelTicket(&t,i);
    	              break;

    	    case 5:   printf("Enter Train Number :"); scanf("%d",&i);
    		      display_coach_data(&t,i);
    	              break;

    	    case 6:   printf("Enter Train Number :"); scanf("%d",&i);
    		      Range_display_coach_data(&t,i);
    	              break;

            case 0:   ch=0;
                      break;

    	    default: printf("\nWrong choice!\n");
    	}
	}

    printf("**************************************Thanks For Visiting**************************************\n");

    return 0;
}




ctree* insert(ctree *root,coach ct)
{
	ctree *ptr=root,*parent_ptr;
	ctree *ptr3;
	int i;
	int d=1;
	if(root==NULL)
	{
		root=makecoachnode();
		root->keys[root->c]=ct;
		(root->c)++;
	}
	else
	{
		ptr=search_pos_coach(root,ct.id[0],id_to_key(ct.id),&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array_ins(ptr->keys,ct,ptr->c);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array_ins(ptr->keys,ct,ptr->c);
				ptr->c++;
				root=split(root,ptr);
			}
		}
	}
	return root;
}


ctree* split(ctree *root,ctree *ptr)
{
	int i,j,flag,z,p;
	ctree *ptr2,*pptr,*ptr3,*retval;
	if(ptr==root)
	{
		root=insert(NULL,ptr->keys[MAX/2]);
		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			root->a[0]=ptr2;
			root->isleaf=false;
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;
			}
			ptr2=NULL;
			j++;
			for(;j<MAX;j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;
			}
			root->a[1]=ptr2;
			retval=root;

		if(ptr->isleaf==false)
		{
			j=0;
			ptr2=root->a[0];
			for(i=0;i<=(MAX/2);i++)
			{
				//printf("&%d& ",ptr->a[j]->c);
				ptr2->a[i]=ptr->a[j++];
			}
			ptr2=root->a[1];
			j=0;

			for(;i<=MAX;i++)
			{
				ptr2->a[j++]=ptr->a[i];
			}
		}
		free(ptr);
	}
	else
	{
		pptr=findparent(root,ptr);
		add_array_ins(pptr->keys,ptr->keys[MAX/2],pptr->c);
		pptr->c++;
		flag=0;
		for(i=0;i<pptr->c && flag==0;)
		{
			if(id_to_key(pptr->keys[i].id)==id_to_key(ptr->keys[MAX/2].id))
			{
				flag=1;
			}
			else
			{
				i++;
			}
		}

		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			pptr->a[i]=ptr2;
			pptr->isleaf=false;
			z=0;
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}

		ptr2=NULL;
		j++;
			for(;j<MAX;j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}
			int k,l;
			l=pptr->c-1;
			k=l+1;
			while(l!=i)
			{
				pptr->a[k]=pptr->a[l];
				k--;
				l--;
			}
			pptr->a[k]=ptr2;

		if(pptr->c<5)
		{
			retval=root;
		}
		else
		{
			retval=split(root,pptr);
		}
		free(ptr);
	}
	return retval;
}

ptree* insert_tik(ptree *root,ticket ct)
{
	ptree *ptr=root,*parent_ptr;
	ptree *ptr3;
	int i;
	int d=1;
	if(root==NULL)
	{
		root=maketicketnode();
		root->keys[root->c]=ct;
		(root->c)++;
	}
	else
	{
		ptr=search_pos_ticket(root,ct.ticket_id,&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array_ins_tik(ptr->keys,ct,ptr->c);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array_ins_tik(ptr->keys,ct,ptr->c);
				ptr->c++;
				root=split_tik(root,ptr);
			}
		}
	}
	return root;
}


ptree* split_tik(ptree *root,ptree *ptr)
{
	int i,j,flag,z,p;
	ptree *ptr2,*pptr,*ptr3,*retval;
	if(ptr==root)
	{
		root=insert_tik(NULL,ptr->keys[MAX/2]);
		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=insert_tik(ptr2,ptr->keys[j]);
			}
			root->a[0]=ptr2;
			root->isleaf=false;
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;
			}
			ptr2=NULL;
			j++;
			for(;j<MAX;j++)
			{
				ptr2=insert_tik(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;
			}
			root->a[1]=ptr2;
			retval=root;

		if(ptr->isleaf==false)
		{
			j=0;
			ptr2=root->a[0];
			for(i=0;i<=(MAX/2);i++)
			{
				//printf("&%d& ",ptr->a[j]->c);
				ptr2->a[i]=ptr->a[j++];
			}
			ptr2=root->a[1];
			j=0;

			for(;i<=MAX;i++)
			{
				ptr2->a[j++]=ptr->a[i];
			}
		}
		free(ptr);
	}
	else
	{
		pptr=findparent_tik(root,ptr);
		add_array_ins_tik(pptr->keys,ptr->keys[MAX/2],pptr->c);
		pptr->c++;
		flag=0;
		for(i=0;i<pptr->c && flag==0;)
		{
			if(pptr->keys[i].ticket_id==ptr->keys[MAX/2].ticket_id)
			{
				flag=1;
			}
			else
			{
				i++;
			}
		}

		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=insert_tik(ptr2,ptr->keys[j]);
			}
			pptr->a[i]=ptr2;
			pptr->isleaf=false;
			z=0;
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}

		ptr2=NULL;
		j++;
			for(;j<MAX;j++)
			{
				ptr2=insert_tik(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}
			int k,l;
			l=pptr->c-1;
			k=l+1;
			while(l!=i)
			{
				pptr->a[k]=pptr->a[l];
				k--;
				l--;
			}
			pptr->a[k]=ptr2;

		if(pptr->c<5)
		{
			retval=root;
		}
		else
		{
			retval=split_tik(root,pptr);
		}
		free(ptr);
	}
	return retval;
}



