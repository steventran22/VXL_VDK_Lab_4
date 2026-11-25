#include "scheduler.h"

//#define SCH_MAX_TASKS		10
#define ERROR_TIMER			5
#define ERROR_DUR			1000
#define error_id			99999

uint32_t id_counter;
struct List list;
struct List runList;
uint8_t Error_code = NO_ERROR;

/**
 * @brief  Initializes a linked list
 * @param  lists: Pointer to the list to be initialized
 * @retval None
 */
void List_Init(struct List* lists) {
	lists->head = NULL;
	lists->tail = NULL;
	lists->count = 0;
	lists->prefix = 0;
}

/**
 * @brief  Creates a new task node
 * @param  idx: Unique ID of the task
 * @param  pFunction: Pointer to the function to be executed by the task
 * @param  DELAY: Initial delay before the task is executed (in tick)
 * @param  PERIOD: Period of the task for repeated execution (in tick)
 * @param  origin: Flag to determine the source of the task (1: from Add_Task, 0: from Dispatcher)
 * @retval struct Node*: Pointer to the newly created task node
 */
struct Node* Create_Node(uint32_t idx, void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD, uint8_t origin){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->taskID = idx;
    newNode->pFunction = pFunction;
    newNode->DELAY = DELAY;
    newNode->PERIOD = PERIOD;
    newNode->time = origin ? DELAY : PERIOD;
    newNode->next = NULL;
    return newNode;
}

/**
 * @brief  Inserts a task node into the list, maintaining time-based order, starting from the head
 * @param  node: Pointer to the task node to be inserted
 * @retval None
 */
void Insert_From_Head(struct Node* node){
	if (list.count == 0) {
		list.head = node;
		list.tail = node;
		list.prefix = node->time;
		list.count++;
		return;
	}
	if (node->time < list.head->time){			//(remove equal sign) prioritize the task added in before
		node->next = list.head;
		list.head->time -= node->time;
		list.head = node;
	}
	else {
		struct Node* tempHead = list.head;
		do {
			node->time -= tempHead->time;
			if (node->time <= tempHead->next->time) break;
			tempHead = tempHead->next;
		} while (node->time > tempHead->time);
		node->next = tempHead->next;
		tempHead->next->time -= node->time;
		tempHead->next = node;
	}

	list.count++;
}

/**
 * @brief  Inserts a task node into the list at the tail
 * @param  node: Pointer to the task node to be inserted
 * @retval None
 */
void Insert_From_Tail(struct Node* node){
	if (list.count == 0) {
		list.head = node;
		list.tail = node;
	}
	else {
		list.tail->next = node;
		list.tail = node;
	}

	node->time -= list.prefix;
	list.prefix += node->time;
	list.count++;
}

/**
 * @brief  Removes a task node with the specified task ID from the given list
 * @param  inList: Pointer to the list from which the task will be removed
 * @param  taskID: ID of the task to be removed
 * @retval uint32_t: ID of killed task node (if kill fail or not found ret error_id = 99999)
 */
uint32_t Kill_Task(struct List* inList,uint32_t taskID){
	uint32_t victim_id;
	if (inList->head != NULL) {
		struct Node* tempNode = inList->head;
		if (tempNode->taskID == taskID) {  				//Delete head
			tempNode->next->time += tempNode->time;		//hidden bug fixed here
			inList->head = tempNode->next;
			inList->count--;
			//inList->prefix -= tempNode->time;
			victim_id = tempNode->taskID;
			free(tempNode);
			return victim_id;
		}
		while (tempNode->next != NULL) {
			if (tempNode->next->taskID == taskID) {
				struct Node* victimNode = tempNode->next;
				tempNode->next = victimNode->next;
				inList->count--;
				tempNode->next->time += victimNode->time;		//hidden bug fixed here
				//inList->prefix -= victimNode->time;
				victim_id = victimNode->taskID;
				free(victimNode);
				return victim_id;
			}
			tempNode = tempNode->next;
		}
	}
	return error_id;
}


