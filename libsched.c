/* Name: Gabriel Beedles (4197665)
* Assignment: 2
* Filename: libsched.c
* Date Modified: 26/4/2015
*
* Purpose: Contains the definitions of functions to simulate various scheduling
* algorithms. The algorithms used are as follows:
* First Come First Served (FCFS)
* Shortest Job First (SJF)
* Shortest Remaining Time (SRT)
* Round Robbin (RR)
*
*/

#include <libsched.h>

/* Functions used for both scheduling algorithms */
void initialiseProcesses(tProcess processes[]) {
	int i;
	for (i = 0; i < 64; ++i) {
		initialiseProcess(&processes[i]);
		processes[i].id = i + 1;
	}
} /* void initialiseProcesses(tProcess processes[]) */

void initialiseProcess(tProcess *process) {
	process->id = 1;
	process->burstTime = 0;
	process->startTime = 0;
	process->stopTime = 0;
	process->waitTime = 0;
	process->turnAroundTime = 0;
	process->remainingTime = 0;
} /* void initialiseProcess(tProcess *process) */

void printResults(tProcess processes[], int procNum, 
	double averageTurnAroundTime, double averageWaitTime) {
	/* Start and stop times */
	printf("Process\tBurst Time\tStart Time\tStop Time\n");
	int i;
	for (i = 0; i < procNum; ++i) {
		printf("P%d\t%d\t\t%d\t\t%d\n", processes[i].id, 
			processes[i].burstTime, processes[i].startTime, 
			processes[i].stopTime);
	}

	/* Wait times */
	printf("\nComputing Wait Times\n\n");
	for (i = 0; i < procNum; ++i) {
		printf("P%d\t%d\n", processes[i].id, processes[i].waitTime);
	}

	/* Turn around times */
	printf("\nComputing Turn Around Times\n\n");
	for (i = 0; i < procNum; ++i) {
		printf("P%d\t%d\n", processes[i].id, processes[i].turnAroundTime);
	}

	/* Averages */
	printf("\nComputing Average Times\n\n");
	printf("Average Wait Time: %f\n", averageWaitTime);
	printf("Average Turn Around Time: %f\n", averageTurnAroundTime);
} /* void printResults(tProcess processes[], int procNum, 
	double averageTurnAroundTime, double averageWaitTime) */

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
		if (i < j) {
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
} /* void sortProcesses(tProcess arr[], int left, int right) */

void procCpy(tProcess * dest, tProcess src) {
	dest->burstTime = src.burstTime;
	dest->id = src.id;
	dest->startTime = src.startTime;
	dest->stopTime = src.stopTime;
	dest->turnAroundTime = src.turnAroundTime;
	dest->waitTime = src.waitTime;
	dest->remainingTime = src.remainingTime;
} /* void procCpy(tProcess * dest, tProcess src) */

