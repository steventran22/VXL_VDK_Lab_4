#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"

struct Node{
	uint32_t taskID;
	void (* pFunction)();
	uint32_t DELAY;
	uint32_t PERIOD;
	uint32_t time;
	struct Node* next;
};
struct List{
	struct Node* head;
	struct Node* tail;
	uint32_t count;
	uint32_t prefix;
};

/**
 * @brief  Initializes the scheduler by resetting the task ID counter and initializing the task lists
 * @retval None
 */
void SCH_Init(void);

/**
 * @brief  Adds a new task to the scheduler, maintaining time-based order
 * @param  pFunction: Pointer to the function to be executed by the task
 * @param  DELAY: Initial delay before the task is executed (in tick)
 * @param  PERIOD: Period of the task for repeated execution (in tick)
 * @retval uint32_t: Unique ID of the task added
 * @note   Users suppose to use this function to add their custom tasks to the scheduler.
 */
uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD);

/**
 * @brief  Updates the scheduler by decrementing the prefix and the time of the head task
 * @retval None
 * @note   This function has a time complexity of O(1) because it performs a constant number of operations regardless of the size of the task list.
 */
void SCH_Update(void);

/**
 * @brief  Dispatches tasks that are ready to run, executes them, and re-adds periodic tasks to the scheduler
 * @retval None
 * @details 
 * - Moves tasks with `time == 0` from the main list to the run list.
 * - Executes all tasks in the run list.
 * - Re-adds periodic tasks back to the scheduler with their original delay and period.
 * - Frees memory for one-time tasks after execution.
 */
void SCH_Dispatch_Tasks(void);

/**
 * @brief  Deletes a task with the specified task ID from the scheduler
 * @param  taskID: Unique ID of the task to be deleted
 * @retval uint8_t: Status of the operation (1: success, 0: failure if task not found or delete unsuccessfully)
 */
uint8_t SCH_Delete_Task(uint32_t taskID);

/**
 * @brief  Reports scheduler status by periodically displaying the current error code
 * @retval None
 * @note   Active only when SCH_REPORT_ERRORS is defined. Uses ERROR_TIMER and ERROR_DUR to rate-limit updates and calls display7SEGError(Error_code).
 */
void SCH_Report_Status(void);


#endif /* INC_SCHEDULER_H_ */
