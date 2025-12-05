/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: RoundRobin.c
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
#define QUANTUM 5

#define SIZE_INIT 100

typedef struct {
    int pid;
    int start;
    int finish;
} GanttSlot;

typedef struct{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
    int iRemaining;
} PCB;

int isArrival[30];

void inputProcess(int n, PCB P[])
{
    srand(time(NULL));
    printf("Generating %d process... (┬┬﹏┬┬)\n", n);
    for (int i = 0; i < n; i++)
    {
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;
        P[i].iBurst = (rand() % 11) + 2;
        P[i].iStart = -1; 
        P[i].iFinish = 0;
        P[i].iResponse = -1;
        P[i].iTaT = 0;
        P[i].iRemaining = P[i].iBurst;
        printf("Process ID: %d, Arrival Time: %d, Burst Time: %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void printProcess(int n, PCB P[]) {
    printf("| PID | Arrival | Burst |\n");
    for (int i = 0; i < n; i++) {
        printf("| P%d  |\t%d\t|  %d\t|\n",
            P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void exportGanttChartSlots(GanttSlot G[], int gCount)
{
    if (gCount == 0) return;
    printf("\n===== Gantt Chart =====\n");

    printf("Time:      ");
    for (int i = 0; i < gCount; i++) {
        printf("%-8d", G[i].start);
    }
    printf("%d\n", G[gCount - 1].finish);

    printf("           ");
    for (int i = 0; i < gCount; i++) {
        printf("--------");
    }
    printf("-\n");
    printf("Process:   ");
    for (int i = 0; i < gCount; i++) {
        printf("| P%-5d", G[i].pid);
    }
    printf("|\n");
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 1;
}

int partition (PCB P[], int low, int high, int iCriteria) {
    PCB pivot = P[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (iCriteria == SORT_BY_ARRIVAL) {
            if ((P[j].iArrival < pivot.iArrival) || (P[j].iArrival == pivot.iArrival && P[j].iBurst < pivot.iBurst)) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }

        if (iCriteria == SORT_BY_PID && P[j].iPID < pivot.iPID) {
            i++;
            swapProcess(&P[i], &P[j]);
        }

        if (iCriteria == SORT_BY_BURST && P[j].iBurst < pivot.iBurst) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int totalWaiting = 0;
    printf("Waiting Time: ");
    for (int i = 0; i < n; i++) {
        printf("P%d = %d | ", P[i].iPID, P[i].iWaiting);
        totalWaiting += P[i].iWaiting;
    }
    printf("\n");
    printf("Average Waiting Time: %.2f\n", (float)totalWaiting / n);
}

void calculateART(int n, PCB P[]) {
    int totalResponse = 0;
    printf("Response Time: ");
    for (int i = 0; i < n; i++) {
        printf("P%d = %d | ", P[i].iPID, P[i].iResponse);
        totalResponse += P[i].iResponse;
    }
    printf("\n");
    printf("Average Response Time: %.2f\n", (float)totalResponse / n);
}

void calculateATaT(int n, PCB P[]) {
    int totalTaT = 0;
    printf("Turnaround Time: ");
    for (int i = 0; i < n; i++) {
        printf("P%d = %d | ", P[i].iPID, P[i].iTaT);
        totalTaT += P[i].iTaT;
    }
    printf("\n");
    printf("Average Turnaround Time: %.2f\n", (float)totalTaT / n);
}

int main()
{
    GanttSlot Gantt[SIZE_INIT];
    int gCount = 0;

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

    if (iRemain > 0) {
        pushProcess(&iReady, ReadyQueue, Input[0]);
        removeProcess(&iRemain, 0, Input);
    }

  int time = 0;

    while (iTerminated < iNumberOfProcess)
    {
        while (iRemain > 0 && Input[0].iArrival <= time) {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
        }
        if (iReady == 0) {
            if (iRemain > 0) {
                if (time < Input[0].iArrival) time = Input[0].iArrival;
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            } else break;
        }

        PCB current = ReadyQueue[0];
        removeProcess(&iReady, 0, ReadyQueue);

        if (current.iRemaining == current.iBurst) {
            if (time < current.iArrival) time = current.iArrival;
            current.iStart = time;
            current.iResponse = current.iStart - current.iArrival;
        } else {
            if (time < current.iArrival) time = current.iArrival;
        }
        int runTime = (current.iRemaining > QUANTUM) ? QUANTUM : current.iRemaining;

        Gantt[gCount].pid = current.iPID;
        Gantt[gCount].start = time;
        Gantt[gCount].finish = time + runTime;
        gCount++;

        time += runTime;
        current.iRemaining -= runTime;

        if (current.iRemaining == 0) {
            current.iFinish = time;
            current.iTaT = current.iFinish - current.iArrival;
            current.iWaiting = current.iStart - current.iArrival;
            pushProcess(&iTerminated, TerminatedArray, current);
        } else {
            while (iRemain > 0 && Input[0].iArrival <= time) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            }
            pushProcess(&iReady, ReadyQueue, current);
        }
    }

    printf("\n===== Round Robin Scheduling (quantum = %d) =====\n", QUANTUM);
    exportGanttChartSlots(Gantt, gCount);

    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);

    calculateAWT(iTerminated, TerminatedArray);
    calculateART(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}
