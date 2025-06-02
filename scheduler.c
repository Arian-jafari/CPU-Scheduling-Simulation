#include "scheduler.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int compareByArrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrivalTime - p2->arrivalTime;
}


Metrics fcfs_metrics(Process proc[], int n) {
    Process procs[n];
    memcpy(procs, proc, sizeof(Process) * n);
    qsort(procs, n, sizeof(Process), compareByArrival);

    int time = 0;
    float totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;

    for (int i = 0; i < n; ++i) {
        if (time < procs[i].arrivalTime) time = procs[i].arrivalTime;
        procs[i].startTime = time;
        procs[i].completionTime = time + procs[i].burstTime;
        time += procs[i].burstTime;

        int turnaround = procs[i].completionTime - procs[i].arrivalTime;
        int waiting = turnaround - procs[i].burstTime;
        int response = procs[i].startTime - procs[i].arrivalTime;

        totalTurnaround += turnaround;
        totalWaiting += waiting;
        totalResponse += response;
    }

    Metrics m = {
        totalTurnaround / n,
        totalWaiting / n,
        totalResponse / n
    };
    return m;
}

Metrics sjf_metrics(Process proc[], int n) {
    Process procs[n];
    memcpy(procs, proc, sizeof(Process) * n);

    int completed = 0, time = 0;
    float totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;
    int isDone[n];
    memset(isDone, 0, sizeof(isDone));

    while (completed < n) {
        int idx = -1;
        int minBurst = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (!isDone[i] && procs[i].arrivalTime <= time && procs[i].burstTime < minBurst) {
                minBurst = procs[i].burstTime;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        procs[idx].startTime = time;
        procs[idx].completionTime = time + procs[idx].burstTime;
        time += procs[idx].burstTime;
        isDone[idx] = 1;
        completed++;

        int turnaround = procs[idx].completionTime - procs[idx].arrivalTime;
        int waiting = turnaround - procs[idx].burstTime;
        int response = procs[idx].startTime - procs[idx].arrivalTime;

        totalTurnaround += turnaround;
        totalWaiting += waiting;
        totalResponse += response;
    }

    Metrics m = {
        totalTurnaround / n,
        totalWaiting / n,
        totalResponse / n
    };
    return m;
}

Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    Process procs[n];
    memcpy(procs, proc, sizeof(Process) * n);

    int time = 0, completed = 0;
    int remaining[n];
    int responseSet[n];
    for (int i = 0; i < n; ++i) {
        remaining[i] = procs[i].burstTime;
        procs[i].startTime = -1;
        responseSet[i] = 0;
    }

    float totalTurnaround = 0, totalWaiting = 0, totalResponse = 0;
    int queue[1000], front = 0, rear = 0;
    int visited[n];
    memset(visited, 0, sizeof(visited));

    for (int i = 0; i < n; ++i) {
        if (procs[i].arrivalTime == time) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            time++;
            for (int i = 0; i < n; ++i) {
                if (!visited[i] && procs[i].arrivalTime <= time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];

        if (procs[idx].startTime == -1) {
            procs[idx].startTime = time;
            totalResponse += procs[idx].startTime - procs[idx].arrivalTime;
        }

        int slice = remaining[idx] < timeQuantum ? remaining[idx] : timeQuantum;
        remaining[idx] -= slice;
        time += slice;

        for (int i = 0; i < n; ++i) {
            if (!visited[i] && procs[i].arrivalTime <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (remaining[idx] > 0) {
            queue[rear++] = idx;
        } else {
            procs[idx].completionTime = time;
            completed++;
            int turnaround = procs[idx].completionTime - procs[idx].arrivalTime;
            int waiting = turnaround - procs[idx].burstTime;

            totalTurnaround += turnaround;
            totalWaiting += waiting;
        }
    }

    Metrics m = {
        totalTurnaround / n,
        totalWaiting / n,
        totalResponse / n
    };
    return m;
}
