#include "engineer.h"
#include "pid.h"
#include "time.h"

#define PROTRACT_WAITE  100
#define GRAB_CLOSE_WAITE 150
#define GRAB_OPEN_WAITE  30
#define MOVE_WAITE 80
#define UP_AND_DOWN 100


#define GRAB_RANGE 200

#define SNATCH_RANGE    ((RM3508_Snatch_Data.Resolve_Position > (snatch_position_pid.set-GRAB_RANGE))\
&&(RM3508_Snatch_Data.Resolve_Position < (snatch_position_pid.set+GRAB_RANGE)))


struct pid_param snatch_outer = 
{
	.p = 0.45f,
	.i = 0.0040f,
	.d = 0.0f,
  .imax = 700.0f,
	.omax = 1500.0f,
};

struct pid_param snatch_inner = 
{
	.p = 12.0f,
	.i = 0.0f,
	.d = 0.0f,
  .imax = 0.0f,
	.omax = 15000.0f,
};

struct pid_param ele_outer = 
{
	.p = 0.5f,
	.i = 0.001f,
	.d = 0.0f,
  .imax = 200.0f,
	.omax = 12000.0f,
};
struct pid_param ele_inner = 
{
	.p = 3.5f,
	.i = 0.0f,
	.d = 0,
  .imax = 0.0f,
	.omax = 15000.0f,
};

struct pid snatch_position_pid;
struct pid snatch_speed_pid;

struct pid ele_left_position_pid;
struct pid ele_left_speed_pid;

struct pid ele_right_position_pid;
struct pid ele_right_speed_pid;

struct uuid uuid_t;

static void uuid_change_zero(struct uuid *uuid);
static void get_big(struct uuid *uuid);
static void covert_(struct uuid *uuid);
static void get_big_three(struct uuid *uuid);
static void tuitui_box(void);

void engineer_task(void const * argument)
{
	pid_struct_init(&snatch_position_pid,snatch_outer.omax,snatch_outer.imax,snatch_outer.p,snatch_outer.i,snatch_outer.d);
	pid_struct_init(&snatch_speed_pid,snatch_inner.omax,snatch_inner.imax,snatch_inner.p,snatch_inner.i,snatch_inner.d);
	
	pid_struct_init(&ele_left_position_pid,ele_outer.omax,ele_outer.imax,ele_outer.p,ele_outer.i,ele_outer.d);
	pid_struct_init(&ele_right_position_pid,ele_outer.omax,ele_outer.imax,ele_outer.p,ele_outer.i,ele_outer.d);
	
	pid_struct_init(&ele_left_speed_pid,ele_inner.omax,ele_inner.imax,ele_inner.p,ele_inner.i,ele_inner.d);
	pid_struct_init(&ele_right_speed_pid,ele_inner.omax,ele_inner.imax,ele_inner.p,ele_inner.i,ele_inner.d);
	
	
	while(1){

    if(uuid_t.init_uuid == 1)
    {
			engineer_init();
			set_data_init();
      uuid_t.init_uuid = 2;
    }
    else if(uuid_t.init_uuid == 2)
    {
        if(control_mode_state == REMOTE){
					
           engineering_keyboard();				
        }
    }
		
		os_delay(3);
	}
}

void engineering_keyboard()
{
    switch (motion_mode_state)
    {
    case GET_ORE_BIG:
    {
      get_big(&uuid_t);
    }
    break;
		case GET_ORE_SMALL:
    {
      get_big_three(&uuid_t);
    }
    break;
    case CONVERT_ORE:
    {
			covert_(&uuid_t);
    }
		break;
		case UUID_ZERO:
		{
			uuid_change_zero(&uuid_t);
			close_location();
			
			motion_mode_state = MOTION_MODE_PAUSE;
			break;
		}
		case 10:
		{
			tuitui_box();
			break;
		}
		case MOTION_MODE_PAUSE:
		{
			break;
		}
    default:;break;
    }
}

