#ifndef RAMP_H
#define RAMP_H

#define scale_factor 0.008

struct ramp_order
{
    float out;
    float factor;
		float now_out;
		float last_out;
};

float ramp_calc(struct ramp_order *ramp, float input);
void ramp_init(struct ramp_order *ramp, float num);
extern struct ramp_order chassis_ramp_r;
extern struct ramp_order chassis_ramp_x;
extern struct ramp_order chassis_ramp_y;

#endif