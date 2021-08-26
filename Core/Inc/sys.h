#ifndef SYS_H
#define SYS_H

#include "can.h"
#include "stm32f4xx_it.h"
#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "chassis_task.h"

#include "pid.h"
#include "ramp.h"
#include "filter.h"

#define Snatch_PositionPID_default \
{\
	0.45f,\
	0.0040f,\
	0.0f,\
	1500.0f,\
	700,\
}

#define Snatch_SpeedPID_default \
{\
	12.0f,\
	0.0f,\
	0.0f,\
	15000.0f,\
}

#define Elevator_PositionPID_default \
{\
	0.3,\
	0.0f,\
	0.0f,\
	10000.0f,\
}

#define Elevator_SpeedPID_default \
{\
	2.0f,\
	0.0f,\
	0.0f,\
	15000.0f,\
}

typedef struct Encoder{
	
	float initialPosition;
	float targetPosition;
	float secondPosition;
	float thirdPosition;
	
}Encoder_Typedef;

typedef enum air_mode{
	
	CLOSE=0,
	OPEN,
	
}air_mode_typedef;

extern struct Encoder Snatch_Encoder ;
extern struct Encoder ElevatorLeft_Encoder;
extern struct Encoder ElevatorRight_Encoder;

extern uint8_t pid_calc_init;
void os_delay(uint32_t time);
#endif