/* Functions used differently for each algorithm */
/* Functions from sched1 */
int * fcfsSim1(tProcess processes[], int procNum) {
	/* Calculate process times and calculate the first process */
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime, totalTime = 0;
	double totalWaitTime = 0.0, totalTurnAroundTime = 
		processes[0].turnAroundTime;

	/* Calculate totalTime for timeLine and fill in first process */
	int * timeLine;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTime += processes[i].burstTime;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;
	for (i = 1; i <= currentTime; ++i)
		timeLine[i] = processes[0].id;

	/* Calculate the remaining processes */
	for (i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		/* Move the time forward */
		currentTime += processes[i].burstTime;
		/* Calculate timeLine */
		int x;
		for (x = currentTime - processes[i].burstTime + 1; x <= currentTime; 
			++x)

			timeLine[x] = processes[i].id;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with First Come First Served (FCFS)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * fcfsSim1(tProcess processes[], int procNum) */

int * sjfSim1(tProcess processes[], int procNum) {
	/* Sort the array by burst time */
	sortProcesses(processes, 0, procNum - 1);

	/* Calculate process times and calculate the first process */
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime, totalTime = 0;
	double totalWaitTime = 0.0, totalTurnAroundTime = 
		processes[0].turnAroundTime;

	/* Calculate totalTime for timeLine and fill in first process */
	int * timeLine;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTime += processes[i].burstTime;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;
	for (i = 1; i <= currentTime; ++i)
		timeLine[i] = processes[0].id;

	/* Calculate the remaining processes */
	for (i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		/* Move the time forward */
		currentTime += processes[i].burstTime;
		/* Calculate timeLine */
		int x;
		for (x = currentTime - processes[i].burstTime + 1; x <= currentTime; 
			++x)

			timeLine[x] = processes[i].id;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with Shortest Job First (SJF)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * sjfSim1(tProcess processes[], int procNum) */

int * srtSim1(tProcess processes[], int procNum) {
	/* Sort the array by burst time */
	sortProcesses(processes, 0, procNum - 1);

	/* Calculate process times and calculate the first process */
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime;
	double totalWaitTime = 0.0, totalTurnAroundTime = 
		processes[0].turnAroundTime;

	/* Calculate totalTime for timeLine and fill in first process */
	int * timeLine, totalTime = 0;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTime += processes[i].burstTime;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;
	for (i = 1; i <= currentTime; ++i)
		timeLine[i] = processes[0].id;

	/* Calculate the remaining processes */
	for (i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		/* Move the time forward */
		currentTime += processes[i].burstTime;
		/* Calculate timeLine */
		int x;
		for (x = currentTime - processes[i].burstTime + 1; x <= currentTime;
			++x)
			
			timeLine[x] = processes[i].id;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with Shortest Remaining Time (SRT)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * srtSim1(tProcess processes[], int procNum) */

int * rrSim1(tProcess processes[], int procNum, int quanta) {
	/* Calculate the total time it will take for all processes */
	int totalTimeRemaining = 0, currentProcess = 0, currentCycle = 0, 
		currentTime = 0;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTimeRemaining += processes[i].remainingTime;

	/* Calculate totalTime for timeLine */
	int * timeLine, totalTime = totalTimeRemaining;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;

	while (totalTimeRemaining > 0) {
		/* Check if the process will complete before the cycle finishes */
		if (processes[currentProcess].remainingTime > quanta)
			currentCycle = quanta;
		else if (processes[currentProcess].remainingTime > 0)
			currentCycle = processes[currentProcess].remainingTime;
		else /* If we need to skip because this process is already completed */
			currentCycle = -1;

		if (currentCycle != -1) {
			/* Calculate the stopTime and turnAroundTime */
			processes[currentProcess].stopTime = currentTime + currentCycle;
			processes[currentProcess].turnAroundTime = 
				processes[currentProcess].stopTime;

			/* Update the remaining time of the currentProcess */
			processes[currentProcess].remainingTime -= currentCycle;
			/* Update the totalTimeRemaining */
			totalTimeRemaining -= currentCycle;
			/* Update the currentTime */
			currentTime += currentCycle;
			/* Calculate timeLine */
			for (i = currentTime - currentCycle + 1; i <= currentTime; ++i)
				timeLine[i] = processes[currentProcess].id;

			/* Update wait time for all processes except the current one */
			for (i = 0; i < procNum; ++i) {
				if (i != currentProcess)
					processes[i].waitTime += currentCycle;
			}
		}

		/* Move onto next process or back to the start */
		++currentProcess;
		if (currentProcess == procNum)
			currentProcess = 0;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0, 
		totalWaitTime = 0.0, totalTurnAroundTime = 0.0;
	for (i = 0; i < procNum; ++i) {
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
	}
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with Round Robin (RR)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * rrSim1(tProcess processes[], int procNum, int quanta) */

/* Functions from sched2 */
int *  fcfsSim2(tProcess processes[], int procNum) {
	/* Calculate process times and Calculate the first process */
	processes[0].stopTime = processes[0].burstTime;
	processes[0].turnAroundTime = processes[0].stopTime;
	int currentTime = processes[0].burstTime;
	double totalWaitTime = 0.0, totalTurnAroundTime = 
		processes[0].turnAroundTime;

	/* Calculate totalTime for timeLine and fill in first process */
	int * timeLine, totalTime = 0;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTime += processes[i].burstTime;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;
	for (i = 1; i <= currentTime; ++i)
		timeLine[i] = processes[0].id;

	/* Calculate the remaining processes */
	for (i = 1; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		/* Move the time forward */
		currentTime += processes[i].burstTime;
		/* Calculate timeLine */
		int x;
		for (x = currentTime - processes[i].burstTime + 1; x <= currentTime; 
			++x)
			
			timeLine[x] = processes[i].id;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with First Come First Served (FCFS)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * fcfsSim2(tProcess processes[], int procNum) */

int *  sjfSim2(tProcess processes[], int procNum) {
	int currentTime = 0, lastCurrentlyQueued = 0, currentProcess = 0;
	double totalWaitTime = 0.0, totalTurnAroundTime = 0.0;

	/* Calculate totalTime for timeLine */
	int * timeLine, totalTime = 0;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTime += processes[i].burstTime;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;

	while (lastCurrentlyQueued < procNum - 1) {
		/* Find all processes that are queued at currentTime */
		while (processes[lastCurrentlyQueued].startTime <= currentTime && 
			lastCurrentlyQueued < procNum)
			
			++lastCurrentlyQueued;
		--lastCurrentlyQueued; /* Step back one as the loop ends on the next 
							   time marker */

		/* Sort this section of the array */
		if (currentProcess < lastCurrentlyQueued)
			sortProcesses(processes, currentProcess, lastCurrentlyQueued);

		/* Calculate running the current process */
		processes[currentProcess].startTime = currentTime;
		processes[currentProcess].stopTime = 
			processes[currentProcess].burstTime + currentTime;
		processes[currentProcess].waitTime = currentTime;
		processes[currentProcess].turnAroundTime = 
			processes[currentProcess].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[currentProcess].turnAroundTime;
		totalWaitTime += processes[currentProcess].waitTime;
		/* Move the time forward */
		currentTime += processes[currentProcess].burstTime;
		/* Calculate timeLine */
		for (i = currentTime - processes[currentProcess].burstTime + 1; 
			i <= currentTime; ++i)
			
			timeLine[i] = processes[currentProcess].id;
		/* move onto the next process */
		++currentProcess;
	}
	/* All Processes are now sorted and queued
	Calculate the remaining processes */
	for (i = currentProcess; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		/* Move the time forward */
		currentTime += processes[i].burstTime;
		/* Calculate timeLine */
		int x;
		for (x = currentTime - processes[i].burstTime + 1; x <= currentTime;
			++x)
			
			timeLine[x] = processes[i].id;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with Shortest Job First (SJF)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * sjfSim2(tProcess processes[], int procNum) */

int *  srtSim2(tProcess processes[], int procNum) {
	int currentTime = 0, lastCurrentlyQueued = 0, currentProcess = 0, 
		currentCycle = 0;
	double totalWaitTime = 0.0, totalTurnAroundTime = 0.0;

	/* Calculate totalTime for timeLine and fill in first process */
	int * timeLine, totalTime = 0;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTime += processes[i].burstTime;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;

	while (lastCurrentlyQueued < procNum - 1) {
		/* Find all processes that are queued at currentTime */
		while (processes[lastCurrentlyQueued].startTime <= currentTime && 
			lastCurrentlyQueued < procNum)
			
			++lastCurrentlyQueued;
		--lastCurrentlyQueued; /* Step back one as the loop ends on the next
							   time marker */

		/* Sort this section of the array */
		if (currentProcess < lastCurrentlyQueued)
			sortProcesses(processes, currentProcess, lastCurrentlyQueued);

		/* Calculate how long this process will run for. ie. until the next
		process job arrives to be queued or until the process is completed */
		if (processes[currentProcess].remainingTime > 
			processes[lastCurrentlyQueued + 1].startTime)
			
			currentCycle = processes[lastCurrentlyQueued + 1].startTime - 
			currentTime;
		else
			currentCycle = processes[currentProcess].remainingTime;

		/* Calculate the stopTime and turnAroundTime */
		processes[currentProcess].stopTime = currentTime + currentCycle;
		processes[currentProcess].turnAroundTime = 
			processes[currentProcess].stopTime;

		/* Update the remaining time of the currentProcess */
		processes[currentProcess].remainingTime -= currentCycle;
		/* Update the currentTime */
		currentTime += currentCycle;
		/* Calculate timeLine */
		for (i = currentTime - currentCycle + 1; i <= currentTime; ++i)
			timeLine[i] = processes[currentProcess].id;

		/* Update wait time for all processes except the current one */
		for (i = currentProcess + 1; i < lastCurrentlyQueued; ++i)
			processes[i].waitTime += currentCycle;

		/* Check if the current process has completed */
		if (processes[currentProcess].remainingTime <= 0)
			++currentProcess;
	}
	/* All Processes are now sorted and queued
	Calculate the remaining processes */
	for (i = currentProcess; i < procNum; ++i) {
		processes[i].startTime = currentTime;
		processes[i].stopTime = processes[i].burstTime + currentTime;
		processes[i].waitTime = currentTime;
		processes[i].turnAroundTime = processes[i].stopTime;
		/* Add new wait and turn around times to totals */
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
		/* Move the time forward */
		currentTime += processes[i].burstTime;
		/* Calculate timeLine */
		int x;
		for (x = currentTime - processes[i].burstTime + 1; x <= currentTime;
			++x)
		
			timeLine[x] = processes[i].id;
	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0;
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with Shortest Remaining Time (SRT)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int *  srtSim2(tProcess processes[], int procNum) */

int *  rrSim2(tProcess processes[], int procNum, int quanta) {
	/* Calculate the total time it will take for all processes */
	int totalTimeRemaining = 0, currentProcess = -1, currentCycle = 0, 
		currentTime = 0, lastCurrentlyQueued = 0;
	int i;
	for (i = 0; i < procNum; ++i)
		totalTimeRemaining += processes[i].remainingTime;

	/* Calculate totalTime for timeLine */
	int * timeLine, totalTime = totalTimeRemaining;
	timeLine = malloc((totalTime + 1)*(sizeof(int)));
	timeLine[0] = totalTime;

	/* Loop through processes until all total time is 0 */
	while (totalTimeRemaining > 0) {
		/* Find all processes that are queued at currentTime */
		while (processes[lastCurrentlyQueued].startTime <= currentTime && 
			lastCurrentlyQueued < procNum)
			++lastCurrentlyQueued;
		--lastCurrentlyQueued; /* Step back one as the loop ends on the next 
							   time marker */

		/* Move onto next process or back to the start */
		++currentProcess;
		if (currentProcess > lastCurrentlyQueued)
			currentProcess = 0;

		/* Check if the process will complete before the cycle finishes */
		if (processes[currentProcess].remainingTime > quanta)
			currentCycle = quanta;
		else if (processes[currentProcess].remainingTime > 0)
			currentCycle = processes[currentProcess].remainingTime;
		else /* If we need to skip because this process is already completed */
			currentCycle = -1;

		if (currentCycle != -1) {
			/* Calculate the stopTime and turnAroundTime */
			processes[currentProcess].stopTime = currentTime + currentCycle;
			processes[currentProcess].turnAroundTime = 
				processes[currentProcess].stopTime;

			/* Update the remaining time of the currentProcess */
			processes[currentProcess].remainingTime -= currentCycle;
			/* Update the totalTimeRemaining */
			totalTimeRemaining -= currentCycle;
			/* Update the currentTime */
			currentTime += currentCycle;
			/* Calculate timeLine */
			for (i = currentTime - currentCycle + 1; i <= currentTime; ++i)
				timeLine[i] = processes[currentProcess].id;

			/* Update wait time for all processes except the current one */
			for (i = 0; i < lastCurrentlyQueued; ++i) {
				if (i != currentProcess && processes[i].remainingTime > 0)
					processes[i].waitTime += currentCycle;
			}
		}

	}

	/* Calculate average wait and turn around times */
	double averageWaitTime = 0.0, averageTurnAroundTime = 0.0, 
		totalWaitTime = 0.0, totalTurnAroundTime = 0.0;
	for (i = 0; i < procNum; ++i) {
		totalTurnAroundTime += processes[i].turnAroundTime;
		totalWaitTime += processes[i].waitTime;
	}
	averageTurnAroundTime = totalTurnAroundTime / procNum;
	averageWaitTime = totalWaitTime / procNum;

	/* Print results */
	printf("Scheduled Jobs with Round Robin (RR)\n\n");
	printResults(processes, procNum, averageTurnAroundTime, averageWaitTime);

	return timeLine;
} /* int * rrSim2(tProcess processes[], int procNum, int quanta) */
