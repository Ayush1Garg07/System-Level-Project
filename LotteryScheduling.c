#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct 
{
    char name[5];
    int tickets;
    int remaining_time;
    int turnaround_time;
    int burst_time;
    int wait_time;
    int completed;
    int response_time;
}Lottery;

void LotteryScheduling(Lottery jobs[], int n)
{
    int total_tickets, time_elapse=0, completed=0;

    srand(time(NULL));

    for(int i=0; i<n; i++)
    {
        jobs[i].remaining_time = jobs[i].burst_time;
        jobs[i].turnaround_time=0;
        jobs[i].wait_time=0;
        jobs[i].completed=0;
        jobs[i].response_time=-1;
    }

    while(completed<n)
    {
       total_tickets=0;
       for(int i=0; i<n; i++){
            if (!jobs[i].completed)             //Calculate the total tickets
            {
                total_tickets += jobs[i].tickets;
            }
       } 

       if(total_tickets==0) break;
       
       int winticket = rand()%total_tickets + 1;   //Random Selection of Winning Ticket

       int count=0;
       for(int i=0; i<n; i++)
       {
           if(jobs[i].completed) continue;
           count+=jobs[i].tickets;

           if(winticket<=count){                //Checking in which range winning ticket lies

            if (jobs[i].response_time == -1)
            jobs[i].response_time = time_elapse;

            jobs[i].remaining_time--;
            time_elapse++;

            if(jobs[i].remaining_time==0){
                jobs[i].completed=1;
                completed++;
                jobs[i].turnaround_time = time_elapse;
                jobs[i].wait_time = jobs[i].turnaround_time - jobs[i].burst_time;
            }
            break;
           }
       }
    }
    
    printf("\nProcess\tBT\tTickets\tWT\tTAT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", jobs[i].name, jobs[i].burst_time, jobs[i].tickets,
               jobs[i].wait_time, jobs[i].turnaround_time, jobs[i].response_time);
        }
}

int main()
{
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Lottery jobs[n];
    int range;
    int choice;
    printf("Press 1 for random ticket Allocation or 2 for User Ticket input");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:

        printf("Enter the range of the tickets: ");
        scanf("%d", &range);
        for(int i=0; i<n; i++)
        {
        printf("Enter the name, burst time for job %d: ", i+1);
        scanf("%s %d", jobs[i].name, &jobs[i].burst_time);
        jobs[i].tickets = rand()%range + 1;
        }
        LotteryScheduling(jobs, n);
        break;

    case 2: 

    for(int i=0; i<n; i++)
    {
        printf("Enter the name, tickets, burst time for job %d: ", i+1);
        scanf("%s %d %d", jobs[i].name, &jobs[i].tickets, &jobs[i].burst_time);
    }
    LotteryScheduling(jobs, n);
    break;
    
    default:
        printf("Wrong Choice!");
        break;
    }

    return 0;
}