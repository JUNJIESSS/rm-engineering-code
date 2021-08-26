#ifndef ENGERTASK_H
#define ENGERTASK_H
#include "main.h"

#define close_grab() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
#define open_grab() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);

#define open_move() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)
#define close_move() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET)

#define open_protract() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET)
#define close_protract() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET)

#define open_up_down() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)
#define close_up_down() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)

#define open_location() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define close_location() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)
										 

struct uuid
{
	uint8_t init_uuid;
	uint8_t get_ore_big_uuid;
	uint8_t get_ore_small_uuid;
	uint8_t convert_uuid;
	uint8_t pick_uuid;
};

#define ELE_LEFT_INDEX 0
#define ELE_RIGHT_INDEX 1

extern int32_t snatch_position[4];
extern int32_t ele_position[2][2];

extern struct pid snatch_position_pid;
extern struct pid snatch_speed_pid;

extern struct pid ele_left_position_pid;
extern struct pid ele_left_speed_pid;

extern struct pid ele_right_position_pid;
extern struct pid ele_right_speed_pid;

extern struct uuid uuid_t;

void set_data_init(void);
void engineer_init(void);
void engerTaskControl(void);
void engineering_keyboard(void);

#endif