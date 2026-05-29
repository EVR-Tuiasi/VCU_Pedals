#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Pedals/Pedals.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/


/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
// Valoarea maxima de nealiniere a senzorilor pentru plauzibilitate
#define PLAUSIBILITY_VAL 100U

// Valorile voltajului sensorilor de pozitie
#define ACC_SENS1_MIN 330U // 0% travel
#define ACC_SENS1_MAX 2300U // 100% travel
#define ACC_SENS2_MIN 330U
#define ACC_SENS2_MAX 2300U
#define BRK_SENS1_MIN 330U
#define BRK_SENS1_MAX 2300U
#define BRK_SENS2_MIN 330U
#define BRK_SENS2_MAX 2300U

#define ABS_DIFF(a, b)  ((a) > (b) ? (a) - (b) : (b) - (a))

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
uint16_t g_mV_poz_Acc1;
uint16_t g_mV_poz_Acc2;
uint16_t g_mV_poz_Brk1;
uint16_t g_mV_poz_Brk2;

uint16_t g_mV_pressure;
uint16_t g_Bar_pressure;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void Pedals_SetSensorValues(uint16_t mV_Acc1, uint16_t mV_Acc2, uint16_t mV_Brk1, uint16_t mV_Brk2, uint16_t mV_Pressure1)
{
    g_mV_poz_Acc1 = mV_Acc1;
    g_mV_poz_Acc2 = mV_Acc2;
    g_mV_poz_Brk1 = mV_Brk1;
    g_mV_poz_Brk2 = mV_Brk2;
    g_mV_pressure = mV_Pressure1;
}

void Pedals_Init(void){
    //TODO: Implementation
}

void Pedals_Test(void){
    //TODO: Implementation
}

PedalsErrors_t Pedals_GetErrors(void){
    PedalsErrors_t ErrorData = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if(g_mV_poz_Acc1 == 0) {
        ErrorData.Accel_Sensor1_ShortToGnd = 1;
    }else if(g_mV_poz_Acc1 == 3.3) {
        ErrorData.Accel_Sensor1_ShortToVcc = 1;
    } else if(g_mV_poz_Acc1 < 0 || g_mV_poz_Acc1 > 3.3) {
        ErrorData.Accel_Sensor1_OutOfRangeOutput = 1;
    }

    if(g_mV_poz_Acc2 == 0) {
        ErrorData.Accel_Sensor2_ShortToGnd = 1;
    }else if(g_mV_poz_Acc2 == 3.3) {
        ErrorData.Accel_Sensor2_ShortToVcc = 1;
    } else if(g_mV_poz_Acc2 < 0 || g_mV_poz_Acc2 > 3.3) {
        ErrorData.Accel_Sensor2_OutOfRangeOutput = 1;
    }

    if(ABS_DIFF(g_mV_poz_Acc1, g_mV_poz_Acc2) > PLAUSIBILITY_VAL)
    {
        ErrorData.Accel_Implausibility = 1;
    }

    if(g_mV_poz_Brk1 == 0) {
        ErrorData.Brake_Sensor1_ShortToGnd = 1;
    }else if(g_mV_poz_Brk1 == 3.3) {
        ErrorData.Brake_Sensor1_ShortToVcc = 1;
    } else if(g_mV_poz_Brk1 < 0 || g_mV_poz_Brk1 > 3.3) {
        ErrorData.Brake_Sensor1_OutOfRangeOutput = 1;
    }

    if(g_mV_poz_Brk2 == 0) {
        ErrorData.Brake_Sensor2_ShortToGnd = 1;
    }else if(g_mV_poz_Brk2 == 3.3) {
        ErrorData.Brake_Sensor2_ShortToVcc = 1;
    } else if(g_mV_poz_Brk2 < 0 || g_mV_poz_Brk2 > 3.3) {
        ErrorData.Brake_Sensor2_OutOfRangeOutput = 1;
    }

    if(ABS_DIFF(g_mV_poz_Brk1, g_mV_poz_Brk2) > PLAUSIBILITY_VAL)
    {
        ErrorData.Accel_Implausibility = 1;
    }

    return ErrorData;
}

uint32_t Pedals_GetData(Pedal_t PedalSelect, Sensor_t SensorSelect, PedalValue_t DesiredValueType){
    switch(PedalSelect){
		case ACCEL:
			if(SensorSelect == SENSOR1){
				if(DesiredValueType == VOLTAGE)
					return g_mV_poz_Acc1;
				else if(DesiredValueType == PERCENTAGE)
					return ((uint32_t)(g_mV_poz_Acc1 - ACC_SENS1_MIN) * 100U) / (ACC_SENS1_MAX - ACC_SENS1_MIN);
			} else if(SensorSelect == SENSOR2) {
				if(DesiredValueType == VOLTAGE)
					return g_mV_poz_Acc2;
				else if(DesiredValueType == PERCENTAGE)
					return ((uint32_t)(g_mV_poz_Acc2 - ACC_SENS2_MIN) * 100U) / (ACC_SENS2_MAX - ACC_SENS2_MIN);
			}
			break;
		case BRAKE:
			if(SensorSelect == SENSOR1) {
				if(DesiredValueType == VOLTAGE)
					return g_mV_poz_Brk1;
				else if(DesiredValueType == PERCENTAGE)
					return ((uint32_t)(g_mV_poz_Brk1 - BRK_SENS1_MIN) * 100U) / (BRK_SENS1_MAX - BRK_SENS1_MIN);
			} else if(SensorSelect == SENSOR2) {
				if(DesiredValueType == VOLTAGE)
					return g_mV_poz_Brk2;
				else if(DesiredValueType == PERCENTAGE)
					return ((uint32_t)(g_mV_poz_Brk2 - BRK_SENS2_MIN) * 100U) / (BRK_SENS2_MAX - BRK_SENS2_MIN);
			}
        break;
    }
}

uint32_t Pedals_GetPressure(BrakePressure_t Value){
    switch(Value){
    case PRESSURE_VOLTAGE:
    	return g_mV_pressure;
    	break;
    case BARS:
    	return g_Bar_pressure;  // 0.5-4.5V masoara 0-1.2Mpa aka 12 bar
    	break;
    }
}

#ifdef __cplusplus
}
#endif
