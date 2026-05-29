#include "Adc.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "Pedals/Pedals.h"
#include "Communications/CanMessaging.h"

extern ISR(Adc_1_Isr);

Adc_ValueGroupType ResultBuffer[3] = {0xaaaa, 0xaaaa, 0xaaaa};
Adc_ValueGroupType BrakesBuffer[2] = {0xaaaa, 0xaaaa};

volatile uint8_t  g_convDone      = 0U;

volatile uint16_t g_mV_poz_Accel1;
volatile uint16_t g_mV_poz_Accel2;
volatile uint16_t g_mV_poz_Brake1;
volatile uint16_t g_mV_poz_Brake2;
volatile uint16_t g_mV_Pressure;

volatile uint16_t g_raw_poz_Accel1;
volatile uint16_t g_raw_poz_Accel2;
volatile uint16_t g_raw_poz_Brake1;
volatile uint16_t g_raw_poz_Brake2;
volatile uint16_t g_raw_Pressure;

/* Notification called by the ADC ISR when conversion finishes */
void Notification_0(void)
{
    Adc_ReadGroup(AdcGroupSoftwareOneShot1, ResultBuffer);
    g_raw_poz_Accel1 = ResultBuffer[0];
    g_raw_poz_Accel2 = ResultBuffer[1];
    g_raw_Pressure = ResultBuffer[2];
    g_mV_poz_Accel1 = (uint16_t)(((uint32_t)g_raw_poz_Accel1 * 5000U) / 4096U);
    g_mV_poz_Accel2 = (uint16_t)(((uint32_t)g_raw_poz_Accel2 * 5000U) / 4096U);
    g_mV_Pressure = (uint16_t)(((uint32_t)g_raw_Pressure * 5000U) / 4096U);
    g_convDone = 1U;
}

