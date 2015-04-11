/* Name: Gabriel Beedles (4197665)
* Assignment: 2
* Filename: sched1.c
* Date Modified: 19/4/2015
*
* Purpose: Contains a simulation for various scheduling algorithms when all
* processes arrive at the same time. The algorithms used are as follows:
* First Come First Served (FCFS)
* Shortest Job First (SJF)
* Shortest Remaining Time (SRT)
* Round Robbin (RR)
*
*/

// Visual Studio compile fix
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

// Type declarations
typedef enum { false, true } bool;

typedef struct Process {
	int id, burstTime, startTime, stopTime, waitTime, turnAroundTime, remainingTime;
} tProcess;

// Function declarations
void initialiseProcesses(tProcess processes[]);
void initialiseProcess(tProcess * process);
void const printResults(tProcess processes[], int procNum, double averageTurnAroundTime, double averageWaitTime);
void sortProcesses(tProcess arr[], int left, int right);
void procCpy(tProcess * dest, tProcess src);
void const fcfsSim(tProcess processes[], int procNum);
void const sjfSim(tProcess processes[], int procNum);
void const srtSim(tProcess processes[], int procNum);
void const rrSim(tProcess processes[], int procNum, int quanta);

int main() {

	char algorithm[4] = {'\0'};
	int quanta = 0, tempBurst = 0, procNum = 0, readTemp = 0;
	bool goodRead = true;
	tProcess processes[64];

	initialiseProcesses(processes);

	printf("Welcome to Job Scheduler Computer\nWhat algorithm would you like to use?\n");
	scanf("%s", algorithm);

	if (strcmp(algorithm, "RR") == 0) {
		printf("What is the time quanta for RR scheduling?\n");
		scanf("%d", &quanta);
	}

	while (goodRead && procNum < 64) {
		printf("Process %d (P%d) - What is the burst time?\n", procNum+1, procNum+1);
		readTemp = scanf("%d", &tempBurst);
		if (readTemp > 0) {
			processes[procNum].burstTime = tempBurst;
			processes[procNum].remainingTime = tempBurst;
			++procNum;
		}
		else
			goodRead = false;
	}

	if (strcmp(algorithm, "RR") == 0) {
		rrSim(processes, procNum, quanta);
	}
	else if (strcmp(algorithm, "FCFS") == 0) {
		fcfsSim(processes, procNum);
	}
	else if (strcmp(algorithm, "SJF") == 0) {
		sjfSim(processes, procNum);
	}
	else if (strcmp(algorithm, "SRT") == 0) {
		srtSim(processes, procNum);
	}
	else {
		printf("%s is not an available option. Program will now quit.", algorithm);
	}

	scanf("%s", algorithm);

	return 0;
}

// Function definitions
void initialiseProcesses(tProcess processes[]) {
	for (int i = 0; i < 64; ++i) {
		initialiseProcess(&processes[i]);
		processes[i].id = i + 1;
	}
} // void initialiseProcesses(tProcess processes[])

void initialiseProcess(tProcess *process) {
	process->id = 1;
	process->burstTime = 0;
	process->startTime = 0;
	process->stopTime = 0;
	process->waitTime = 0;
	process->turnAroundTime = 0;
	process->remainingTime = 0;
} // void initialiseProcess(tProcess *process)

void const printResults(tProcess processes[], int procNum, double averageTurnAroundTime, double averageWaitTime) {
	// Start and stop times
	printf("Process\tBurst Time\tStart Time\tStop Time\n");
	for (int i = 0; i < procNum; ++i) {
		printf("P%d\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].burstTime, processes[i].startTime, processes[i].stopTime);
	}

	// Wait times
	printf("\nComputing Wait Times\n\n");
	for (int i = 0; i < procNum; ++i) {
		printf("P%d\t%d\n", processes[i].id, processes[i].waitTime);
	}

	// Turn around times
	printf("\nComputing Turn Around Times\n\n");
	for (int i = 0; i < procNum; ++i) {
		printf("P%d\t%d\n", processes[i].id, processes[i].turnAroundTime);
	}

	// Averages
	printf("\nComputing Average Times\n\n");
	printf("Average Wait Time: %f\n", averageWaitTime);
	printf("Average Turn Around Time: %f\n", averageTurnAroundTime);
} // void printResults(tProcess processes[], int procNum, double averageTurnAroundTime, double averageWaitTime)

void sortProcesses(tProcess arr[], int left, int right) {
	int i = left, j = right;
	tProcess tmp = arr[0];
	initialiseProcess(&tmp);
	tProcess pivot = arr[(left + right) / 2];

	while (i <= j) {
		while (arr[i].remainingTime < pivot.remainingTime)
			++i;
		while (arr[j].remainingTime > pivot.remainingTime)
			--j;
		if (i <= j) {
			procCpy(&tmp, arr[i]);
			procCpy(&arr[i], arr[j]);
			procCpy(&arr[j], tmp);
			++i;
			--j;
		}
	}

	if (left < j)
		sortProcesses(arr, left, j);
	if (i < right)
		sortProcesses(arr, i, right);
} // void sortProcesses(tProcess arr[], int left, int right)

void procCpy(tProcess * dest, tProcess src) {
	dest->burstTime = src.burstTime;
	dest->id = src.id;
	dest->startTime = src.startTime;
	dest->stopTime = src.stopTime;
	dest->turnAroundTime = src.turnAroundTime;
	dest->waitTime = src.waitTime;
	dest->remainingTime = src.remainingTime;
} // void procCpy(tProcess * dest, tProcess src)

void const fcfsSim(tProcess processes[], int procNum) {
	// Calculate process times
	// Calculate the first process
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime;
	double totalWaitTime = 0.0, totalTurnAroundTime = processes[0].turnAroundTime;

	// Calculate the remaining processes
	for (int i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		// Add new wait and turn around times to totals
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		// Move the time forward
		currentTime += processes[i].burstTime;
	}

	// Calculate average wait and turn around times
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	// Print results
	printf("Scheduled Jobs with First Come First Served (FCFS)\n\n"); // Title
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);
} // void fcfsSim(tProcess processes[], int procNum)

