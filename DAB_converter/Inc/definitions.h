#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

// This header provides all definitions and global variables used in the firmware

// Global definitions:
#define TRUE 	1
#define FALSE 	0
#define true 	1
#define false 	0
#define True 	1
#define False 	0

#define AND 	&&
#define and 	&&
#define OR		||
#define or		||

#define ms *10U		// defines a millisecond as 10 cycles @ the control frequency


// Comms interface configurations:
#define DEVICE_IS_MASTER 0

// CHANGE TO THE CORRESPONDENT MODULE:
#define BROADCAST_ID	0
#define DEVICE_RXCH_ID 	300
#define DEVICE_TXCH_ID 	301


// Limiting values:
#define VLV_MAX_SAFE_VALUE 				70.0f
#define VLV_MIN_SAFE_VALUE 				30.0f

#define VHV_MAX_SAFE_VALUE 				350.0f
#define VHV_MIN_SAFE_VALUE 				200.0f

#define ILV_WARNING_SAFE_VALUE  		50.0f
#define ILV_MAX_SAFE_VALUE	 			60.0f

#define T_HEATSINK_MAX_SAFE_VALUE		70.0f
#define T_HFT_MAX_SAFE_VALUE			90.0f


// Control variables output saturations:
#define PHASESHIFT_SATURATION_VALUE 	0.05f
#define D_SATURATION_LOW_VALUE      	0.0f
#define D_SATURATION_HIGH_VALUE     	0.4998f

#define P_REF_SATURATION_VALUE			1500.0f
#define I_REF_SATURATION_VALUE			30.0f

#define INTERNAL_IRATE_MAX_VALUE		0.02f
#define INTERNAL_IRATE_MIN_VALUE		0.002f

#define INTERNAL_PRATE_MAX_VALUE		20.0f
#define INTERNAL_PRATE_MIN_VALUE		1.0f


// Hardware characteristics:
#define L 								20e-6
#define fs 								20e3
#define n 								6.0f
#define pi 								PI


// Control modes:
#define PS_CONTROL 						0
#define I_CONTROL 						1
#define P_CONTROL 						2


// ==============================================================================
// GLOBAL VARIABLES:

// Measured quantities:
float 	    VLV, 				FILT_VLV,
			VHV, 				FILT_VHV,
			NEIGHBOUR_VHV, 		FILT_NEIGHBOUR_VHV,
			ILV, 				FILT_ILV,
			IHV, 				FILT_IHV,
			T_HFT, 				FILT_T_HFT,
			T_HS, 				FILT_T_HS;


// External setpoints:
float 	EXTERNAL_PSREF 				= 0.0f;
float 	EXTERNAL_D1REF 				= 0.05f;
float 	EXTERNAL_D2REF 				= 0.05f;

float 	EXTERNAL_IREF 				= 0.0f;
float 	EXTERNAL_IRATE				= 0.01f;	// 0.01 A / 100us

float 	EXTERNAL_PREF 				= 0.0f;
float 	EXTERNAL_PRATE				= 10.0f;	// 10 W / 100us

uint32_t 	EXTERNAL_CONTROLMODE 		= PS_CONTROL;


// Internal setpoints:
float	INTERNAL_PSREF				= 0.0f;
float   INTERNAL_D1REF				= 0.05f;
float   INTERNAL_D2REF 				= 0.05f;

float 	INTERNAL_IREF				= 0.0f;
float 	INTERNAL_IRATE				= 0.01f;

float 	INTERNAL_PREF				= 0.0f;
float 	INTERNAL_PRATE				= 10.0f;

uint32_t 	INTERNAL_CONTROLMODE		= PS_CONTROL;


// External commands:
// TURNON: Closes pre-charge contactors & closes main contactors if voltages are ok
// TURNOFF: Opens all contactors
// START: Starts operation in defined control mode
// STOP: Stops operation and stays idle (with closed contactors)
// RESET: Clears alarms and resets the state machine

uint8_t EXTERNAL_TURNOFF 				= FALSE;
uint8_t EXTERNAL_TURNON 				= FALSE;
uint8_t EXTERNAL_START 					= FALSE;
uint8_t EXTERNAL_STOP 					= FALSE;
uint8_t EXTERNAL_RESET 					= FALSE;
uint8_t EXTERNAL_SET_CONTROL_MODE 		= FALSE;


// Enabling flags:
uint8_t ENABLE_CONTROL 					= FALSE;

// Keepalive timer:
uint32_t KEEPALIVE_TIME_SINCE_LAST 		= 0;


// Alarms:
uint8_t	GLOBAL_ALARM 					= FALSE;
uint8_t ALARM_LV_OVERVOLTAGE 			= FALSE;
uint8_t ALARM_LV_UNDERVOLTAGE 			= FALSE;
uint8_t ALARM_HV_OVERVOLTAGE			= FALSE;
uint8_t ALARM_HV_UNDERVOLTAGE			= FALSE;
uint8_t ALARM_OVERLOAD					= FALSE;
uint8_t ALARM_OVERTEMP					= FALSE;
uint8_t ALARM_HARDWARE_FAULT			= FALSE;


// Hardware mapping:
#define KLV_MAIN 						OUT2
#define KLV_PRECHARGE 					OUT3
#define KHV_MAIN 						OUT4
#define KHV_PRECHARGE 					OUT1

// Discrete Inputs/Outputs:
uint8_t OUT1,
		OUT2,
		OUT3,
		OUT4;

uint8_t IN1,
		IN2,
		IN3,
		IN4;

uint8_t LED1,
		LED2,
		LED3,
		LED4;

uint8_t RESET1,
		RESET2;

uint8_t RESET_FEEDBACK1,
		RESET_FEEDBACK2;

uint8_t FAULT1,
		FAULT2;

uint8_t EN1,
		EN2,
		EN3,
		EN4,
		EN5,
		EN6,
		EN7,
		EN8;


uint8_t LVBUS_STATE, HVBUS_STATE, OVERLOAD_STATE, DRIVERS_STATE, TEMPERATURE_STATE, COMMS_STATE, ERROR_STATE, OPERATION_STATE;


#endif