int main(void)
{
    /* Clock init */
    Mcu_Init(NULL_PTR);
    Mcu_InitClock(McuClockSettingConfig_0);
//    while(MCU_PLL_LOCKED != Mcu_GetPllStatus());
    Mcu_SetMode(McuModeSettingConf_0);

    Port_Init(NULL_PTR);

    /* Install ADC interrupt */
    Platform_Init(NULL_PTR);
    Platform_InstallIrqHandler(ADC0_IRQn, Adc_0_Isr, NULL_PTR);
    Platform_SetIrq(ADC0_IRQn, TRUE);
    Platform_InstallIrqHandler(ADC1_IRQn, Adc_1_Isr, NULL_PTR);
    Platform_SetIrq(ADC1_IRQn, TRUE);

    /* ADC init */
    Adc_Init(NULL_PTR);
//    Adc_CalibrationStatusType CalibStatus;
//    Adc_Calibrate(AdcHwUnit_1, &CalibStatus);
    Adc_SetupResultBuffer(AdcGroupSoftwareOneShot1, ResultBuffer);
    Adc_EnableGroupNotification(AdcGroupSoftwareOneShot1);

//    volatile uint8_t count = 0U;
//    volatile uint32_t medval = 0U;
//    volatile uint16_t media = 0U;

    /* Main loop — continuously sample */
    while(1)
    {
        g_convDone = 0U;
        Adc_StartGroupConversion(AdcGroupSoftwareOneShot1);

        /* Wait for ISR to signal done */
        while(g_convDone == 0U) {}

        Pedals_SetSensorValues(g_mV_poz_Accel1, g_mV_poz_Accel2, 0U, 0U, g_mV_Pressure);

        PedalsErrors_t PedalsErrors = Pedals_GetErrors();

        CanMessaging_Init();

        CanMessaging_SetValue(Can_PEDALS_AcceleratorSensor1Voltage, Pedals_GetData(ACCEL, SENSOR1, VOLTAGE));
        CanMessaging_SetValue(Can_PEDALS_AcceleratorSensor2Voltage, Pedals_GetData(ACCEL, SENSOR2, VOLTAGE));
        CanMessaging_SetValue(Can_PEDALS_AcceleratorSensor1TravelPercentage, Pedals_GetData(ACCEL, SENSOR1, PERCENTAGE));
        CanMessaging_SetValue(Can_PEDALS_AcceleratorSensor2TravelPercentage, Pedals_GetData(ACCEL, SENSOR2, PERCENTAGE));

        /*
         * TO DO: Implement the brake system CAN messaging
         *
         * Pedals_GetData(BRAKE, SENSOR*, TYPE) // For voltage/travel percent, replace * with 1 or 2; Type - VOLTAGE / PERCENTAGE
         *
         * Pedals_GetPressure(TYPE) // Type - PRESSURE_VOLTAGE / BARS
         */
        CanMessaging_SetValue(Can_PEDALS_BrakeSensor1Voltage, 0);
        CanMessaging_SetValue(Can_PEDALS_BrakeSensor2Voltage, 0);
        CanMessaging_SetValue(Can_PEDALS_BrakeSensor1TravelPercentage, 0);
        CanMessaging_SetValue(Can_PEDALS_BrakeSensor2TravelPercentage, 0);
        CanMessaging_SetValue(Can_PEDALS_PressureSensorVoltage, 0);
        CanMessaging_SetValue(Can_PEDALS_PressureSensorBars, 0);


        CanMessaging_SetValue(Can_PEDALS_Accel_Sensor1_ShortToGnd, PedalsErrors.Accel_Sensor1_ShortToGnd);
        CanMessaging_SetValue(Can_PEDALS_Accel_Sensor1_ShortToVcc, PedalsErrors.Accel_Sensor1_ShortToVcc);
        CanMessaging_SetValue(Can_PEDALS_Accel_Sensor1_OutOfRangeOutput, PedalsErrors.Accel_Sensor1_OutOfRangeOutput);
        CanMessaging_SetValue(Can_PEDALS_Accel_Sensor2_ShortToGnd, PedalsErrors.Accel_Sensor2_ShortToGnd);
        CanMessaging_SetValue(Can_PEDALS_Accel_Sensor2_ShortToVcc, PedalsErrors.Accel_Sensor2_ShortToVcc);
        CanMessaging_SetValue(Can_PEDALS_Accel_Sensor2_OutOfRangeOutput, PedalsErrors.Accel_Sensor2_OutOfRangeOutput);
        CanMessaging_SetValue(Can_PEDALS_Accel_Implausibility, PedalsErrors.Accel_Implausibility);
        CanMessaging_SetValue(Can_PEDALS_Brake_Sensor1_ShortToGnd, PedalsErrors.Brake_Sensor1_ShortToGnd);
        CanMessaging_SetValue(Can_PEDALS_Brake_Sensor1_ShortToVcc, PedalsErrors.Brake_Sensor1_ShortToVcc);
        CanMessaging_SetValue(Can_PEDALS_Brake_Sensor1_OutOfRangeOutput, PedalsErrors.Brake_Sensor1_OutOfRangeOutput);
        CanMessaging_SetValue(Can_PEDALS_Brake_Sensor2_ShortToGnd, PedalsErrors.Brake_Sensor2_ShortToGnd);
        CanMessaging_SetValue(Can_PEDALS_Brake_Sensor2_ShortToVcc, PedalsErrors.Brake_Sensor2_ShortToVcc);
        CanMessaging_SetValue(Can_PEDALS_Brake_Sensor2_OutOfRangeOutput, PedalsErrors.Brake_Sensor2_OutOfRangeOutput);
        CanMessaging_SetValue(Can_PEDALS_Brake_Implausibility, PedalsErrors.Brake_Implausibility);

        CanMessaging_CreateBuffer(idCanAcceleratie);
        CanMessaging_CreateBuffer(idCanFrana);

//        medval = medval + g_mV_poz_Accel1;
//
//        count++;
//
//        if(count == 10U) {
//        	media = (uint16_t)(medval / 10U);
//        	medval = 0;
//        	count = 0;
//        }

        /*
         * g_adcRaw        = raw 12-bit value (e.g. 2048)
         * g_adcMillivolts = voltage in mV    (e.g. 1650)
         */
    }

    return 0;
}
