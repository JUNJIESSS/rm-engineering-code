#include "chassis_task.h"
#include "math.h"
#include "conDbus.h"

static int8_t power;

struct pid_param chassis_pid_param = 
{
	.p = 10,\
	.omax = 15000,\
};

struct pid chassis_pid;

struct pid PID_RM3508_Speed_1;
struct pid PID_RM3508_Speed_2;
struct pid PID_RM3508_Speed_3;
struct pid PID_RM3508_Speed_4;

struct chassis_speed chassis_speed_t;
struct chassis_rel_speed chassis_rel_speed_t;

static void power_limit(void);
static void chassis_remote(struct chassis_rel_speed *speed);
	
void chasssis_task(void const * argument){
	
	pid_struct_init(&PID_RM3508_Speed_1,chassis_pid_param.omax,chassis_pid_param.imax,chassis_pid_param.p,chassis_pid_param.i,chassis_pid_param.d);
	pid_struct_init(&PID_RM3508_Speed_2,chassis_pid_param.omax,chassis_pid_param.imax,chassis_pid_param.p,chassis_pid_param.i,chassis_pid_param.d);
	pid_struct_init(&PID_RM3508_Speed_3,chassis_pid_param.omax,chassis_pid_param.imax,chassis_pid_param.p,chassis_pid_param.i,chassis_pid_param.d);
	pid_struct_init(&PID_RM3508_Speed_4,chassis_pid_param.omax,chassis_pid_param.imax,chassis_pid_param.p,chassis_pid_param.i,chassis_pid_param.d);
	
	while(1){
	
		if(control_mode_state == REMOTE){
			
			chassis_remote(&chassis_rel_speed_t);
		}
		if(control_mode_state == KEYBOARD){
			
			chassis_keyboard(&chassis_rel_speed_t,&chassis_ramp_x,&chassis_speed_t);
		}
		
		PID_RM3508_Speed_1.set = (chassis_rel_speed_t.chassis_speed_x + chassis_rel_speed_t.chassis_speed_y + chassis_rel_speed_t.chassis_speed_r);
		PID_RM3508_Speed_2.set = -(chassis_rel_speed_t.chassis_speed_x - chassis_rel_speed_t.chassis_speed_y - chassis_rel_speed_t.chassis_speed_r);
		PID_RM3508_Speed_3.set = (chassis_rel_speed_t.chassis_speed_x - chassis_rel_speed_t.chassis_speed_y + chassis_rel_speed_t.chassis_speed_r);
		PID_RM3508_Speed_4.set = -(chassis_rel_speed_t.chassis_speed_x + chassis_rel_speed_t.chassis_speed_y - chassis_rel_speed_t.chassis_speed_r);

		PID_RM3508_Speed_1.get = RM3508_Data_1.RollSpeed;
		PID_RM3508_Speed_2.get = RM3508_Data_2.RollSpeed;
		PID_RM3508_Speed_3.get = RM3508_Data_3.RollSpeed;
		PID_RM3508_Speed_4.get = RM3508_Data_4.RollSpeed;
			
		PidCalculate(&PID_RM3508_Speed_1);
		PidCalculate(&PID_RM3508_Speed_2);
		PidCalculate(&PID_RM3508_Speed_3);
		PidCalculate(&PID_RM3508_Speed_4);

		power_limit();

		sendMotorCurrent(&hcan1,0x200,PID_RM3508_Speed_1.out,\
																	PID_RM3508_Speed_2.out,\
																	PID_RM3508_Speed_3.out,\
																	PID_RM3508_Speed_4.out);	
		
		os_delay(5);
	}
}

float read_num(float read_num_t)
{
	if(read_num_t>0)return 1;
	else if(read_num_t<0)return -1;
	else return 0;
}

void chassis_remote(struct chassis_rel_speed *speed)
{
  speed->chassis_speed_r = DBUS.rc.ch2 * 8.0f;
	speed->chassis_speed_x = first_order_filter_cali(&chassis_cmd_vx, (float)DBUS.rc.ch1 * 10.0f);
	speed->chassis_speed_y = first_order_filter_cali(&chassis_cmd_vy, (float)DBUS.rc.ch0 * 10.0f);
}

void chassis_keyboard(struct chassis_rel_speed *rel_speed,struct ramp_order *ramp,struct chassis_speed *speed)
{
  rel_speed->chassis_speed_x = speed->chassis_speed_x+read_num(speed->chassis_speed_x)*(speed->chassis_speed_acc+speed->chassis_speed_dowm);
	rel_speed->chassis_speed_y = speed->chassis_speed_y+read_num(speed->chassis_speed_y)*(speed->chassis_speed_acc+speed->chassis_speed_dowm);
	 if(DBUS.key.v & KEY_CTRL)
	 {
		 
	 }
	 else
	 {
		rel_speed->chassis_speed_x = ramp_calc(&chassis_ramp_x,rel_speed->chassis_speed_x);
		rel_speed->chassis_speed_y = ramp_calc(&chassis_ramp_y,rel_speed->chassis_speed_y);
	 }
	if(MOTION_MODE_ROTATE)
	{
		rel_speed->chassis_speed_r = ramp_calc(&chassis_ramp_r,7500);
	}
	else rel_speed->chassis_speed_r = first_order_filter_cali(&chassis_cmd_rotate, (float)speed->chassis_speed_mouse * 37.0f);
}

void power_limit(void)
{
	while(power==1)
	{
	if((fabs(PID_RM3508_Speed_1.out)+fabs(PID_RM3508_Speed_2.out)+fabs(PID_RM3508_Speed_3.out)+fabs(PID_RM3508_Speed_4.out))>28000)
	{
		PID_RM3508_Speed_1.out *= 0.95f;
		PID_RM3508_Speed_2.out *= 0.95f;
		PID_RM3508_Speed_3.out *= 0.95f;
		PID_RM3508_Speed_4.out *= 0.95f;
	}
	else power=0;
	}
}