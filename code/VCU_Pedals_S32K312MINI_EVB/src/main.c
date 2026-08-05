
#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcu.h"
#include "Port.h"
#include "Platform.h"
#include "Can_GeneralTypes.h"
#include "Can_43_FLEXCAN.h"
#include "CanIf.h"
#include "SchM_Can_43_FLEXCAN.h"
#include "CDD_Uart.h"
#include "Dio.h"
#include "Mcl.h"
#include "Adc.h"
#include "Pwm.h"
#include "Gpt.h"
#include "CanMessaging.h"
#include "UartMessaging.h"
#include "Messaging.h"
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


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
extern MonitoredValues_t MonitoredValues;
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(void)
{
	Mcu_Init(NULL_PTR);
	Mcu_InitClock(McuModeSettingConf_0);
	while(MCU_PLL_LOCKED != Mcu_GetPllStatus())
	{
		 ;
	}
	Mcu_DistributePllClock();
	Mcu_SetMode(McuModeSettingConf_0);
	Mcl_Init(NULL_PTR);
	Port_Init(NULL_PTR);
	Platform_Init(NULL_PTR);
	Adc_Init(NULL_PTR);
	Can_43_FLEXCAN_Init(NULL_PTR);
	CanIf_Init(NULL_PTR);
	Pwm_Init(NULL_PTR);
	Gpt_Init(NULL_PTR);

	CanMessaging_Init();
	//UartMessaging_Init();
	Pedals_Init();

	while(1){
		Pedals_Update();
		Pedals_Test();
		WriteCanDataAtAddress(Pedals_GetData(ACCEL, SENSOR1, VOLTAGE), &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1Voltage);
		WriteCanDataAtAddress(Pedals_GetData(ACCEL, SENSOR2, VOLTAGE), &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2Voltage);
		WriteCanDataAtAddress(Pedals_GetData(BRAKE, SENSOR1, VOLTAGE), &MonitoredValues.PedalsMonitoredValues.BrakeSensor1Voltage);
		WriteCanDataAtAddress(Pedals_GetData(BRAKE, SENSOR2, VOLTAGE), &MonitoredValues.PedalsMonitoredValues.BrakeSensor2Voltage);

		WriteCanDataAtAddress(Pedals_GetData(ACCEL, SENSOR1, PERCENTAGE), &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1TravelPercentage);
		WriteCanDataAtAddress(Pedals_GetData(ACCEL, SENSOR2, PERCENTAGE), &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2TravelPercentage);
		WriteCanDataAtAddress(Pedals_GetData(BRAKE, SENSOR1, PERCENTAGE), &MonitoredValues.PedalsMonitoredValues.BrakeSensor1TravelPercentage);
		WriteCanDataAtAddress(Pedals_GetData(BRAKE, SENSOR2, PERCENTAGE), &MonitoredValues.PedalsMonitoredValues.BrakeSensor2TravelPercentage);

		WriteCanDataAtAddress(Pedals_GetError(ACCEL, SENSOR1, SHORT_TO_GND), &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToGnd);
		WriteCanDataAtAddress(Pedals_GetError(ACCEL, SENSOR2, SHORT_TO_GND), &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToGnd);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, SENSOR1, SHORT_TO_GND), &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToGnd);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, SENSOR2, SHORT_TO_GND), &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToGnd);

		WriteCanDataAtAddress(Pedals_GetError(ACCEL, SENSOR1, SHORT_TO_VCC), &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToVcc);
		WriteCanDataAtAddress(Pedals_GetError(ACCEL, SENSOR2, SHORT_TO_VCC), &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToVcc);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, SENSOR1, SHORT_TO_VCC), &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToVcc);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, SENSOR2, SHORT_TO_VCC), &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToVcc);

		WriteCanDataAtAddress(Pedals_GetError(ACCEL, SENSOR1, OUT_OF_RANGE_OUTPUT), &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_OutOfRangeOutput);
		WriteCanDataAtAddress(Pedals_GetError(ACCEL, SENSOR2, OUT_OF_RANGE_OUTPUT), &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_OutOfRangeOutput);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, SENSOR1, OUT_OF_RANGE_OUTPUT), &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_OutOfRangeOutput);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, SENSOR2, OUT_OF_RANGE_OUTPUT), &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_OutOfRangeOutput);

		WriteCanDataAtAddress(Pedals_GetError(ACCEL, 0, IMPLAUSIBILITY), &MonitoredValues.PedalsMonitoredValues.Accel_Implausibility);
		WriteCanDataAtAddress(Pedals_GetError(BRAKE, 0, IMPLAUSIBILITY), &MonitoredValues.PedalsMonitoredValues.Brake_Implausibility);

		WriteCanDataAtAddress(Pedals_GetPressure(PRESSURE_VOLTAGE), &MonitoredValues.PedalsMonitoredValues.PressureSensorVoltage);
		WriteCanDataAtAddress(Pedals_GetPressure(BARS), &MonitoredValues.PedalsMonitoredValues.PressureSensorBars);

		CanMessaging_Update();
	}
}


#ifdef __cplusplus
}
#endif

/** @} */
