#ifndef CON_CAN_H
#define CON_CAN_H

#include "main.h"

typedef enum
{
	//底盘
		RM3508_ID1			=0x201,  
		RM3508_ID2			=0x202,
		RM3508_ID3			=0x203,
		RM3508_ID4			=0x204,
	
		RM3508_1_ID5			=0x205,
		RM3508_1_ID6			=0x206,
	

} CAN1_ID;

typedef enum
{
		//升降
    RM3508_2_ID1			=0x201,//左
		RM3508_2_ID2			=0x202,//右
	
	  //抓取
    RM3508_2_ID3			=0x203, 
	
		//图传
		RM2006_2_ID5      =0x205, //Pitch
	
} CAN2_ID;


typedef struct RM_Encoder//float 与 int16 位数不一样
{
    int16_t Position;
		int16_t	RollSpeed;
    int16_t Current;
		int16_t Last_Position;
		int16_t UUID;
    float Resolve_Position;
	  float Laps;
		float Error;	
} RM_Encoder_Typedef;

extern RM_Encoder_Typedef RM3508_Data_1;
extern RM_Encoder_Typedef RM3508_Data_2;
extern RM_Encoder_Typedef RM3508_Data_3;
extern RM_Encoder_Typedef RM3508_Data_4;
extern RM_Encoder_Typedef RM3508_Snatch_Data;
extern RM_Encoder_Typedef RM3508_Elevator_Data_L;
extern RM_Encoder_Typedef RM3508_Elevator_Data_R;
extern RM_Encoder_Typedef RM2006_Scan_Data;
extern RM_Encoder_Typedef RM2006_Yaw_Data;
extern RM_Encoder_Typedef RM2006_Pitch_Data;

void can_filter_init(void);
void value_get(RM_Encoder_Typedef *rm,uint8_t *data);
void sendMotorCurrent(CAN_HandleTypeDef *_hcan, int stdid, int num1, int num2,int num3, int num4);

#endif



