#ifndef FSM_H_
#define FSM_H_

#define NB_SMES 8

typedef struct {
  uint8_t  state ,       		// Current State
           prev_state;   		// Previous State
  uint32_t TIS;         		// Time In State, base: Ts (discrete control sample time)
} fsm_t;

fsm_t smes[NB_SMES];

#define LVBUS 				smes[0]
#define HVBUS				smes[1]
#define OVERLOAD			smes[2]
#define DRIVERS				smes[3]
#define TEMPERATURE			smes[4]
#define COMMS				smes[5]
#define ERROR				smes[6]
#define OPERATION			smes[7]



// LVBUS FSM - CONDITIONS:
#define C_LVBUS_01 (VLV > 1.05f * VLV_MIN_SAFE_VALUE)
#define C_LVBUS_12 (VLV > VLV_MAX_SAFE_VALUE)
#define C_LVBUS_10 (VLV < VLV_MIN_SAFE_VALUE)
#define C_LVBUS_23 (VLV < 0.95f * VLV_MAX_SAFE_VALUE)
#define C_LVBUS_32 (VLV > VLV_MAX_SAFE_VALUE)
#define C_LVBUS_30 (EXTERNAL_RESET)


// HVBUS FSM - CONDITIONS:
#define C_HVBUS_01 (VHV > 1.05f * VHV_MIN_SAFE_VALUE)
#define C_HVBUS_12 (VHV > VHV_MAX_SAFE_VALUE)
#define C_HVBUS_10 (VHV < VHV_MIN_SAFE_VALUE)
#define C_HVBUS_23 (VHV < 0.95f * VHV_MAX_SAFE_VALUE)
#define C_HVBUS_32 (VHV > VHV_MAX_SAFE_VALUE)
#define C_HVBUS_30 (EXTERNAL_RESET)


// OVERLOAD FSM - CONDITIONS:
#define C_OVERLOAD_01 (ILV > ILV_WARNING_SAFE_VALUE)
#define C_OVERLOAD_10 (ILV < 0.9f * ILV_WARNING_SAFE_VALUE)
#define C_OVERLOAD_12 (ILV > ILV_MAX_SAFE_VALUE)
#define C_OVERLOAD_20 (EXTERNAL_RESET)


// DRIVERS FSM - CONDITIONS:
#define C_DRIVERS_01 (!FAULT2 OR (OPERATION.state == 2 AND RESET_FEEDBACK1 == FALSE) OR (OPERATION.state == 2 AND RESET_FEEDBACK2 == FALSE))
#define C_DRIVERS_12 (EXTERNAL_RESET)
#define C_DRIVERS_20 (DRIVERS.TIS > 1000 ms)


// TEMPERATURE FSM - CONDITIONS:
#define C_TEMPERATURE_01 ( (T_HFT > T_HFT_MAX_SAFE_VALUE) OR (T_HS > T_HEATSINK_MAX_SAFE_VALUE) )
#define C_TEMPERATURE_10 (EXTERNAL_RESET AND (T_HFT < T_HFT_MAX_SAFE_VALUE) AND (T_HS < T_HEATSINK_MAX_SAFE_VALUE) )

// if the system is not off and the time since last keepalive packet is too high, set device as offline;
// set it online again once keepalive packets start arriving
// COMMS FSM - CONDITIONS:
#define C_COMMS_01 ( KEEPALIVE_TIME_SINCE_LAST > 200 ms )
#define C_COMMS_10 ( KEEPALIVE_TIME_SINCE_LAST < 100 ms )


// ERROR FSM - CONDITIONS:
#define C_ERROR_01 (LVBUS.state == 2 OR  LVBUS.state == 3  OR  HVBUS.state == 2  OR  HVBUS.state == 3  OR  OVERLOAD.state == 2  OR  DRIVERS.state == 1  OR  COMMS.state == 1  OR  (LVBUS.state == 0 AND OPERATION.state == 3) OR (HVBUS.state == 0 AND OPERATION.state ==3) OR TEMPERATURE.state == 1 )
#define C_ERROR_10 (EXTERNAL_RESET)
#define C_ERROR_12 (ERROR.TIS > 1000 ms)
#define C_ERROR_20 (EXTERNAL_RESET)
#define C_ERROR_21 (ERROR.TIS > 1000 ms)



// OPERATION FSM - CONDITIONS:
#define C_OPERATION_01 (ERROR.state == 0  	AND		(OPERATION.TIS > 1000 ms)   AND 	EXTERNAL_TURNON)
#define C_OPERATION_04 (ERROR.state != 0)

