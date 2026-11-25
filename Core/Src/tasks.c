#include "tasks.h"

int num = 0;
int idx = 0;
void task_7seg(){
	switch(idx){
		case 0:
			display7SEG(num%10);
			enableSEGs(idx);
			idx++;
			break;
		case 1:
			display7SEG(num/10);
			enableSEGs(idx);
			idx = 0;
			break;
		default:
			break;
	}
}

void task_incre_num(){
	num++;
}

void task1(){
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

void task2(){
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

void task3(){
	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
}

void task4(){
	HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
}

void task5(){
	HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
}
