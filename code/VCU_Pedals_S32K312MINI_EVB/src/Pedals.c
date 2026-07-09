#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Adc.h"
#include "Pedals.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
static Adc_ValueGroupType buffer0[4];
static Adc_ValueGroupType buffer1[1];
static PedalsData_t date_pedale;
static SensorLimits acc1 = {2600, 4500};
static SensorLimits acc2 = {3130, 1670};
static SensorLimits brake1 = {0, 0};
static SensorLimits brake2 = {0, 0};

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void Pedals_Init(void){
	Adc_SetupResultBuffer(AdcGroup_0, buffer0);
	Adc_SetupResultBuffer(AdcGroup_1, buffer1);
}
void Pedals_Test(void){

}
PedalsErrors_t Pedals_GetErrors(void){

}
uint32_t Pedals_GetData(Pedal_t PedalSelect, Sensor_t SensorSelect, PedalValue_t DesiredValueType){
	switch(DesiredValueType){
		case VOLTAGE:
			if(SensorSelect == SENSOR1){
				if(PedalSelect == ACCEL)
					return date_pedale.AcceleratorSensor1Voltage;
				else
					return date_pedale.BrakeSensor1Voltage;
			}
			else{
				if(PedalSelect == ACCEL)
					return date_pedale.AcceleratorSensor2Voltage;
				else
					return date_pedale.BrakeSensor2Voltage;
			}
			break;
		case PERCENTAGE:
			if(SensorSelect == SENSOR1){
				if(PedalSelect == ACCEL)
					return date_pedale.AcceleratorSensor1TravelPercentage;
				else
					return date_pedale.BrakeSensor1TravelPercentage;
			}
			else{
				if(PedalSelect == ACCEL)
					return date_pedale.AcceleratorSensor2TravelPercentage;
				else
					return date_pedale.BrakeSensor2TravelPercentage;
			}
			break;
	}
	return 0;
}
uint32_t Pedals_GetPressure(BrakePressure_t ValueType){
	switch(ValueType){
		case BARS:
			return date_pedale.PressureSensorBars;
			break;
		case PRESSURE_VOLTAGE:
			return date_pedale.PressureSensorVoltage;
			break;
	}
	return 0;
}

void Pedals_Update(void){

	volatile uint32_t i = 50000;

	Adc_StartGroupConversion(AdcGroup_0);
	Adc_StartGroupConversion(AdcGroup_1);

	while(Adc_GetGroupStatus(AdcGroup_0) == ADC_BUSY);
	while(Adc_GetGroupStatus(AdcGroup_1) == ADC_BUSY);

	Adc_ReadGroup(AdcGroup_0, buffer0);
	Adc_ReadGroup(AdcGroup_1, buffer1);
	while(i--);
	i = 50000;

	date_pedale.AcceleratorSensor1Voltage = buffer0[0];
	date_pedale.AcceleratorSensor1TravelPercentage = 100*(buffer0[0]-acc1.start_valid)/(acc1.end_valid-acc1.start_valid);

	date_pedale.AcceleratorSensor2Voltage = buffer0[1];
	date_pedale.AcceleratorSensor2TravelPercentage = 100*(acc2.start_valid-buffer0[1])/(acc2.start_valid-acc2.end_valid);

	date_pedale.BrakeSensor1Voltage = buffer0[2];
	date_pedale.BrakeSensor2Voltage = buffer0[3];
}

#ifdef __cplusplus
}
#endif