void SCH_Init(void) {
	id_counter = 0;
	List_Init(&list);
	List_Init(&runList);
	timerSet(ERROR_TIMER, 10);
}
uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
	if (DELAY > UINT32_MAX/2) {		//check if this unsigned number get wrap-around
		Error_code = NEG_DELAY_PARAM;
		return error_id;
	}
	if (PERIOD == 0) {
		Error_code = ZERO_PERIOD_PARAM;
		return error_id;
	}
	if (PERIOD > UINT32_MAX/2) {	//check if this unsigned number get wrap-around
		Error_code = NEG_PERIOD_PARAM;
		return error_id;
	}

	id_counter++;
	uint32_t idx = id_counter;

	DELAY = DELAY / TIMER_CYCLE;
	PERIOD = PERIOD / TIMER_CYCLE;

	struct Node* node = Create_Node(idx, pFunction, DELAY, PERIOD, FROM_ADD_TASK);
	if (node->time < list.prefix){
		Insert_From_Head(node);
	} else {
		Insert_From_Tail(node);
	}
	return idx;
}

/**
 * @brief  Adds a task back into the scheduler, maintaining time-based order
 * @param  taskID: Unique ID of the task to be added
 * @param  pFunction: Pointer to the function to be executed by the task
 * @param  DELAY: Initial delay before the task is executed (in tick)
 * @param  PERIOD: Period of the task for repeated execution (in tick)
 * @retval uint32_t: ID of the task added (if applicable, currently not returned)
 */
uint32_t SCH_Add_Task_Back(uint32_t taskID, void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
	struct Node* node = Create_Node(taskID, pFunction, DELAY, PERIOD, FROM_ADD_TASK_BACK);
	if (node->time < list.prefix){
		Insert_From_Head(node);
	} else {
		Insert_From_Tail(node);
	}
	return error_id;
}
void SCH_Update(void) {
	Watchdog_Refresh();					//Feed watch_dog here
	if (list.count != 0 && list.prefix != 0) {
		list.prefix--;
		list.head->time--;
	}
}
void SCH_Dispatch_Tasks(void) {
	if (list.head == NULL) return;
	if (list.head->time == 0){
		runList.head = list.head;
		runList.tail = list.head;
		runList.count++;
		list.count--;
		struct Node* tempNode = list.head->next;
		if (tempNode != NULL) {
			while (tempNode->time == 0){
				runList.count++;
				list.count--;
				runList.tail = tempNode;
				if (tempNode->next == NULL) break;
				tempNode = tempNode->next;
			}
		}
		list.head = tempNode;  //cut into 2 lists
		runList.tail->next = NULL;
	}
	struct Node* tempNode = runList.head;
	while (tempNode != NULL){	//run
		tempNode->pFunction();
		tempNode = tempNode->next;
	}
	tempNode = runList.head;
	while (tempNode != NULL){		//add back
		SCH_Add_Task_Back(tempNode->taskID, tempNode->pFunction, tempNode->DELAY, tempNode->PERIOD);
		tempNode = tempNode->next;
	}
	tempNode = runList.head;
	while (tempNode != NULL){		//free resource
		struct Node* killedNode = tempNode;
		tempNode = tempNode->next;
		free(killedNode);
	}
	runList.count = 0;
	runList.head = NULL;
	runList.tail = NULL;
}
uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (taskID > UINT32_MAX/2) {		//check if this unsigned number get wrap-around
		Error_code = INVALID_VICTIM_ID;
		return 0;
	}
	uint32_t killed_id;
	killed_id = Kill_Task(&list, taskID);
	return (taskID == killed_id);
}

void SCH_Report_Status(void) {
#ifdef SCH_REPORT_ERRORS
    // ONLY APPLIES IF WE ARE REPORTING ERRORS
	if (timerFlag(ERROR_TIMER)) {
		timerSet(ERROR_TIMER, ERROR_DUR);
		display7SEGError(Error_code);
	}
#endif
}
