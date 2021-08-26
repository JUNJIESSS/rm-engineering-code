#ifndef __FIRST_ORDER_FITER_H
#define __FIRST_ORDER_FITER_H
#define CHASSIS_ACCEL_X_NUM 0.1666666667f
#define CHASSIS_ACCEL_Y_NUM 0.1666666666f
#define CHASSIS_ACCEL_R_NUM 0.1066666666f
#define CHASSIS_CONTROL_TIME 0.005

typedef struct first_order_filter *first_order_filter_t;

typedef  struct first_order_filter
{
    float out;          //滤波输出的数据
    float num;       //滤波参数
    float frame_period; //滤波的时间间隔 单位 s
		float last_out;    
} first_order_filter;

float first_order_filter_cali(first_order_filter_t filter, float input);
void first_order_filter_init(first_order_filter_t filter, float num, float frame_period);

extern first_order_filter chassis_cmd_vx;
extern first_order_filter chassis_cmd_vy;
extern first_order_filter chassis_cmd_rotate;

#endif

