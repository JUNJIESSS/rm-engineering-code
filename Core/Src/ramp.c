#include "ramp.h"
#include "math.h"
#include "stdio.h"
#include "chassis_task.h"

struct ramp_order chassis_ramp_x;
struct ramp_order chassis_ramp_y;
struct ramp_order chassis_ramp_r;
float ramp_calc(struct ramp_order *ramp, float input)
{
	if(read_num(input)==1)
	{
		ramp->now_out += ramp->factor * input;
		if(ramp->now_out > input)
		{
			ramp->now_out = input;
			return input;
		}
	}
	else if(read_num(input)==-1)
	{
		ramp->now_out += ramp->factor * input;
		if(ramp->now_out < input)
		{
			ramp->now_out = input;
			return input;
		}
	}
	else
	{
		if(ramp->now_out!=0)
		{
			 ramp->now_out = read_num(ramp->now_out)*(fabs(ramp->now_out) - 0.05 * fabs(ramp->now_out));
		}
		else
		{
				ramp->now_out = 0;
		}
	}
	
	ramp->last_out = input;
	return ramp->now_out;
}

void ramp_init(struct ramp_order *ramp, float factor)
{
	ramp->factor = factor;
}