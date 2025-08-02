#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100

typedef struct 
{
    int data[MAX];
    int front;
    int rear;
}Queue;

void initQueue(Queue* q)
{
    q->front = q->rear = -1;
}

int isEmpty(Queue* q)
{
    return q->front==-1;
}

void enqueue(Queue* q, int val)
{
    if(q->rear == MAX - 1)
    {
        printf("Overflow");
        return;
    }
    if(isEmpty(q)) q->front = 0;

    q->rear++;
    q->data[q->rear] = val;
}

int dequeue(Queue* q)
{
    if(isEmpty(q)){
        printf("Queue Underflow");
        return -1;
    }
    int value= q->data[q->front];
    if(q->front == q->rear)
    {
        q->front = q->rear = -1;
    }
    else q->front++;
    return value;
}

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

int min(int a, int b)
{
    if(a<b) return a;
    else return b;
}


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

int main()
{
    int n;
    scanf("%d", &n);

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

    return 0;
}
