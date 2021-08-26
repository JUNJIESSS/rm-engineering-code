#ifndef CAN_DBUS_H
#define CAN_DBUS_H

#include "main.h"

/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_W	((uint16_t)0x01<<0)
#define KEY_S	((uint16_t)0x01<<1)
#define KEY_A	((uint16_t)0x01<<2)
#define KEY_D	((uint16_t)0x01<<3)
#define KEY_SHIFT	((uint16_t)0x01<<4)
#define KEY_CTRL	((uint16_t)0x01<<5)
#define KEY_Q	((uint16_t)0x01<<6)
#define KEY_E	((uint16_t)0x01<<7)

#define KEY_R	((uint16_t)0x01<<8)
#define KEY_F	((uint16_t)0x01<<9)
#define KEY_G	((uint16_t)0x01<<10)
#define KEY_Z	((uint16_t)0x01<<11)
#define KEY_X	((uint16_t)0x01<<12)
#define KEY_C	((uint16_t)0x01<<13)
#define KEY_V	((uint16_t)0x01<<14)
#define KEY_B	((uint16_t)0x01<<15)

#define Mouse_X 0.1f
#define Mouse_Y 0.1f

#define Mouse_Clik  70.0f				//14ms计算一次遥控值，当按下时间小于4*50=200ms时，视为单击

#define AIR_ON 1
#define AIR_OFF 0

struct DBUS_Typedef
{
    struct
    {
        int16_t ch0;
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;
        uint8_t s1;
        uint8_t s2;
        int16_t dial;
    } rc;
		
    struct
    {
        int16_t mouse_x;
        int16_t mouse_y;
        int16_t mouse_z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;

    struct
    {
        uint16_t v;
    } key;

};

extern struct DBUS_Typedef DBUS;

typedef struct Key_Count_Control
{
    uint16_t KeyW_Count;
    uint16_t KeyS_Count;
    uint16_t KeyA_Count;
    uint16_t KeyD_Count;
    uint16_t KeySHIFT_Count;
    uint16_t KeyCTRL_Count;
    uint16_t MouseL_Count;
    uint16_t MouseR_Count;
    uint16_t KeyQ_Count;
    uint16_t KeyE_Count;
    uint16_t KeyR_Count;
    uint16_t KeyF_Count;
    uint16_t KeyG_Count;
    uint16_t KeyZ_Count;
    uint16_t KeyX_Count;
    uint16_t KeyC_Count;
    uint16_t KeyV_Count;
    uint16_t KeyB_Count;
		uint8_t  VisionQ_Count;
	  uint16_t DBUS_Count;//遥控丢失计数
} Key_Count_Control; //按键计数

struct air_type_mode
{
	uint8_t grab_t;
	uint8_t rescue_t;
	uint8_t fill_t;
	uint8_t protract_t;
	uint8_t freight_t;
	uint8_t slot_card;
};

extern struct air_type_mode air_type_mode_t;

enum control_mode{
	
	REMOTE=1,
	KEYBOARD=2,
};


enum motion_mode_type
{
	MOTION_MODE_PAUSE = 0,
	GET_ORE_BIG,
	GET_ORE_SMALL,
	CONVERT_ORE,
	PICK_ORE,
	UUID_ZERO,
	ORE_PUT,
	SLOT_CARD,
	ROTATE,
	ORE_SECONDE_PUT,
	ABSOLUTE_RESTART,
};

extern enum motion_mode_type motion_mode_type_t;
extern uint8_t MOTION_MODE_ROTATE;
extern int8_t ore_number;
extern uint8_t motion_mode_state;
extern uint8_t control_mode_state;
extern Key_Count_Control Key_Count;//按键消抖

void dbus_getdata(void);

#endif
