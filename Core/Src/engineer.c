#include "pid.h"
#include "engineer.h"
#include "conCan.h"

int32_t snatch_position[4] = {0,38192,48192,77824};
int32_t ele_position[2][2] = {0,-863240,0,863240};

void engineer_init()
{
	for(int i = 0;i < 4;i++)
	{
		snatch_position[i] += RM3508_Snatch_Data.Position;
	}
	
	for(int m = 0; m < 2; m++ )
	{
		ele_position[ELE_LEFT_INDEX][m] += RM3508_Elevator_Data_L.Position;
		ele_position[ELE_RIGHT_INDEX][m] += RM3508_Elevator_Data_R.Position;
	}
	
}

void set_data_init()
{
	snatch_position_pid.set = RM3508_Snatch_Data.Position;
	ele_left_position_pid.set = RM3508_Elevator_Data_L.Position;
  ele_right_position_pid.set = RM3508_Elevator_Data_R.Position;
}