void get_big(struct uuid *uuid)
{
    if(uuid->get_ore_big_uuid == 0){
	
			snatch_position_pid.set = snatch_position[1];
			close_location();
			open_protract();
			
			if(SNATCH_RANGE){
					
					motion_mode_state = 0;
					uuid->get_ore_big_uuid = 1;
			}
    }
		
		else if(uuid->get_ore_big_uuid == 1){
			
			snatch_position_pid.set = snatch_position[3];
			
			if(SNATCH_RANGE){
				
				close_grab();
				os_delay(GRAB_CLOSE_WAITE);
				snatch_position_pid.set = snatch_position[2];
				uuid->get_ore_big_uuid = 2;
			}
		}
		
		else if(uuid->get_ore_big_uuid == 2){
			
			if(SNATCH_RANGE){
				
				close_protract();
				os_delay(PROTRACT_WAITE);
				uuid->get_ore_big_uuid = 3;
			}
			}

		else if(uuid->get_ore_big_uuid == 3){
			
				snatch_position_pid.set = snatch_position[0];
				
				if(SNATCH_RANGE){
					
					open_grab();
					//osDelay(100);
					uuid->get_ore_big_uuid = 0;
          motion_mode_state=0;
				}
			}	
}

void get_big_three(struct uuid *uuid){
	
    if(uuid->get_ore_small_uuid == 0){
	
			snatch_position_pid.set = snatch_position[1];
			close_location();
			open_protract();
			
			if(SNATCH_RANGE){
					
					motion_mode_state = 0;
					uuid->get_ore_small_uuid = 1;
			}
    }
		
		else if(uuid->get_ore_small_uuid == 1){
			
			snatch_position_pid.set = snatch_position[3];
			
			if(SNATCH_RANGE){
				
				close_grab();
				os_delay(GRAB_CLOSE_WAITE);
				snatch_position_pid.set = snatch_position[1];
				uuid->get_ore_small_uuid = 2;
			}
		}
	
		else if(uuid->get_ore_small_uuid == 2){
					
			if(SNATCH_RANGE){
				close_protract();
				os_delay(PROTRACT_WAITE);
				uuid->get_ore_small_uuid = 3;
				motion_mode_state = 0;
			}
		}
		
	else if(uuid->get_ore_small_uuid == 3){
		
		open_up_down();
		os_delay(UP_AND_DOWN);
		snatch_position_pid.set = snatch_position[0];
		
		if(SNATCH_RANGE){
			open_grab();
			os_delay(GRAB_OPEN_WAITE);
			uuid->get_ore_small_uuid = 4;
		}
	}
	else if(uuid->get_ore_small_uuid == 4)
	{
		os_delay(50);
		close_grab();
		uuid->get_ore_small_uuid = 0;
		motion_mode_state=0;
	}
	
		
}

void covert_(struct uuid *uuid){
	
		if(uuid->convert_uuid==0){
				open_location();
        close_grab();
				osDelay(GRAB_CLOSE_WAITE);
				uuid->convert_uuid=1;
		}
		
		else if(uuid->convert_uuid==1){		
				 
			open_up_down();
			snatch_position_pid.set = snatch_position[1];
				
			if(SNATCH_RANGE){
			open_protract();
			osDelay(PROTRACT_WAITE);
			snatch_position_pid.set = snatch_position[3];
			
			motion_mode_state=0;
			uuid->convert_uuid=2;
			}
		}
      
		else if(uuid->convert_uuid == 2){
				if(SNATCH_RANGE)
         {
						open_grab();
						osDelay(GRAB_OPEN_WAITE);
						open_move();
						osDelay(MOVE_WAITE);
						close_move();
						uuid->convert_uuid = 3;
				 }
			}
			else if(uuid->convert_uuid == 3)
			{
					snatch_position_pid.set = snatch_position[0];
          close_protract();
				  osDelay(PROTRACT_WAITE);
					close_up_down();
					os_delay(100);
					uuid->convert_uuid =0;
					motion_mode_state = 0;
      }
}

void uuid_change_zero(struct uuid *uuid)
{
	uuid->convert_uuid = 0;
	uuid->get_ore_big_uuid = 0;
	uuid->pick_uuid = 0;
	uuid->get_ore_small_uuid = 0;
	ore_number = -1 ;
	
	open_grab();
	close_protract();
	close_up_down();
	
	os_delay(GRAB_OPEN_WAITE);
	snatch_position_pid.set = snatch_position[0];
}

void tuitui_box()
{
	snatch_position_pid.set = snatch_position[3];
	close_grab();
	open_protract();
	open_up_down();
	close_location();
}