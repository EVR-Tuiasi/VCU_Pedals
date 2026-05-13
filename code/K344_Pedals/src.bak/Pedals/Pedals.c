#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Pedals.h"

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
#define ACC_SEN1 = 0U; // 14 bits for each sensor voltage
#define ACC_SEN2 = 0U;
#define BRK_SEN1 = 0U;
#define BRK_SEN2 = 0U;
#define PRES_SEN1 = 0U; // 9 bits for 0-500 (5V max, 0.01V per bit)
#define PRES_SEN2 = 0U;
#define PRESSURE_BARS = 0U;

// Valoarea maxima de nealiniere a senzorilor pentru plauzibilitate
#define PLAUSIBILITY_VAL = 0.1;

// Valorile voltajului sensorilor de pozitie
#define ACC_SENS1_MIN = 0U; // 0% travel
#define ACC_SENS1_MAX = 0U; // 100% travel
#define ACC_SENS2_MIN = 0U;
#define ACC_SENS2_MAX = 0U;
#define BRK_SENS1_MIN = 0U;
#define BRK_SENS1_MAX = 0U;
#define BRK_SENS2_MIN = 0U;
#define BRK_SENS2_MAX = 0U;

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void Pedals_Init(void){
    //TODO: Implementation
}

void Pedals_Test(void){
    //TODO: Implementation
}

PedalsErrors_t Pedals_GetErrors(void){
    PedalsErrors_t ErrorData
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if(ACC_SEN1 == 0) {
        ErrorData.Accel_Sensor1_ShortToGnd = 1;
    }else if(ACC_SEN1 == 3.3) {
        ErrorData.Accel_Sensor1_ShortToVcc = 1;
    } else if(ACC_SEN1 < 0 || ACC_SEN1 > 3.3) {
        ErrorData.Accel_Sensor1_OutOfRangeOutput = 1;
    }

    if(ACC_SEN2 == 0) {
        ErrorData.Accel_Sensor2_ShortToGnd = 1;
    }else if(ACC_SEN2 == 3.3) {
        ErrorData.Accel_Sensor2_ShortToVcc = 1;
    } else if(ACC_SEN2 < 0 || ACC_SEN2 > 3.3) {
        ErrorData.Accel_Sensor2_OutOfRangeOutput = 1;
    }

    if(abs(ACC_SEN1 - ACC_SEN2) > PLAUSIBILITY_VAL) {
        ErrorData.Accel_Implausibility = 1;
    }

    if(BRK_SEN1 == 0) {
        ErrorData.Brake_Sensor1_ShortToGnd = 1;
    }else if(BRK_SEN1 == 3.3) {
        ErrorData.Brake_Sensor1_ShortToVcc = 1;
    } else if(BRK_SEN1 < 0 || ACC_SEN1 > 3.3) {
        ErrorData.Brake_Sensor1_OutOfRangeOutput = 1;
    }

    if(BRK_SEN2 == 0) {
        ErrorData.Brake_Sensor2_ShortToGnd = 1;
    }else if(BRK_SEN2 == 3.3) {
        ErrorData.Brake_Sensor2_ShortToVcc = 1;
    } else if(BRK_SEN2 < 0 || ACC_SEN2 > 3.3) {
        ErrorData.Brake_Sensor2_OutOfRangeOutput = 1;
    }

    if(abs(BRK_SEN1 - ACC_SEN2) > PLAUSIBILITY_VAL) {
        ErrorData.Brake_Implausibility = 1;
    }

    return ErrorData;
}

uint32_t Pedals_GetData(Pedal_t PedalSelect, Sensor_t SensorSelect, PedalValue_t DesiredValueType){
    switch(PedalSelect){
        case ACCEL:
        if(SensorSelect == SENSOR1){
            if(DesiredValueType == VOLTAGE)
                return ACC_SEN1;
            else if(DesiredValueType == PERCENTAGE)
                return (ACC_SEN1 - ACC_SEN1_MIN) / (ACC_SEN1_MAX - ACC_SEN1_MIN) * 100;
        } else if(SensorSelect == SENSOR2) {
            if(DesiredValueType == VOLTAGE)
                return ACC_SEN2;
            else if(DesiredValueType == PERCENTAGE)
                return (ACC_SEN2 - ACC_SEN2_MIN) / (ACC_SEN2_MAX - ACC_SEN2_MIN) * 100;
        }
        break;
        case BRAKE:
        if(SensorSelect == SENSOR1) {
            if(DesiredValueType == VOLTAGE)
                return BRK_SEN1;
            else if(DesiredValueType == PERCENTAGE)
                return (BRK_SEN1 - BRK_SEN1_MIN) / (BRK_SEN1_MAX - BRK_SEN1_MIN) * 100;
        } else if(SensorSelect == SENSOR2) {
            if(DesiredValueType == VOLTAGE)
                return BRK_SEN2;
            else if(DesiredValueType == PERCENTAGE)
                return (BRK_SEN2 - BRK_SEN2_MIN) / (BRK_SEN2_MAX - BRK_SEN2_MIN) * 100;
        }
        break;
    }
}

uint32_t Pedals_GetPressure(BrakePressure_t Value){
    switch(Value){
    case PRESSURE_VOLTAGE:
    return PRES_SEN1;
    break;
    case BARS
    /*
     * TODO: Check pressure sensor specification
     *       Implement the formula for 0-500 voltage to 0-255 bars
     */
    return PRES_SEN1 * 0.51;
}
}

#ifdef __cplusplus
}
#endif
