/* Name: Gabriel Beedles (4197665)
* Assignment: 2
* Filename: sched2.c
* Date Modified: 26/4/2015
*
* Purpose: Used to test implementation of various scheduling algorithms saved
* as libraries. The algorithms used are as follows:
* First Come First Served (FCFS)
* Shortest Job First (SJF)
* Shortest Remaining Time (SRT)
* Round Robbin (RR)
*
*/

#include <libsched.h>
#include <libgrapher.h>

int main() {

	char algorithm[5] = { '\0' }, imageFilename[128] = { '\0' };
	int quanta = 0, tempBurst = 0, procNum = 0, readTemp = 0, tempStart = 0, 
		totalTime = 0, lastStart = 0;
	bool goodRead = true;
	tProcess processes[64];
	int * timeLine = NULL;

	initialiseProcesses(processes);

	printf("Welcome to Job Scheduler Computer\nEnter a filename for the output ");
	printf("image:\n");
	scanf("%s", imageFilename);

	printf("What algorithm would you like to use?\n");
	scanf("%s", algorithm);

	if (strcmp(algorithm, "RR") == 0) {
		printf("What is the time quanta for RR scheduling?\n");
		scanf("%d", &quanta);
	}

	while (goodRead && procNum < 64) {
		printf("Process %d (P%d) - What is the burst time & start time?\n", 
			procNum + 1, procNum + 1);
		readTemp = scanf("%d %d", &tempBurst, &tempStart);
		if (readTemp > 0 && tempStart <= totalTime && tempStart >= lastStart) {
			processes[procNum].burstTime = tempBurst;
			processes[procNum].remainingTime = tempBurst;
			processes[procNum].startTime = tempStart;
			totalTime += tempBurst;
			lastStart = tempStart;
			++procNum;
		}
		else if (tempStart > totalTime || tempStart< lastStart) {
			printf("ERROR: The start time cannot be greater than the sum of all ");
			printf("previous burst times and it must be at least equal to the last");
			printf("process start time.\nPlease try again.\n\n");
		}
		else
			goodRead = false;
	}

	if (strcmp(algorithm, "RR") == 0) {
		timeLine = rrSim2(processes, procNum, quanta);
	}
	else if (strcmp(algorithm, "FCFS") == 0) {
		timeLine = fcfsSim2(processes, procNum);
	}
	else if (strcmp(algorithm, "SJF") == 0) {
		timeLine = sjfSim2(processes, procNum);
	}
	else if (strcmp(algorithm, "SRT") == 0) {
		timeLine = srtSim2(processes, procNum);
	}
	else {
		printf("%s is not an available option. Program will now quit.", algorithm);
	}

	outputImage(timeLine, procNum, imageFilename);

	free(timeLine);

	return 0;
}
