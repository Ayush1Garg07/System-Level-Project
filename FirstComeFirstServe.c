#include <stdio.h>
#include <string.h>

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

int main()
{
    int n;
    printf("Enter the number of jobs: ");
    scanf("%d", &n);

    Job jobs[n];

    for(int i=0; i<n; i++)
    {
        printf("Enter the arrival time and burst time for job %d: ", i);
        scanf("%d %d", &jobs[i].arrival_time, &jobs[i].burst_time);
        jobs[i].pid = i;
    }

    FCFS(jobs, n);
    
    return 0;
}
