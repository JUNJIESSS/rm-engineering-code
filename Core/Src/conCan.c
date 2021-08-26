#include "conCan.h"

CAN_FilterTypeDef can_filter_st;

RM_Encoder_Typedef RM3508_Data_1;
RM_Encoder_Typedef RM3508_Data_2;
RM_Encoder_Typedef RM3508_Data_3;
RM_Encoder_Typedef RM3508_Data_4;

RM_Encoder_Typedef RM3508_Snatch_Data = {.Position = 4096,};
RM_Encoder_Typedef RM3508_Elevator_Data_L = {.Position = 4096,};
RM_Encoder_Typedef RM3508_Elevator_Data_R = {.Position = 4096,};
RM_Encoder_Typedef RM2006_Scan_Data = {.Position = 4096,};
RM_Encoder_Typedef RM2006_Yaw_Data = {.Position = 4096,};
RM_Encoder_Typedef RM2006_Pitch_Data = {.Position = 4096,};

uint32_t a;
void can_filter_init()
{
    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;	
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;	
    can_filter_st.FilterIdHigh = 0x0000;	
    can_filter_st.FilterIdLow = 0x0000;	
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;	
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;		
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);		
    HAL_CAN_Start(&hcan1);		
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    can_filter_st.SlaveStartFilterBank = 14;		
    can_filter_st.FilterBank = 14;	
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);		
    HAL_CAN_Start(&hcan2);	
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
    return;
}

void sendMotorCurrent(CAN_HandleTypeDef *_hcan, int stdid, int num1, int num2,int num3, int num4)
{
    CAN_TxHeaderTypeDef sendmessage;
    uint8_t Data[8];
    uint32_t send_mail_box;
    sendmessage.DLC = 0x08;
    sendmessage.IDE = CAN_ID_STD;
    sendmessage.RTR = CAN_RTR_DATA;
    sendmessage.StdId = stdid;
    Data[0] = ((num1) >> 8);
    Data[1] = (num1);
    Data[2] = ((num2) >> 8);
    Data[3] = (num2);
    Data[4] = ((num3) >> 8);
    Data[5] = (num3);
    Data[6] = ((num4) >> 8);
    Data[7] = (num4);
    if ((_hcan) == &hcan1)
    {
        HAL_CAN_AddTxMessage(&hcan1, &sendmessage, Data, &send_mail_box);
    }
    if ((_hcan) == &hcan2)
    {
        HAL_CAN_AddTxMessage(&hcan2, &sendmessage, Data, &send_mail_box);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
    if (hcan == &hcan1){
        switch (rx_header.StdId){
        case RM3508_ID1:
					value_get(&RM3508_Data_1,rx_data);    
					break;
				case RM3508_ID2:						
					value_get(&RM3508_Data_2,rx_data);            									
					break;				
				case RM3508_ID3:					
					value_get(&RM3508_Data_3,rx_data);           									
					break;
				case RM3508_ID4:					
					value_get(&RM3508_Data_4,rx_data);
					break;			
        case RM3508_1_ID5:
					value_get(&RM3508_Elevator_Data_L,rx_data);
					break;
				case RM3508_1_ID6:
					value_get(&RM3508_Elevator_Data_R,rx_data);
					break;
       }
    }
    if (hcan == &hcan2){
        switch (rx_header.StdId){
						case RM3508_2_ID1:		
						value_get(&RM3508_Snatch_Data,rx_data);						
						break;
        }
		static int can_uuid=0;
		if(can_uuid==0)
		{
			uuid_t.init_uuid = 1 ;
			RM3508_Elevator_Data_R.UUID = 1;
			can_uuid = 1;
		}	
	}
}

void value_get(RM_Encoder_Typedef *rm,uint8_t *data){

	rm->Last_Position = rm->Position;
	rm->Position   = (data[0] << 8) + data[1];
	rm->RollSpeed  = (data[2] << 8) + data[3];
	rm->Current    = (data[4] << 8) + data[5];
	rm->Error = rm->Last_Position - rm->Position;
	if(rm->Error > 4096){
			rm->Laps++;
	}
	else if(rm->Error < -4096){
			rm->Laps--;
	}
	rm->Resolve_Position = rm->Laps * 8192 +rm->Position;	
}
