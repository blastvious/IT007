/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: FCFS.c
#
######################################*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
typedef struct
{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;
void inputProcess(int n, PCB P[])
{
    srand(time(NULL));
    printf("Generating %d process... (┬┬﹏┬┬)\n", n);
    for (int i = 0; i < n; i++)
    {
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;
        P[i].iBurst = (rand() % 11) + 2;
        P[i].iStart = 0;
        P[i].iFinish = 0;
        P[i].iResponse = 0;
        P[i].iTaT = 0;
        printf("Process ID: %d, Arrival Time: %d, Burst Time: %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}
void printProcess(int n, PCB P[])
{
    printf("List of PCB\n");
    for (int i = 0; i < n; i++)
    {
        printf("PID %d     Arrival Time %d     Bust Time %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}
void exportGanttChart(int n, PCB P[])
{
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++)
    {
        printf("| P%d ", P[i].iPID);
    }
    printf("|\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d   ", P[i].iStart);
    }
    printf("%d\n", P[n - 1].iFinish);
}
void pushProcess(int *n, PCB P[], PCB Q)
{
    if (*n < 0)
        return;
    P[*n] = Q;
    (*n)++;
}
void removeProcess(int *n, int index, PCB P[])
{
    if (*n < 0)
        return;
    if (index < 0 || index >= *n)
        return; // check invalid
    for (int i = index; i < *n - 1; i++)
    {
        P[i] = P[i + 1];
    }
    (*n)--;
}
int swapProcess(PCB *P, PCB *Q)
{
    PCB tmp = *P;
    *P = *Q;
    *Q = tmp;
}
int partition(PCB P[], int low, int high, int iCriteria)
{
    if (low < 0 || high < 0 || low >= high)
    {
        return low;
    }
    int pivotValue;
    switch (iCriteria)
    {
    case 0:
        pivotValue = P[high].iArrival;
        break;
    case 1:
        pivotValue = P[high].iBurst;
        break;
    default:
        pivotValue = P[high].iArrival;
        break;
    }
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        int compareValue;
        switch (iCriteria)
        {
        case 0:
            compareValue = P[j].iArrival;
            break;
        case 1:
            compareValue = P[j].iBurst;
            break;
        default:
            compareValue = P[j].iArrival;
            break;
        }

        if (compareValue < pivotValue)
        {
            ++i;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return i + 1;
}
void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low < high)
    {
        int pos = partition(P, low, high, iCriteria);
        quickSort(P, low, pos - 1, iCriteria);
        quickSort(P, pos + 1, high, iCriteria);
    }
}
void calculateAWT(int n, PCB P[])
{
    int totalWaitingTime = 0;
    printf("Waiting time: ");
    for (int i = 0; i < n; i++)
    {
        printf("P%d = %d | ", P[i].iPID, P[i].iWaiting);
        totalWaitingTime += P[i].iWaiting;
    }
    printf("\n");
    printf("Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
}
void calculateATaT(int n, PCB P[])
{
    int totalTat = 0;
    printf("Turnaround Time: ");
    for (int i = 0; i < n; i++)
    {
        printf("P%d = %d | ", P[i].iPID, P[i].iTaT);
        totalTat += P[i].iTaT;
    }
    printf("\n");
    printf("Average Turnaround Time: %.2f\n", (float)totalTat / n);
}

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    printf("Initial Processes:\n");
    printProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    // printf("After SORT:\n");
    // printProcess(iNumberOfProcess, Input);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    while (iTerminated < iNumberOfProcess)
    {
        while (iRemain > 0)
        {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            }
            else
                break;
        }
        if (iReady > 0)
        {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
            ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
    }
    printf("\n===== FCFS Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}