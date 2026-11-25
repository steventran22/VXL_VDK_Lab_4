# MCU LAB4: A COOPERATIVE SCHEDULER

## Introduction

I am a student of the Microcontroller course at HCMUT. In this lab, I continued working with STM32CubeIDE and Proteus simulation, using the STM32F103C6 microcontroller.

Lab 4 focuses on a fundamental concept in embedded systems: **cooperative multitasking**. The project involves designing and implementing a non-preemptive scheduler from the ground up. Key challenges included creating a delta-time linked list for efficient O(1) time updates, managing dynamic memory with `malloc()` and `free()`, and integrating a watchdog timer for system robustness. Through this lab, I learned how to manage multiple "concurrent" tasks on a bare-metal system, the trade-offs of different scheduling data structures, and the importance of memory management in long-running embedded applications.

## Core Features

This scheduler provides a lightweight framework for running multiple tasks on an STM32 microcontroller.

- **Cooperative, Non-preemptive Multitasking:** Tasks run to completion or voluntarily yield control.
- **Delta-Time Linked List:** The core data structure is a time-ordered linked list where each node stores the time delay relative to the previous one. This allows for an **O(1) time complexity** for the `SCH_Update()` function, making it highly efficient and suitable for use within a timer ISR.
- **Dynamic Task Management:** Tasks can be added (`SCH_Add_Task`) or removed (`SCH_Delete_Task`) at any point during runtime, thanks to dynamic memory allocation.
- **Periodic and One-Shot Tasks:** The scheduler supports both tasks that run once after a delay and tasks that repeat with a fixed period.
- **Error Reporting System:** A simple, non-intrusive error reporting mechanism displays error codes on a 7-segment LED for easy debugging. This feature can be disabled for production builds.
- **Watchdog Integration:** An Independent Watchdog (IWDG) is used to automatically reset the system in case of a software freeze, enhancing overall stability.

---

## Project Structure

The project is organized into several key modules:

- `Core/Inc/scheduler.h`: Defines the public API and data structures for the scheduler.
- `Core/Src/scheduler.c`: Contains the core implementation of the scheduler logic, including task insertion, deletion, updating, and dispatching.
- `Core/Inc/software_timer.h` & `Core/Src/software_timer.c`: A generic software timer module used for testing purposes (e.g., triggering task deletion after a set time).
- `Core/Inc/tasks.h` & `Core/Src/tasks.c`: Contains the definitions of the actual application tasks (e.g., toggling LEDs).
- `Core/Src/main.c`: The main application entry point, responsible for initializing the system, loading initial tasks, and running the main scheduler loop.

The main code for the scheduler and its related modules is organized in the `Core/` directory as follows:

```text
Core/
│
├── Src/                        # Main embedded source code
│   ├── scheduler.c             # Core scheduler logic (add, delete, dispatch, update)
│   ├── tasks.c                 # Application-specific task definitions
│   ├── software_timer.c        # Auxiliary software timer module for testing
│   ├── display7seg.c           # Driver for the 7-segment display
│   ├── main.c                  # Main program entry point, test case selection
│   ├── stm32f1xx_hal_msp.c     # HAL MSP initialization
│   ├── stm32f1xx_it.c          # Interrupt handlers (e.g., for TIM2)
│   ├── syscalls.c              # System call stubs
│   ├── sysmem.c                # System memory allocation
│   └── system_stm32f1xx.c      # System clock/config
│
├── Inc/                        # Header files for each module
│   ├── scheduler.h
│   ├── tasks.h
│   ├── software_timer.h
│   ├── display7seg.h
│   ├── common.h                # Common definitions (error codes, constants)
│   ├── main.h
│   ├── stm32f1xx_hal_conf.h
│   └── stm32f1xx_it.h
│
└── Startup/
    └── startup_stm32f103c6ux.s   # MCU startup assembly code
```

---
## How to Use the Scheduler

