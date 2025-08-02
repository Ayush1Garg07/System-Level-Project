//*****************************CPU Scheduling Algorithms***************************************
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue *q) {
    return q->front == -1;
}

void enqueue(Queue *q, int value) {
    if (q->rear == MAX - 1) {
        printf("Queue Overflow\n");
        return;
    }
    if (isEmpty(q))
        q->front = 0;
    q->rear++;
    q->data[q->rear] = value;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue Underflow\n");
        return -1;
    }
    int val = q->data[q->front];
    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front++;
    return val;
}

int min(int a, int b) { return a < b ? a : b; }

////////////////********FIRST COME FIRST SERVE**********////////////////////////////////////////////////

typedef struct
{
    int pid;
    int arrival_time;
    int burst_time;
    int comp_time;
    int wait_time;
    int turnaround_time;
    int response_time;
}Job;

void FCFS(Job jobs[],int n)
{
    int time=0;
    int start_time;
    for(int i=0; i<n; i++)
    {
        if(time < jobs[i].arrival_time) 
        time = jobs[i].arrival_time;
        
        start_time = time;
        time += jobs[i].burst_time;
        jobs[i].comp_time = time;
        jobs[i].turnaround_time = jobs[i].comp_time - jobs[i].arrival_time;
        jobs[i].wait_time = jobs[i].turnaround_time - jobs[i].burst_time;
        jobs[i].response_time = start_time - jobs[i].arrival_time;

    }

    printf("\nJob\tArrival\tBurst\tComp\tTurn\tWait\tResponse\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            jobs[i].pid,
            jobs[i].arrival_time,
            jobs[i].burst_time,
            jobs[i].comp_time,
            jobs[i].turnaround_time,
            jobs[i].wait_time,
            jobs[i].response_time);
    }
}

////////////******SHORTEST JOB FIRST***********//////////////////////////////////////////////////////////////

typedef struct 
{
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int completion_time;
    int turnaround_time;
    int start_time;
    int response_time;
    int is_completed;
    int remaining_time;
}SJFjobs;

int cmp_burst_time(const void *a, const void *b) {
    SJFjobs *j1 = (SJFjobs *)a;
    SJFjobs *j2 = (SJFjobs *)b;
    return j1->burst_time - j2->burst_time;
}

void printJobTable(SJFjobs jobs[], int n) {
    printf("\nPID\tAT\tBT\tST\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               jobs[i].pid,
               jobs[i].arrival_time,
               jobs[i].burst_time,
               jobs[i].start_time,
               jobs[i].completion_time,
               jobs[i].turnaround_time,
               jobs[i].waiting_time,
               jobs[i].response_time);
    }
}

void shortestJobFirst(SJFjobs jobs[], int n)
{
    int time =0;
    int start_time;
    for(int i=0; i<n; i++)
    {
        if(time < jobs[i].arrival_time) time = jobs[i].arrival_time;
        
        start_time= time;
        time += jobs[i].burst_time;
        jobs[i].completion_time = time;
        jobs[i].turnaround_time = jobs[i].completion_time - jobs[i].arrival_time;
        jobs[i].waiting_time = jobs[i].turnaround_time - jobs[i].burst_time;
        jobs[i].response_time = start_time - jobs[i].arrival_time;
    }
}

void ShortestRemainingtimefirst(SJFjobs jobs[], int n)
{
    int time=0;
    int completed=0;

    while(completed < n)
    {
        int idx = -1;
        int min_remain_time = INT_MAX;

        for(int i=0; i<n ;i++)
        {
            if(jobs[i].arrival_time <= time && jobs[i].remaining_time > 0)
            {
                if(jobs[i].remaining_time < min_remain_time)
                {
                    min_remain_time = jobs[i].remaining_time;
                    idx = i;
                }
            }
        }

        if(idx != -1)
            {
                if(jobs[idx].start_time == -1)
                    jobs[idx].start_time = time;


                    jobs[idx].remaining_time -=1;
                    time+=1;

                if (jobs[idx].remaining_time == 0) {
                jobs[idx].completion_time = time;
                jobs[idx].turnaround_time = jobs[idx].completion_time - jobs[idx].arrival_time;
                jobs[idx].waiting_time = jobs[idx].turnaround_time - jobs[idx].burst_time;
                jobs[idx].response_time = jobs[idx].start_time - jobs[idx].arrival_time;
                completed++;
            }
            }
            else {
                time += 1;
            }
    }
}

////////////////*****ROBIN ROUND********//////////////////////////////////////////////////////

typedef struct 
{
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int completion_time;
}RRjobs;

void RobinRound(RRjobs jobs[], int k)
{
    Queue q;
    initQueue(&q);

    int time = 0;
    int completed =0;
    int quantum=2; //we assumed it as 2, it can be anything
    int remaining_time[k], visited[k];

    for(int i=0; i<k; i++)
    {
        remaining_time[i] = jobs[i].burst_time;
        visited[i]=0;
    }

    for(int i=0; i<k; i++)
    {
        if(jobs[i].arrival_time <= time && !visited[i]){
            enqueue(&q, i);
            visited[i]=1;
        }
    }

    while(completed < k)
    {
        if(isEmpty(&q)){
            time++;            //CPU idle

            for(int i=0; i<k; i++)
            {
                if(jobs[i].arrival_time <= time && !visited[i])
                {
                    remaining_time[i] = jobs[i].burst_time;
                    visited[i] =1;
                }
            }
            continue;    
        }

        int idx = dequeue(&q);

        if(remaining_time[idx] == jobs[idx].burst_time)
        {
            jobs[idx].response_time = time - jobs[idx].arrival_time;
        }



        int exec_time = (remaining_time[idx] < quantum) ? remaining_time[idx] : quantum;
        time+=exec_time;
        remaining_time[idx] -= exec_time;
        
        for(int i=0; i<k; i++)
        {
            if(jobs[i].arrival_time <= time && !visited)
            {
                enqueue(&q, i);
                visited[i] = 1;
            }
        }

        if(remaining_time[idx] > 0){
            enqueue(&q, idx);
        }
        else{
            completed++;
            jobs[idx].completion_time = time;
            jobs[idx].turnaround_time = jobs[idx].completion_time - jobs[idx].arrival_time;
            jobs[idx].waiting_time = jobs[idx].turnaround_time - jobs[idx].burst_time;
        }

    }
    
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < k; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", jobs[i].pid, jobs[i].arrival_time, jobs[i].burst_time,
        jobs[i].completion_time, jobs[i].turnaround_time, jobs[i].waiting_time, jobs[i].response_time);
    }              
}

