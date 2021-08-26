#include "pid.h"
#include "sys.h"

void Abs_Limit(struct pid *pid){
    if(pid->out < -pid->pid_param.omax){			
        pid->out = -pid->pid_param.omax;
    }		
    else if(pid->out > pid->pid_param.omax){			
        pid->out = pid->pid_param.omax;
    }
}

void Abs_Inte_Llimit(struct pid *pid)
{
	  if(pid->iout < -pid->pid_param.imax){			
        pid->iout = -pid->pid_param.imax;
    }		
    else if(pid->iout > pid->pid_param.imax){			
        pid->iout = pid->pid_param.imax;
    }
}

void PidCalculate(struct pid *pid){
	
    pid->last_error = pid->error;
    pid->error = pid->set - pid->get;
	
		pid->pout = pid->pid_param.p * pid->error;
	
    pid->iout += pid->pid_param.i * pid->error;
		Abs_Inte_Llimit(pid);
	
    pid->dout  = pid->pid_param.d *(pid->error - pid->last_error);
	
    pid->out = pid->pout + pid->iout + pid->dout;
		
    Abs_Limit(pid);
}

void pid_struct_init(    
		struct pid *pid,
    float maxout,
    float integral_limit,

    float kp,
    float ki,
    float kd)
{
	pid->pid_param.p = kp;
	pid->pid_param.i = ki;
	pid->pid_param.d = kd;
	pid->pid_param.imax = integral_limit;
	pid->pid_param.omax = maxout;
}