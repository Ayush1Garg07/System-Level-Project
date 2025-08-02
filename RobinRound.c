#include <stdio.h>
#include <limits.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int front;
    int rear;
} Queue;

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

int main()
{
    int k;
    printf("Enter the number of jobs: ");
    scanf("%d", &k);

    RRjobs jobs[k];

    for(int i=0; i<k; i++)
    {
        printf("Enter the arrival time and burst time of job %d: ", i);
        scanf("%d %d", &jobs[i].arrival_time, &jobs[i].burst_time);
        jobs[i].pid = i;
    }

    RobinRound(jobs, k);
    return 0;
}