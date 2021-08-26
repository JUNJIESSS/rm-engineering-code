#include "conDbus.h"
#include "engineer.h"

struct DBUS_Typedef DBUS;

enum control_mode control_mode_t ;
enum motion_mode_type motion_mode_type_t;
int8_t ore_number = -1;
struct air_type_mode air_type_mode_t = {0};
Key_Count_Control Key_Count = {0};
uint8_t control_mode_state = 0;//Ò£¿ØÎª1 ¼üÅÌÎª2
uint8_t motion_mode_state = 0;
uint8_t MOTION_MODE_ROTATE = 0;

int small_and_big = 0;
uint8_t my_upanddown = 0;

void dbus_getdata(){
	
	DBUS.rc.ch0	=	(DBUS_BUFFER[0]|(int16_t)DBUS_BUFFER[1]<<8)&0x07FF;
  DBUS.rc.ch1	= (DBUS_BUFFER[1]>>3|(int16_t)DBUS_BUFFER[2]<<5)&0x07FF;
  DBUS.rc.ch2 = (DBUS_BUFFER[2] >> 6 | (int16_t)DBUS_BUFFER[3] << 2| DBUS_BUFFER[4]<<10) & 0x07FF;
  DBUS.rc.ch3 = (DBUS_BUFFER[4] >> 1 | (int16_t)DBUS_BUFFER[5] << 7) & 0x07FF;
  DBUS.rc.dial=(DBUS_BUFFER[16]|(int16_t)DBUS_BUFFER[17]<<8)&0x07FF;
	
  if(DBUS.rc.ch0&&DBUS.rc.ch1&&DBUS.rc.ch2&&DBUS.rc.ch3&&DBUS.rc.dial){
        
		DBUS.rc.ch0 -= 1024;
    DBUS.rc.ch1 -= 1024;
    DBUS.rc.ch2 -= 1024;
    DBUS.rc.ch3 -= 1024;
    DBUS.rc.dial -= 1024;
   }
	
    DBUS.rc.s1   = (DBUS_BUFFER[5] >> 4) & 0x03;
    DBUS.rc.s2   = (DBUS_BUFFER[5] >> 6) & 0x03;
    DBUS.mouse.mouse_x = DBUS_BUFFER[6] | (int16_t)DBUS_BUFFER[7] << 8;
    DBUS.mouse.mouse_y = DBUS_BUFFER[8] | (int16_t)DBUS_BUFFER[9] << 8;
    DBUS.mouse.mouse_z = DBUS_BUFFER[10] | (int16_t)DBUS_BUFFER[11] << 8;
    DBUS.mouse.press_l = DBUS_BUFFER[12];
    DBUS.mouse.press_r = DBUS_BUFFER[13];
    DBUS.key.v = DBUS_BUFFER[14] | (int16_t)DBUS_BUFFER[15] << 8;
/*************************************/	
		if(DBUS.rc.s2 == REMOTE)
		{
			control_mode_state = REMOTE;
		}
		else if(DBUS.rc.s2 == 3)
		{
			control_mode_state = REMOTE;
		}
		else control_mode_state = 0;
		
/*************************************/		
		if(DBUS.rc.s1 == 1 && small_and_big==0)
		{
			motion_mode_state = GET_ORE_BIG;
			small_and_big = 1;
		}	
/*************************************/	
		if(DBUS.key.v & KEY_G){
			
			motion_mode_state = GET_ORE_SMALL;                                                                     ;
		}
/*************************************/
		if(DBUS.rc.s1 == 2 && small_and_big == 0){
			
			motion_mode_state = CONVERT_ORE;
			small_and_big = 1;
		}
/*************************************/		
		if(DBUS.key.v & KEY_E)
		{
			motion_mode_state = MOTION_MODE_PAUSE;
		}
/*************************************/
		if(DBUS.key.v & KEY_R)
		{
			motion_mode_state = UUID_ZERO;
		}
		if(DBUS.rc.s1 == 3)
		{
			small_and_big = 0;
		}
/*************************************/	
		if(DBUS.key.v & KEY_Q)
	 {
			Key_Count.KeyQ_Count++;
			if(Key_Count.KeyQ_Count == 2)
			{
					if(small_and_big == 0)
					{
						open_grab();
						small_and_big = 1;
					}
					else {close_grab();small_and_big = 0;}
			}
		}
		else Key_Count.KeyQ_Count = 0;
/*************************************/		
		if(DBUS.rc.s2 == 3){
			
						ele_left_position_pid.set = ele_position[ELE_LEFT_INDEX][1];
						ele_right_position_pid.set = ele_position[ELE_RIGHT_INDEX][1];
						my_upanddown = 1;
					}
		else if(DBUS.rc.s2 == 1)
			{
			
						ele_left_position_pid.set = ele_position[ELE_LEFT_INDEX][0];
			    	ele_right_position_pid.set = ele_position[ELE_RIGHT_INDEX][0];
				  	my_upanddown = 0;
			}

/*************************************/	
		if(DBUS.key.v & KEY_X)
	 {
			Key_Count.KeyX_Count++;
			if(Key_Count.KeyX_Count == 2)
			{
				motion_mode_state = 10;
			}
		}
		else Key_Count.KeyX_Count = 0;
/*************************************/	
		if(control_mode_state == KEYBOARD){
			
			if(DBUS.key.v & KEY_W){
				
				chassis_speed_t.chassis_speed_x = CHASSIS_SEPEED_NOR;
			}
			else if(DBUS.key.v & KEY_S){
				
				chassis_speed_t.chassis_speed_x = -CHASSIS_SEPEED_NOR;
			}
			else chassis_speed_t.chassis_speed_x = 0;
			
			if(DBUS.key.v == (KEY_A+KEY_D)){
				
				MOTION_MODE_ROTATE = 1;
				chassis_speed_t.chassis_speed_y = 0;
			}
			else if(DBUS.key.v & KEY_A){
				
				chassis_speed_t.chassis_speed_y = -(CHASSIS_SEPEED_NOR);
				MOTION_MODE_ROTATE = 0;
			}
			else if(DBUS.key.v & KEY_D){
				
				chassis_speed_t.chassis_speed_y = (CHASSIS_SEPEED_NOR);
				MOTION_MODE_ROTATE = 0;
			}
			else {
				
				chassis_speed_t.chassis_speed_y = 0,MOTION_MODE_ROTATE = 0;
			}
			
			if(DBUS.key.v & KEY_SHIFT){
				
				chassis_speed_t.chassis_speed_acc = CHASSIS_SEPEED_ACC;
			}
			else chassis_speed_t.chassis_speed_acc = 0;

			if(DBUS.key.v & KEY_CTRL){
				
				chassis_speed_t.chassis_speed_dowm = -CHASSIS_SEPEED_DOW;
			}
			
			else chassis_speed_t.chassis_speed_dowm = 0;
			
			chassis_speed_t.chassis_speed_mouse = DBUS.mouse.mouse_x * 0.85;
		}
}