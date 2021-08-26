#ifndef CHASSTAK_H
#define CHASSTAK_H

#include "ramp.h"
#include "pid.h"

#define CHASSIS_SEPEED_NOR 4000
#define CHASSIS_SEPEED_ACC 4800

#define CHASSIS_SEPEED_DOW 2700

struct chassis_speed
{
	float chassis_speed_x;
	float chassis_speed_y;
	float chassis_speed_mouse;
	float chassis_speed_acc;
	float chassis_speed_dowm;
};

struct chassis_rel_speed
{
	float chassis_speed_x;
	float chassis_speed_y;
	float chassis_speed_r;
};

extern struct chassis_rel_speed chassis_rel_speed_t;
extern struct chassis_speed chassis_speed_t;


void chassis_task(void);
void chassis_keyboard(struct chassis_rel_speed *rel_speed,struct ramp_order *ramp,struct chassis_speed *speed);
float read_num(float read_num_t);
#endif
