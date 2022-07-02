#include<stdio.h>
#include<stdlib.h>



typedef struct 
{


	int pathTime;

	int Time;

	int reader;

	int book;

	int *assigned;
}Node;

Node newNode(int x, int y, int assigned[],int N)
{
	Node node;
    node.assigned=(int *)malloc(N*sizeof(int));
	for (int j = 0; j < N; j++)
		{
            node.assigned[j] = assigned[j];
        }
	node.assigned[y] = 1;


	node.reader = x;
	node.book = y;

	return node;
}
typedef long long ll;
typedef struct{
    ll front;
    ll rear;
    ll size;
    ll capacity;
    Node *arr;
}Queue;
Queue *BuildHeap(ll n)//Build heap
{
    Queue *q=(Queue *)malloc(sizeof(Queue));
    q->capacity=n+1;
    q->front=0;
    q->size=0;
    q->rear=-1;
    q->arr= (Node *)malloc((n+1)*sizeof(Node));
    return q;
}
void enqueue(Queue* q, Node Nd)//inserting at last
{
    if (q->size==q->capacity)
        return;
    q->rear = (q->rear + 1)% (q->capacity);
    q->arr[q->rear] = Nd;
    q->size = q->size + 1;
}

void update_down(Queue *q,ll index)//update down to maintain heap property
{
    if(index==q->rear)
    {
        return;
    }
  
    Node Nd=q->arr[index];
    if((2*index+1<q->rear)&&(2*index+2)<=q->rear)
    {
    Node l=q->arr[2*index+1];
    Node r=q->arr[2*index+2];
    Node small;
    ll idx;
    if((Nd.Time<=l.Time)&&(Nd.Time<=r.Time))
    {
        small=Nd;
        idx=index;
    }
    else if((l.Time<=Nd.Time)&&(l.Time<=r.Time))
    {
        small=l;
        idx=2*index+1;
    }
    else if((r.Time<=l.Time)&&(r.Time<=Nd.Time))
    {
        small=r;
        idx=2*index+2;
    }
    if(small.Time!=Nd.Time)
    {
        Node temp;
        temp=small;
        q->arr[idx]=Nd;
        q->arr[index]=temp;
        
        update_down(q,idx);
    }
    }
    else if(2*index+1<=q->rear)
    {
    Node l=q->arr[2*index+1];
    
    Node small;
    ll idx;
    if(Nd.Time<l.Time)
    {
        small=Nd;
        idx=index;
    }
    else if(l.Time<Nd.Time)
    {
        small=l;
        idx=2*index+1;
    }
    
    if(small.Time!=Nd.Time)
    {
        
        q->arr[idx]=q->arr[index];
        q->arr[index]=small;
        
        update_down(q,idx);
    }
    }
}
Node remove_min(Queue *q)//removes min and maintain heap property
{
    Node temp;
     temp=q->arr[q->front];
    q->arr[q->front]=q->arr[q->rear];
    q->rear=(q->rear-1)%(q->capacity);
    q->size=q->size-1;
    update_down(q,q->front);
    return temp;
    
}
void update_up(Queue *q,ll index)//updates upwards to maintain heap property
{
    Node Nd=q->arr[index];
    
    if(index!=q->front)
    {
        Node parent=q->arr[((index-1)/2)%(q->capacity)];
        ll idx=((index-1)/2)%(q->capacity);
        if(Nd.Time<parent.Time)
        {
            Node temp=q->arr[idx];
            q->arr[idx]=q->arr[index];
            q->arr[index]=temp;
            update_up(q,idx);
        }
    }
}
void insert(Queue *q,Node Nd)//inserts at last and then heapify
{
  
    enqueue(q,Nd);
    update_up(q,q->rear);
}

int calculateTime(int **TimeMatrix,int N, int x,int y, int assigned[])
{
	int Time = 0;

	int available[N];
    for(int i=0;i<N;i++)
    {
        available[i]=1;
    }

	for (int i = x + 1; i < N; i++)
	{
		int min = 10000, minIndex = -1;

		for (int j = 0; j < N; j++)
		{
			if (!assigned[j] && available[j] &&TimeMatrix[i][j] < min)
			{
				minIndex = j;

				min = TimeMatrix[i][j];
			}
		}
		Time += min;

		available[minIndex] = 0;
	}

	return Time;
}





int findMinTime(int **TimeMatrix,int N)
{

   Queue *pq;
   pq=BuildHeap(100);
	int assigned[N];
    for(int i=0;i<N;i++)
    {
        assigned[i]=0;
    }
	Node root = newNode(-1, -1, assigned,N);
	root.pathTime = root.Time = 0;
	root.reader = -1;

	insert(pq,root);

	while (pq->size!=0)
	{
	Node min = remove_min(pq);



	int i = min.reader + 1;

	if (i == N)
	{
		
		return min.Time;
	}

	for (int j = 0; j < N; j++)
	{
		if (min.assigned[j]==0)
		{
		Node child = newNode(i, j, min.assigned,N);

		child.pathTime = min.pathTime + TimeMatrix[i][j];

	
		child.Time = child.pathTime +	calculateTime(TimeMatrix,N, i, j, child.assigned);

	   
		insert(pq,child);
		}
	}
	}
}


int main()
{
    int N;
    printf("Write the number of readers(number of books is equal to number of readers):  ");
    scanf("%d",&N);

	int **TimeMatrix;
    TimeMatrix=(int **)malloc(N*sizeof(int *));
    for(int i=0;i<N;i++)
    {
        TimeMatrix[i]=(int *)malloc(N*sizeof(int));
    }
    printf("Write 2D matrix of %d X %d entries:\n",N,N);
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            scanf("%d",&TimeMatrix[i][j]);
        }
    }
	/*{
		Sample test case it outputs 140
        {82, 83, 69, 92},
        {77, 37, 49, 92},
        {11, 69,  5, 86},
        { 8,  9, 98, 23}
	};

*/
	
	printf("\nOptimal Time is  %d", findMinTime(TimeMatrix,N));

	return 0;
}
