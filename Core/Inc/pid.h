#ifndef __PID_H_
#define __PID_H_

#include "stdint.h"

struct pid_param
{
	float p;
  float i;
  float d;
  float omax;	
	float imax;
};

struct pid
{
    struct pid_param pid_param;
	
    int32_t error;
    int32_t last_error;
		
    float set;
    float get;
    float pout;
    float iout;
    float dout;
    float out;

};

void Abs_Limit(struct pid *pid);
void Abs_Inte_Llimit(struct pid *pid);
void PidCalculate(struct pid *pid);

void pid_struct_init(    
		struct pid *pid,
    float maxout,
    float integral_limit,

    float kp,
    float ki,
    float kd);
		
#endif //__PID_H_