#define C_OPERATION_10 (ERROR.state == 0 	AND		EXTERNAL_TURNOFF)
#define C_OPERATION_12 (ERROR.state == 0 	AND		OPERATION.TIS > 5000 ms 	AND 	(FILT_VLV >= 1.1*VLV_MIN_SAFE_VALUE)		AND 	(FILT_VHV >= 1.1*VHV_MIN_SAFE_VALUE) )
#define C_OPERATION_14 (ERROR.state != 0	OR		(ERROR.state == 0 			AND		OPERATION.TIS > 5000 ms 		AND 	(FILT_VLV < 1.1*VLV_MIN_SAFE_VALUE	OR 	FILT_VHV < 1.1*VHV_MIN_SAFE_VALUE) ) )

#define C_OPERATION_20 (ERROR.state == 0	AND 	EXTERNAL_TURNOFF)
#define C_OPERATION_23 (ERROR.state == 0 	AND 	EXTERNAL_START)
#define C_OPERATION_24 (ERROR.state != 0)

#define C_OPERATION_32 (ERROR.state != 0	OR 		EXTERNAL_STOP 	OR 		EXTERNAL_TURNOFF)

#define C_OPERATION_40 (EXTERNAL_RESET)

void fsm(void){

	// LVBUS FSM:
	// --------------------------------------------------------------------------------------
			if (LVBUS.state 	== 0 	AND C_LVBUS_01)			{ LVBUS.state 		= 1;	}
	else	if (LVBUS.state 	== 1 	AND C_LVBUS_12)			{ LVBUS.state 		= 2;	}
	else	if (LVBUS.state     == 1	AND C_LVBUS_10)			{ LVBUS.state 		= 0;	}
	else 	if (LVBUS.state 	== 2 	AND C_LVBUS_23)			{ LVBUS.state 		= 3;	}
	else 	if (LVBUS.state		== 3  	AND C_LVBUS_32)			{ LVBUS.state		= 2;	}
	else 	if (LVBUS.state		== 3	AND C_LVBUS_30)			{ LVBUS.state		= 0;	}
	// --------------------------------------------------------------------------------------


	// HVBUS FSM:
	// --------------------------------------------------------------------------------------
			if (HVBUS.state 	== 0 	AND C_HVBUS_01)			{ HVBUS.state 		= 1;	}
	else	if (HVBUS.state 	== 1 	AND C_HVBUS_12)			{ HVBUS.state 		= 2;	}
	else	if (HVBUS.state     == 1	AND C_HVBUS_10)			{ HVBUS.state 		= 0;	}
	else 	if (HVBUS.state 	== 2 	AND C_HVBUS_23)			{ HVBUS.state 		= 3;	}
	else 	if (HVBUS.state		== 3	AND C_HVBUS_32)			{ HVBUS.state		= 2;	}
	else	if (HVBUS.state		== 3	AND C_HVBUS_30)			{ HVBUS.state		= 0;	}
	// --------------------------------------------------------------------------------------


	// OVERLOAD FSM:
	// --------------------------------------------------------------------------------------
			if (OVERLOAD.state 	== 0	AND C_OVERLOAD_01) 		{ OVERLOAD.state	= 1;	}
	else	if (OVERLOAD.state  == 1 	AND C_OVERLOAD_10)		{ OVERLOAD.state	= 0;	}
	else 	if (OVERLOAD.state  == 1 	AND C_OVERLOAD_12) 		{ OVERLOAD.state	= 2;	}
	else	if (OVERLOAD.state  == 2    AND C_OVERLOAD_20)		{ OVERLOAD.state	= 0;	}
	// --------------------------------------------------------------------------------------


	// DRIVERS FSM:
	// --------------------------------------------------------------------------------------
			if (DRIVERS.state  	== 0	AND C_DRIVERS_01)		{ DRIVERS.state 	= 1; 	}
	else	if (DRIVERS.state 	== 1	AND C_DRIVERS_12)		{ DRIVERS.state		= 2; 	}
	else	if (DRIVERS.state   == 2 	AND C_DRIVERS_20)		{ DRIVERS.state		= 0;	}

	// --------------------------------------------------------------------------------------


	// TEMPERATURE FSM:
	// --------------------------------------------------------------------------------------
			if (TEMPERATURE.state == 0 	AND C_TEMPERATURE_01)	{ TEMPERATURE.state = 1;	}
	else 	if (TEMPERATURE.state == 1  AND C_TEMPERATURE_10)	{ TEMPERATURE.state = 0;	}

	// --------------------------------------------------------------------------------------


	// COMMS FSM:
	// --------------------------------------------------------------------------------------
//			if (COMMS.state == 0 		AND C_COMMS_01)			{ COMMS.state = 1;			}
//	else	if (COMMS.state == 1		AND C_COMMS_10)			{ COMMS.state = 0;			}

	// --------------------------------------------------------------------------------------


	// ERROR FSM:
	// --------------------------------------------------------------------------------------
			if (ERROR.state == 0 		AND C_ERROR_01)			{ ERROR.state = 1;			}
	else 	if (ERROR.state == 1 		AND C_ERROR_10)			{ ERROR.state = 0;			}
	else 	if (ERROR.state == 1 		AND C_ERROR_12)			{ ERROR.state = 2;			}
	else	if (ERROR.state == 2		AND C_ERROR_21)			{ ERROR.state = 1;			}
	else 	if (ERROR.state == 2		AND C_ERROR_20)			{ ERROR.state = 0;			}


	// --------------------------------------------------------------------------------------

	// OPERATION FSM:
	// --------------------------------------------------------------------------------------
			if (OPERATION.state == 0    AND C_OPERATION_01)		{ OPERATION.state 	= 1;	}
	else	if (OPERATION.state == 1	AND C_OPERATION_10)		{ OPERATION.state 	= 0;	}
	else	if (OPERATION.state == 1	AND C_OPERATION_12)		{ OPERATION.state 	= 2;	}
	else	if (OPERATION.state == 1	AND C_OPERATION_14)		{ OPERATION.state 	= 4;	}
	else 	if (OPERATION.state == 2 	AND C_OPERATION_20)		{ OPERATION.state   = 0;	}
	else 	if (OPERATION.state == 2	AND C_OPERATION_23)		{ OPERATION.state	= 3;	}
	else 	if (OPERATION.state == 2 	AND C_OPERATION_24)		{ OPERATION.state	= 4;	}
	else 	if (OPERATION.state == 3 	AND C_OPERATION_32)		{ OPERATION.state 	= 2;	}
	else 	if (OPERATION.state == 4 	AND C_OPERATION_40)		{ OPERATION.state   = 0;	}


	// OUTPUTS:
	KLV_PRECHARGE = (OPERATION.state == 1);
	KHV_PRECHARGE = (OPERATION.state == 1);
	KLV_MAIN = (OPERATION.state == 2) OR (OPERATION.state == 3);
	KHV_MAIN = (OPERATION.state == 2) OR (OPERATION.state == 3);


	// ALARMS:
	GLOBAL_ALARM = (ERROR.state == 1) OR (ERROR.state == 2);
	ALARM_LV_UNDERVOLTAGE = LVBUS.state == 0;
	ALARM_LV_OVERVOLTAGE = (LVBUS.state == 2) OR (LVBUS.state == 3);
	ALARM_HV_UNDERVOLTAGE = HVBUS.state == 0;
	ALARM_HV_OVERVOLTAGE = (HVBUS.state == 2) OR (HVBUS.state == 3);
	ALARM_OVERLOAD = (OVERLOAD.state == 2);
	ALARM_OVERTEMP = (TEMPERATURE.state == 1);
	ALARM_HARDWARE_FAULT = (DRIVERS.state == 1);


	// CONTROL:
	RESET1 = (OPERATION.state == 1) OR (OPERATION.state == 2) OR (OPERATION.state == 3) OR (DRIVERS.state == 2);
	RESET2 = (OPERATION.state == 1) OR (OPERATION.state == 2) OR (OPERATION.state == 3) OR (DRIVERS.state == 2);

	ENABLE_CONTROL = (OPERATION.state == 3);
	EN1 = (OPERATION.state == 3);
	EN2 = (OPERATION.state == 3);
	EN3 = (OPERATION.state == 3);
	EN4 = (OPERATION.state == 3);
	EN5 = (OPERATION.state == 3);
	EN6 = (OPERATION.state == 3);
	EN7 = (OPERATION.state == 3);
	EN8 = (OPERATION.state == 3);


	// VISUALS:
	LED2 = TRUE;
	LED3 = (OPERATION.state == 2) OR (OPERATION.state == 3);
	LED4 = (OPERATION.state == 3) OR (ERROR.state == 2);

	// --------------------------------------------------------------------------------------

	// UPDATE FSMs:
	for(uint8_t i=0; i < NB_SMES; i++){

		if (smes[i].state != smes[i].prev_state){
			smes[i].prev_state = smes[i].state;
			smes[i].TIS = 0;
		}
		else {
			smes[i].TIS++;
		}

	}

	// For debugging purposes:
	LVBUS_STATE = LVBUS.state;
	HVBUS_STATE = HVBUS.state;
	OVERLOAD_STATE = OVERLOAD.state;
	DRIVERS_STATE = DRIVERS.state;
	TEMPERATURE_STATE = TEMPERATURE.state;
	COMMS_STATE = COMMS.state;
	ERROR_STATE = ERROR.state;
	OPERATION_STATE = OPERATION.state;

}

void fsm_init(void){

	for(uint8_t i=0; i < NB_SMES; i++){
		smes[i].state = 0;
		smes[i].prev_state = 255;
		smes[i].TIS = 0;
	}

}

#endif