//////////////********MULTILEVEL FEEDBACK QUEUE********//////////////////////////////////////////////////////////

typedef struct 
{
    int pid;
    int arrival_time;
    int burst_time;
    int comp_time;
    int wait_time;
    int remaining_time;
    int turnaround_time;
    int response_time;
    int completion_time;
    int start_time;
    int queue_level;
    bool is_completed ;
}MLFQ;

void MultiLevelFeedbackQueue(MLFQ jobs[], int k)
{
    Queue q1,q2,q3;
    initQueue(&q1);
    initQueue(&q2);
    initQueue(&q3);

    int time=0;
    int quantum[3] = {4,8,1000};
    int completed=0;

    while(completed < k){
        for(int i=0; i<k; i++)
        {
            if(jobs[i].arrival_time == time)
            {
                enqueue(&q1, i);
                jobs[i].queue_level = 0;
            }
        }

        int idx = -1, level=-1;
        if(!isEmpty(&q1)){
            idx = dequeue(&q1);
            level=0;
        }
        else if(!isEmpty(&q2)){
            idx = dequeue(&q2);
            level = 1;
        }
        else if(!isEmpty(&q3)){
            idx = dequeue(&q3);
            level = 2;
        }
        else{
            time++;
            continue;
        }

        MLFQ *p = &jobs[idx];
        int runtime = min(p->remaining_time, quantum[level]);
        
        if(p->start_time == -1){
            p->start_time = time;
            p->response_time = time - p->arrival_time;
        }

        for(int i=0; i<runtime ; i++)
        {
            time++;
            for(int j=0; j < k; j++)
            {
                if(jobs[j].arrival_time == time)
                {
                    enqueue(&q1, j);
                    jobs[j].queue_level=0;
                }
            }
        }

        p->remaining_time -= runtime; 

        if(p->remaining_time == 0){
            p->completion_time = time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->wait_time = p->turnaround_time - p->burst_time;
            p->is_completed = true;
            completed++;
        }
        else{
            if(level == 0) enqueue(&q1, idx);
            else enqueue (&q2, idx);
        }

    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < k; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", jobs[i].pid, jobs[i].arrival_time, jobs[i].burst_time,
               jobs[i].completion_time, jobs[i].turnaround_time, jobs[i].wait_time, jobs[i].response_time);
    }
}

int main() {
    int choice, n, quantum;
    printf("\nCPU Scheduling Algorithms Project\n");
    printf("1. First Come First Serve (FCFS)\n");
    printf("2. Round Robin\n");
    printf("3. Shortest Remaining Time First (SRTF)\n");
    printf("4. Multilevel Feedback Queue (MLFQ)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("\nEnter number of processes: ");
    scanf("%d", &n);

    switch (choice)
    {
    case 1:{
        Job jobs[n];

    for(int i=0; i<n; i++)
    {
        printf("Enter the arrival time and burst time for job %d: ", i);
        scanf("%d %d", &jobs[i].arrival_time, &jobs[i].burst_time);
        jobs[i].pid = i;
    }

    FCFS(jobs, n);
        break;
    }

    case 2: {
        RRjobs jobs[n];

    for(int i=0; i<n; i++)
    {
        printf("Enter the arrival time and burst time of job %d: ", i);
        scanf("%d %d", &jobs[i].arrival_time, &jobs[i].burst_time);
        jobs[i].pid = i;
    }

    RobinRound(jobs, n);
    break;
    }

    case 3:{
        SJFjobs jobs[n];
    
    int select;

    for(int i=0; i<n; i++)
    {
        printf("Enter the arrival time and burst time of job %d: ", i);
        scanf("%d %d", &jobs[i].arrival_time, &jobs[i].burst_time);
        jobs[i].pid = i;
        jobs[i].remaining_time = jobs[i].burst_time;
        jobs[i].start_time = -1;

        if(jobs[i].arrival_time != 0) select++;
    }

    if(select > 0 )
    {
        ShortestRemainingtimefirst(jobs, n);
        printJobTable(jobs, n);
    }
    
    else{
    int n = sizeof(jobs) / sizeof(jobs[0]);

    qsort(jobs, n, sizeof(SJFjobs), cmp_burst_time);

    shortestJobFirst(jobs, n);
    printJobTable(jobs, n);
    }
    break;
    }
    
    case 4:{
        MLFQ jobs[n];

    for(int i=0; i<n; i++)
    {
        printf("Enter the arrival time and burst time for job %d: ", i);
        scanf("%d %d", &jobs[i].arrival_time, &jobs[i].burst_time);
        jobs[i].remaining_time = jobs[i].burst_time;
        jobs[i].start_time = -1;
        jobs[i].is_completed = false;
        jobs[i].pid = i;
    }

    MultiLevelFeedbackQueue(jobs, n);
    break;
    }
    default:
        printf("Invalid Choice");
        break;
    }

    return 0;
}

