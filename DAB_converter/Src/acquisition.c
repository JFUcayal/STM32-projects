#include "acquisition.h"
//#include "arm_math.h"

uint8_t adc_conv_complete_flag = 0;
volatile uint16_t adc_dma_buffer[8];
int16_t	CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8;

// SENSOR GAINS:
float		VLV_SENSOR_GAIN 			= 0.0397f;  	// VLV
float		VHV_SENSOR_GAIN 			= 0.168f;  		// VHV
float		NEIGHBOUR_VHV_SENSOR_GAIN	= 0.168f;		// NEIGHBOUR VHV
float		ILV_SENSOR_GAIN 			= 0.13f;  		// ILV
float		IHV_SENSOR_GAIN 			= 0.027f;  		// IHV
float		T_SENSOR_GAIN				= -0.029f;		// NTCs

// SENSOR BIAS:
int16_t			CH1_SENSOR_BIAS = 2038;
int16_t			CH2_SENSOR_BIAS = 2038;
int16_t			CH3_SENSOR_BIAS = 2038;
int16_t			CH4_SENSOR_BIAS = 2038;
int16_t			CH5_SENSOR_BIAS = 2038;
int16_t			CH6_SENSOR_BIAS = 4076;
int16_t			CH7_SENSOR_BIAS = 4076;
int16_t			CH8_SENSOR_BIAS	= 4076;

extern float VLV, VHV, ILV, IHV, NEIGHBOUR_VHV, T_HFT, T_HS;

extern float FILT_VLV, FILT_VHV, FILT_NEIGHBOUR_VHV, FILT_ILV, FILT_IHV, FILT_T_HFT, FILT_T_HS;


//instalar arm-math.h para implementar biquad filters(CMSIS->DSP)


//arm_biquad_casd_df1_inst_f32 		BIQUAD_VLV_S;
//#define 							BIQUAD_VLV_numStages	1
//#define							BIQUAD_VLV_blockSize 	1
//
//float								BIQUAD_VLV_pState [ 4 * BIQUAD_VLV_numStages ];
//float								BIQUAD_VLV_pCoeffs[ 5 * BIQUAD_VLV_numStages ] = {	 0.02705981905126089, 0.05411963810252179, 0.02705981905126089, 1.48376607172486396, -0.59200534792990733};
//
//arm_biquad_casd_df1_inst_f32 		BIQUAD_VHV_S;
//#define 							BIQUAD_VHV_numStages	1
//#define							BIQUAD_VHV_blockSize 	1
//
//float								BIQUAD_VHV_pState [ 4 * BIQUAD_VHV_numStages ];
//float								BIQUAD_VHV_pCoeffs[ 5 * BIQUAD_VHV_numStages ] = {	 0.02705981905126089, 0.05411963810252179, 0.02705981905126089, 1.48376607172486396, -0.59200534792990733};
//
//arm_biquad_casd_df1_inst_f32 		BIQUAD_NEIGHBOUR_VHV_S;
//#define 							BIQUAD_NEIGHBOUR_VHV_numStages	1
//#define							BIQUAD_NEIGHBOUR_VHV_blockSize 	1
//
//float								BIQUAD_NEIGHBOUR_VHV_pState [ 4 * BIQUAD_NEIGHBOUR_VHV_numStages ];
//float								BIQUAD_NEIGHBOUR_VHV_pCoeffs[ 5 * BIQUAD_NEIGHBOUR_VHV_numStages ] = {	 0.02705981905126089, 0.05411963810252179, 0.02705981905126089, 1.48376607172486396, -0.59200534792990733};
//
//
//arm_biquad_casd_df1_inst_f32 		BIQUAD_ILV_S;
//#define 							BIQUAD_ILV_numStages	1
//#define							BIQUAD_ILV_blockSize 	1
//
//float								BIQUAD_ILV_pState [ 4 * BIQUAD_ILV_numStages ];
//float								BIQUAD_ILV_pCoeffs[ 5 * BIQUAD_ILV_numStages ] = {	 0.02705981905126089, 0.05411963810252179, 0.02705981905126089, 1.48376607172486396, -0.59200534792990733};
//
//arm_biquad_casd_df1_inst_f32 		BIQUAD_IHV_S;
//#define 							BIQUAD_IHV_numStages	1
//#define							BIQUAD_IHV_blockSize 	1
//
//float								BIQUAD_IHV_pState [ 4 * BIQUAD_IHV_numStages ];
//float								BIQUAD_IHV_pCoeffs[ 5 * BIQUAD_IHV_numStages ] = {	 0.02705981905126089, 0.05411963810252179, 0.02705981905126089, 1.48376607172486396, -0.59200534792990733};