void const sjfSim(tProcess processes[], int procNum) {
	// Sort the array by burst time
	sortProcesses(processes, 0, procNum-1);

	// Calculate process times
	// Calculate the first process
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime;
	double totalWaitTime = 0.0, totalTurnAroundTime = processes[0].turnAroundTime;

	// Calculate the remaining processes
	for (int i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		// Add new wait and turn around times to totals
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		// Move the time forward
		currentTime += processes[i].burstTime;
	}

	// Calculate average wait and turn around times
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	// Print results
	printf("Scheduled Jobs with Shortest Job First (SJF)\n\n"); // Title
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);
} // void sjfSim(tProcess processes[], int procNum)

void const srtSim(tProcess processes[], int procNum) {
	// Sort the array by burst time
	sortProcesses(processes, 0, procNum - 1);

	// Calculate process times
	// Calculate the first process
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime;
	double totalWaitTime = 0.0, totalTurnAroundTime = processes[0].turnAroundTime;

	// Calculate the remaining processes
	for (int i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		// Add new wait and turn around times to totals
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		// Move the time forward
		currentTime += processes[i].burstTime;
	}

	// Calculate average wait and turn around times
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	// Print results
	printf("Scheduled Jobs with Shortest Remaining Time (SRT)\n\n"); // Title
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);
} // void srtSim(tProcess processes[], int procNum)

void const rrSim(tProcess processes[], int procNum, int quanta) {
	// Calculate the total time it will take for all processes
	int totalTimeRemaining = 0, currentProcess = 0, currentCycle = 0, currentTime = 0;
	for (int i = 0; i < procNum; ++i)
		totalTimeRemaining += processes[i].remainingTime;

	// Loop through processes until all total time is 0
	while (totalTimeRemaining > 0) {
		// Prints for testing
		printf("currentTime is %d\n", currentTime);
		printf("currentProcess is %d\n", currentProcess);

		// Check if the process will complete before the cycle finishes
		if (processes[currentProcess].remainingTime > quanta)
			currentCycle = quanta;
		else if (processes[currentProcess].remainingTime > 0)
			currentCycle = processes[currentProcess].remainingTime;
		else // If we need to skip because this process is already completed
			currentCycle = -1;

		if (currentCycle != -1) {
			// Calculate the stopTime and turnAroundTime
			processes[currentProcess].stopTime = currentTime + currentCycle;
			processes[currentProcess].turnAroundTime = processes[currentProcess].stopTime;

			// Update the remaining time of the currentProcess
			processes[currentProcess].remainingTime -= currentCycle;
			// Update the totalTimeRemaining
			totalTimeRemaining -= currentCycle;
			// Update the currentTime
			currentTime += currentCycle;

			// Update wait time for all processes except the current one
			for (int i = 0; i < procNum; ++i) {
				if (i != currentProcess)
					processes[i].waitTime += currentCycle;
			}
		}

		// Move onto next process or back to the start
		++currentProcess;
		if (currentProcess == procNum)
			currentProcess = 0;
	}

	// Calculate average wait and turn around times
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0, totalWaitTime = 0.0, totalTurnAroundTime = 0.0;
	for (int i = 0; i < procNum; ++i) {
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
	}
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	// Print results
	printf("Scheduled Jobs with Round Robin (RR)\n\n"); // Title
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);
} // void rrSim(tProcess processes[], int procNum, int quanta)