/* Name: Gabriel Beedles (4197665)
* Assignment: 2
* Filename: libsched.h
* Date Modified: 26/4/2015
*
* Purpose: Contains the declarations of functions to simulate various
* scheduling algorithms. The algorithms used are as follows:
* First Come First Served (FCFS)
* Shortest Job First (SJF)
* Shortest Remaining Time (SRT)
* Round Robbin (RR)
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Type declarations */
typedef enum { false, true } bool;

typedef struct Process {
	int id, burstTime, startTime, stopTime, waitTime, turnAroundTime,
		remainingTime;
} tProcess;

/* Function declarations */
/* Functions used for both scheduling algorithms */
void initialiseProcesses(tProcess processes[]);
void initialiseProcess(tProcess * process);
void printResults(tProcess processes[], int procNum, 
	double averageTurnAroundTime, double averageWaitTime);
void sortProcesses(tProcess arr[], int left, int right);
void procCpy(tProcess * dest, tProcess src);
/* Functions used differently for each algorithm */
int * fcfsSim1(tProcess processes[], int procNum);
int * sjfSim1(tProcess processes[], int procNum);
int * srtSim1(tProcess processes[], int procNum);
int * rrSim1(tProcess processes[], int procNum, int quanta);
int * fcfsSim2(tProcess processes[], int procNum);
int * sjfSim2(tProcess processes[], int procNum);
int * srtSim2(tProcess processes[], int procNum);
int * rrSim2(tProcess processes[], int procNum, int quanta);