void acquisition_init(void){
//	arm_biquad_cascade_df1_init_f32(&BIQUAD_VLV_S, 				BIQUAD_VLV_numStages, 			BIQUAD_VLV_pCoeffs, 			BIQUAD_VLV_pState);
//	arm_biquad_cascade_df1_init_f32(&BIQUAD_VHV_S, 				BIQUAD_VHV_numStages, 			BIQUAD_VHV_pCoeffs, 			BIQUAD_VHV_pState);
//	arm_biquad_cascade_df1_init_f32(&BIQUAD_ILV_S, 				BIQUAD_ILV_numStages, 			BIQUAD_ILV_pCoeffs, 			BIQUAD_ILV_pState);
//	arm_biquad_cascade_df1_init_f32(&BIQUAD_IHV_S, 				BIQUAD_IHV_numStages, 			BIQUAD_IHV_pCoeffs, 			BIQUAD_IHV_pState);
//	arm_biquad_cascade_df1_init_f32(&BIQUAD_NEIGHBOUR_VHV_S, 	BIQUAD_NEIGHBOUR_VHV_numStages, BIQUAD_NEIGHBOUR_VHV_pCoeffs, 	BIQUAD_NEIGHBOUR_VHV_pState);

	//Starts ADC-DMA conversion
	HAL_ADC_Start_DMA(&hadc3, (uint32_t *) adc_dma_buffer , ADC_CHANNELS_SIZE);
}

/**
  * @brief  Changes values of current and voltage
  * 		according to gain and previous adjustments.
  *
  * @param  None
  *
  * @note
  *
  * @retval None
  */
void acquire(void){

	// Map the ADC channels to the respective variables:
	VLV 			= ((float) VLV_SENSOR_MAPPING) * VLV_SENSOR_GAIN;
	VHV 			= ((float) VHV_SENSOR_MAPPING) * VHV_SENSOR_GAIN;
	ILV				= ((float) ILV_SENSOR_MAPPING) * ILV_SENSOR_GAIN;
	IHV 			= ((float) IHV_SENSOR_MAPPING) * IHV_SENSOR_GAIN;
	NEIGHBOUR_VHV 	= ((float) NEIGHBOUR_VHV_SENSOR_MAPPING) * NEIGHBOUR_VHV_SENSOR_GAIN;
	T_HFT			= ((float) T_HFT_SENSOR_MAPPING) * T_SENSOR_GAIN;
	T_HS 			= ((float) T_HS_SENSOR_MAPPING) * T_SENSOR_GAIN;


	// Filter the acquired values with digital filters:
	//BIQUAD FILTERS -> ARM_MATH.H

//	arm_biquad_cascade_df1_f32	( &BIQUAD_VLV_S, 			&(VLV), 			&(FILT_VLV), 			BIQUAD_VLV_blockSize);
//	arm_biquad_cascade_df1_f32	( &BIQUAD_VHV_S, 			&(VHV), 			&(FILT_VHV), 			BIQUAD_VHV_blockSize);
//	arm_biquad_cascade_df1_f32	( &BIQUAD_ILV_S, 			&(ILV), 			&(FILT_ILV), 			BIQUAD_ILV_blockSize);
//	arm_biquad_cascade_df1_f32	( &BIQUAD_IHV_S, 			&(IHV), 			&(FILT_IHV), 			BIQUAD_IHV_blockSize);
//	arm_biquad_cascade_df1_f32	( &BIQUAD_NEIGHBOUR_VHV_S, 	&(NEIGHBOUR_VHV), 	&(FILT_NEIGHBOUR_VHV), 	BIQUAD_NEIGHBOUR_VHV_blockSize);

		//Starts ADC-DMA conversion
		HAL_ADC_Start_DMA(&hadc3, (uint32_t *) adc_dma_buffer , ADC_CHANNELS_SIZE);
	   //ADC-DMA conversion is completed
	   if(adc_conv_complete_flag){
		   //flag reset
		   adc_conv_complete_flag = false;
  }
}


/**
  * @brief  Capture the values acquired by the ADC and stores in array via DMA
  * 		and changes the values according to bias and filters.
  * 		HAL_ADC_ConvCpltCallback function will interrupt the processor
  * 		when DMA conversion is completed.
  *
  * @param  ADC Handle
  *
  * @note   DMA conversion needs stoppage after converting because
  * 		its too fast for the processor to execute other functions & tasks.
  *
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	adc_conv_complete_flag = true;

	HAL_ADC_Stop_DMA(&hadc3);

	   CH1 = adc_dma_buffer[0] - CH1_SENSOR_BIAS;
	   CH2 = adc_dma_buffer[1] - CH2_SENSOR_BIAS;
	   CH3 = adc_dma_buffer[2] - CH3_SENSOR_BIAS;
	   CH4 = adc_dma_buffer[3] - CH4_SENSOR_BIAS;
	   CH5 = adc_dma_buffer[4] - CH5_SENSOR_BIAS;
	   CH6 = adc_dma_buffer[5] - CH6_SENSOR_BIAS;
	   CH7 = adc_dma_buffer[6] - CH7_SENSOR_BIAS;
	   CH8 = adc_dma_buffer[7] - CH8_SENSOR_BIAS;
}


