#ifndef ACQUISITION_H_
#define ACQUISITION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "adc.h"

#define MAX_TIMEOUT 1000
#define ADC_CHANNELS_SIZE 8

// MAPPING HARDWARE CHANNELS TO LOGICAL VARIABLES:
#define VLV_SENSOR_MAPPING 				CH2
#define VHV_SENSOR_MAPPING 				CH3
#define ILV_SENSOR_MAPPING 				CH5
#define IHV_SENSOR_MAPPING 				CH4
#define NEIGHBOUR_VHV_SENSOR_MAPPING 	CH1
#define T_HFT_SENSOR_MAPPING 			CH8		// NTC3
#define T_HS_SENSOR_MAPPING 			CH6		// NTC1

void acquisition_init(void);
void acquire(void);

#endif
