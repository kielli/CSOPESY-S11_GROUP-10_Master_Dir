
# Multi-Tasking OS
This C++ Program is the implementation of the process scheduler with multiple CPU and memory management. It implements various scheduling algorithms such as First-Come-First-Serve and Round-Robin. It provides commandline interface for interating with the program. 

## CSOPESY-S11 Group 10
Contributors:
1. Abenoja, Amelia Joyce
2. Cipriaso, James Kielson
3. Hallar, Francine Marie
4. Hong, Letty
5. Pe, Gyan Josh 

## Technical Report
Should the Technical Report of the system containing command recognition, process representation with an emphasis on memory representation, scheduler implementation, memory management (flat memory and paging) be inaccessible, it can also be viewed at: [CSOPESY MCO2 - Technical Report](https://docs.google.com/presentation/d/1lY2XZvA5WO9QHzWuciFzvqjz-Q1Ku1In9cjHXJ2Plrc/edit?usp=sharing)

## Features
1. Multiple CPU simulation
2. Process creation and management
3. Various scheduling algorithms (FCFS, RR)
4. Real-time process monitoring
5. System utilization reporting
6. Configurable simulation parameters

## Program Setup
1. Ensure you have a C++ compiler that supports C++11 or later.
2. Clone the repository or extract the project files.
```sh
git clone https://github.com/kielli/CSOPESY-S11_GROUP-10_Master_Dir.git
```
3. There are two ways to compile the run the program:
    - Compile the project using your preferred C++ compiler or IDE. If using the Visual Studio Code, make sure to use C++ 20 in order for the project to work.
    - Manually run the program on your local machine.
        1. Navigate to the Project Directory where the `main.cpp` and `config.txt` are located:
        ```sh
        cd CSOPESY-S11_GROUP-10_Master_Dir
        ```
        2. Now, run the the compile executable.
        ```sh
        main
        ```

## Configuration Setup
Edit the `config.txt` file to adjust the parameters:
# Parameters for MCO1 – OS Scheduler

| Parameter               | Description                                                                                                                                         |
|-------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------|
| `num-cpu`              | Number of CPUs available. The range is [1, 128].                                                                                                   |
| `scheduler`            | The scheduler algorithm: “fcfs” or “rr”.                                                                                                           |
| `quantum-cycles`       | The time slice is given for each processor if a round-robin scheduler is used. Has no effect on other schedulers. The range is [1, 2^32].           |
| `batch-process-freq`   | The frequency of generating processes in the “scheduler-test” command in CPU cycles. The range is [1, 2^32]. If one, a new process is generated at the end of each CPU cycle. |
| `min-ins`              | The minimum instructions/command per process. The range is [1, 2^32].                                                                              |
| `max-ins`              | The maximum instructions/command per process. The range is [1, 2^32].                                                                              |
| `delays-per-exec`      | Delay before executing the next instruction in CPU cycles. The delay is a “busy-waiting” scheme where the process remains in the CPU. If zero, each instruction is executed per CPU cycle. |

---

## New Parameters for MCO2 – Multitasking OS

_All memory ranges are [2, 2^32] and powers of 2 format._

| Parameter         | Description                                                                                                                                          |
|-------------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| `max-overall-mem` | Maximum memory available in KB.                                                                                                                     |
| `mem-per-frame`   | The size of memory in KB per frame. This is also the memory size per page.                                                                           |
|                   | The total number of frames is equal to `max-overall-mem / mem-per-frame`.                                                                            |
|                   | If `max-overall-mem == mem-per-frame`, then the emulator will use a flat memory allocator.                                                          |
| `min-mem-proc`    | Memory required for each process.                                                                                                                   |
| `max-mem-proc`    | Let P be the number of pages required by a process and M is the rolled value between `min-mem-proc` and `max-mem-proc`. P can be computed as `M / mem-per-frame`. |


## Usage
Main commands:
- initialize: Initializes the program before anything else.
- screen -s [process_name]: Create and switch to a new process
- screen -r [process_name]: Switch to an existing process
- screen -ls: List all active processes
- scheduler-test: Start automatic process creation
- scheduler-stop: Stop automatic process creation
- report-util: Generate system utilization report
- marquee-console: Switch to marquee display mode
- process-smi: provides a summarized view of the available/used memory, as well as the list of processes and memory occupied. This is similar to the “nvidia-smi”  command.
- vmstat: provides a detailed view of the active/inactive processes, available/used memory, and pages. 
- clear: Clears the screen
- exit: Exit the current console or the program

Process-specific commands:
- process-smi: Show detailed process metrics
- exit: Return to the main console

## Basic Program Flow:
1. Launch the Program
2. Initialize the program using `initialize` command.
    - This loads the configuration of the program from config.txt to setup the CPUs, schedulers and memory allocators.
3. After initializing, use other commands.

```sh
Enter command: initialize
Enter command: scheduler-test
Enter command: shceduler-stop
Enter command: screen -ls
```


Version:
--------
1.0.0

Last Updated:
-------------
DEC. 1, 2024
