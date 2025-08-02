#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

int cmp_burst_time(const void *a, const void *b) {
    SJFjobs *j1 = (SJFjobs *)a;
    SJFjobs *j2 = (SJFjobs *)b;
    return j1->burst_time - j2->burst_time;
}

int main()
{   
    int k;
    printf("Enter the number of jobs: ");
    scanf("%d", &k);

    SJFjobs jobs[k];
    
    int select;

    for(int i=0; i<k; i++)
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
        ShortestRemainingtimefirst(jobs, k);
        printJobTable(jobs, k);
    }
    
    else{
    int n = sizeof(jobs) / sizeof(jobs[0]);

    qsort(jobs, n, sizeof(SJFjobs), cmp_burst_time);

    shortestJobFirst(jobs, k);
    printJobTable(jobs, k);
    }

    return 0;
}