# CPU-Scheduling-Simulation
A C-based simulation of various CPU scheduling algorithms, designed to model and analyze process scheduling strategies in operating systems.

## File Structure
scheduler.c: Contains the main simulation logic.
scheduler.h: Header file with function declarations and data structures.
scheduler_test.c: Test cases for validating scheduling algorithms.
Makefile: Build configuration for compiling the project.

## Features
Implemented Algorithms:
- First-Come, First-Served (FCFS)
- Shortest Job First (SJF)
- Round Robin (RR)

Discrete Event Simulation: Models process scheduling events over time.
Performance Metrics: Calculates and displays metrics such as waiting time, turnaround time, and CPU utilization.
Modular Design: Organized codebase with separate modules for scheduling logic, testing, and build configuration.

## Input File Format
The input file should list processes with their respective structure defined in scheduler.h file. Each line represents a process: <Process_ID> <Arrival_Time> <Burst_Time> <remainingTime> <startTime> <completionTime>
We also need the number of processes and quantum time if Round Robin algorithm is used, then this information is passed to run_test function and respective output is exposed.
Example:
{1, 0, 10, 0, 0, 0},
{2, 1,  5, 0, 0, 0},
{3, 2,  8, 0, 0, 0}

## Testing
A test suite is provided to verify the correctness of the scheduling algorithms: make test
This will compile and run scheduler_test.c, displaying the results of predefined test cases.

## Implementation Details
### Data Structures
Process Structure: Each process is represented by a structure containing:
pid: Process ID
arrival_time: Time at which the process arrives
burst_time: CPU burst time required by the process
remaining_time: Time remaining for execution (used in RR)
waiting_time: Time the process has waited in the ready queue
turnaround_time: Total time taken from arrival to completion
Ready Queue: Implemented as a linked list or array, depending on the algorithm, to manage the order of process execution.

### Scheduling Algorithms
First-Come, First-Served (FCFS)
Type: Non-preemptive
Mechanism:
Processes are sorted based on their arrival times.
The scheduler selects the process with the earliest arrival time.
Each process runs to completion before the next process is scheduled.
Implementation Highlights:
Iterate through the list of processes, updating the current time as each process completes.
Calculate waiting and turnaround times accordingly.

Shortest Job First (SJF)
Type: Non-preemptive
Mechanism:
At each scheduling decision point, select the process with the shortest burst time among the processes that have arrived.
The selected process runs to completion.
Implementation Highlights:
Maintain a list of arrived processes.
Sort or select the process with the minimum burst time.
Update current time, waiting time, and turnaround time after each process execution.

Round Robin (RR)
Type: Preemptive
Mechanism:
Each process is assigned a fixed time quantum.
Processes are executed in a circular order.
If a process's remaining burst time exceeds the time quantum, it is preempted and placed at the end of the ready queue.
Implementation Highlights:
Use a queue to manage the order of process execution.
Decrement the remaining time of the current process by the time quantum or the remaining time, whichever is smaller.
If the process is not completed, enqueue it back; otherwise, calculate its waiting and turnaround times.

## Examples of the values of criteria for each algorithm for given inputs 
### example 1
#### input:
Process test[5] = {
    {1, 0, 3, 0, 0, 0},
    {2, 2, 6, 0, 0, 0},
    {3, 4, 4, 0, 0, 0},
    {4, 6, 5, 0, 0, 0},
    {5, 8, 2, 0, 0, 0}
};
int n = 5;
int quantum = 3;

#### output:
FCFS:  Turnaround: 8.60, Waiting: 4.60, Response: 4.60
SJF:  Turnaround: 7.60, Waiting: 3.60, Response: 3.60
RR (Quantum = 3):  Turnaround: 10.60, Waiting: 6.60, Response: 2.60

### example 2
#### input:
Process test[4] = {
    {1, 0, 8, 0, 0, 0},
    {2, 1, 4, 0, 0, 0},
    {3, 2, 9, 0, 0, 0},
    {4, 3, 5, 0, 0, 0}
};
int n = 4;
int quantum = 3;

#### output:
FCFS:  Turnaround: 15.25, Waiting: 8.75, Response: 8.75
SJF:  Turnaround: 14.25, Waiting: 7.75, Response: 7.75
RR (Quantum = 3):  Turnaround: 20.00, Waiting: 13.50, Response: 3.00
         
