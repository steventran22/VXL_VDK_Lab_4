#include "display7seg.h"

uint16_t segPins1[7] = {A_Pin, B_Pin, C_Pin, D_Pin, E_Pin, F_Pin, G_Pin};
uint16_t segPins2[7] = {ERROR_A_Pin, ERROR_B_Pin, ERROR_C_Pin, ERROR_D_Pin, ERROR_E_Pin, ERROR_F_Pin, ERROR_G_Pin};
uint16_t enablePins[2] = {EN1_Pin, EN2_Pin};
// Bảng mã cho từng số (0-9), 0 = sáng (common anode)
const uint8_t digitCode[11][7] = {
	{0, 0, 0, 0, 0, 0, 1}, 		// 0
	{1, 0, 0, 1, 1, 1, 1}, 		// 1
	{0, 0, 1, 0, 0, 1, 0}, 		// 2
	{0, 0, 0, 0, 1, 1, 0}, 		// 3
	{1, 0, 0, 1, 1, 0, 0}, 		// 4
	{0, 1, 0, 0, 1, 0, 0}, 		// 5
	{0, 1, 0, 0, 0, 0, 0}, 		// 6
	{0, 0, 0, 1, 1, 1, 1}, 		// 7
	{0, 0, 0, 0, 0, 0, 0}, 		// 8
	{0, 0, 0, 0, 1, 0, 0},  	// 9
	{1, 1, 1, 1, 1, 1, 1},		//Disable (all segs are off)
};

void display7SEG(int num) {
	if (num > 10 || num < 0) return;
	for (int i = 0; i < 7; i++) {
	    HAL_GPIO_WritePin(GPIOB, segPins1[i], digitCode[num][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}
void display7SEGError(uint8_t num) {
	if (num > 10 || num < 0) return;
	for (int i = 0; i < 7; i++) {
	    HAL_GPIO_WritePin(GPIOB, segPins2[i], digitCode[num][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}
void enableSEGs(int idx) {
	for (int i = 0; i < 2; i++){
	  	if(i == idx){
	  		HAL_GPIO_WritePin(GPIOB, enablePins[i], GPIO_PIN_RESET);
	  	}
	  	else {
	  		HAL_GPIO_WritePin(GPIOB, enablePins[i], GPIO_PIN_SET);
  		}
  	}
}