This scheduler is designed to be straightforward to integrate into any STM32 project. The basic workflow involves initializing the scheduler, adding tasks, and then running the update and dispatch functions from the correct contexts.

### Core Workflow

1.  **Initialization**: Call `SCH_Init()` once at the beginning of your `main()` function before the main loop. This prepares the scheduler for use.
2.  **Adding Tasks**: Use `SCH_Add_Task()` to add your functions to the scheduler. You can add as many tasks as needed, limited only by available memory.
3.  **Updating Time**: Call `SCH_Update()` from a periodic timer interrupt (e.g., `TIM2_IRQHandler`). This function is the "heartbeat" of the scheduler and must be called at a regular interval (e.g., every 10ms). It is designed to be extremely fast (O(1) complexity).
4.  **Dispatching Tasks**: Call `SCH_Dispatch_Tasks()` continuously inside your main `while(1)` loop. This function checks for any tasks that are due to run and executes them.

### Key API Functions

The scheduler's functionality is exposed through a simple set of functions defined in `scheduler.h`:

-   #### `void SCH_Init(void);`
    -   **Purpose**: Initializes the scheduler's internal data structures.
    -   **Usage**: Must be called once before any other scheduler function.

-   #### `uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);`
    -   **Purpose**: Adds a new task to the schedule.
    -   **Parameters**:
        -   `pFunction`: A pointer to the function you want to execute.
        -   `DELAY`: The initial delay (in milliseconds) before the task first runs. A delay of `0` means the task will be scheduled to run as soon as possible.
        -   `PERIOD`: The interval (in milliseconds) at which the task should repeat. **This must be a non-zero value.** For a one-shot task, simply have it delete itself or use a very large period.
    -   **Returns**: A unique `taskID` for the newly added task. You should save this ID if you plan to delete the task later.

-   #### `void SCH_Update(void);`
    -   **Purpose**: Decrements the internal timers of the scheduler. This is what makes time "pass" for the scheduled tasks.
    -   **Usage**: **Must be called from a periodic timer interrupt.** Its O(1) complexity ensures it will not block the ISR for a long time.

-   #### `void SCH_Dispatch_Tasks(void);`
    -   **Purpose**: Checks the task list and executes any tasks that are due.
    -   **Usage**: **Must be called continuously within the main `while(1)` loop.** This is the "worker" function that actually runs your code.

-   #### `uint8_t SCH_Delete_Task(uint32_t taskID);`
    -   **Purpose**: Removes a task from the scheduler before it has run.
    -   **Parameters**:
        -   `taskID`: The unique ID that was returned by `SCH_Add_Task()` when the task was created.
    -   **Returns**: `1` on success, `0` if the task could not be found.

### Example Usage

Here is a simple example of how to set up and run three different tasks, each with a unique delay and period, to demonstrate the scheduler's flexibility.

```c
// In main.c

#include "scheduler.h"
#include "tasks.h" // Assume task_Toggle_LED, task_Read_Sensor, and task_Update_Display are defined here

// --- In main() ---
int main(void) {
    // ... System Initialization (HAL_Init, Clock, GPIO, etc.) ...

    // 1. Initialize the scheduler
    SCH_Init();

    // 2. Add tasks with different timings
    // Task 1: Toggle an LED every 500ms, starting after an initial 200ms delay.
    SCH_Add_Task(task_Toggle_LED, 200, 500);

    // Task 2: Read a sensor value every 2 seconds (2000ms), starting immediately.
    SCH_Add_Task(task_Read_Sensor, 0, 2000);

    // Task 3: Update a display every second (1000ms), starting after a 1-second delay.
    SCH_Add_Task(task_Update_Display, 1000, 1000);

    // ... Start timer interrupt for SCH_Update() ...
    HAL_TIM_Base_Start_IT(&htim2);

    while (1) {
        // 4. Dispatch tasks continuously
        SCH_Dispatch_Tasks();
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // 3. Update the scheduler's internal clock (e.g., every 10ms)
        SCH_Update();
    }
}
