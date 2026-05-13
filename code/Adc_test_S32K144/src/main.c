#include "Adc.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"

extern ISR(Adc_1_Isr);

Adc_ValueGroupType ResultBuffer[2] = {0xaaaa, 0xaaaa};

// Raw values: 0-4095
volatile uint16_t g_raw_Accel1        = 0U;
volatile uint16_t g_raw_Accel2        = 0U;

// mV values: 0-3300
volatile uint16_t g_mV_Accel1 = 0;
volatile uint16_t g_mV_Accel2 = 0;

volatile uint8_t  g_convDone      = 0U;

/* Notification called by the ADC ISR when conversion finishes */
void Notification_0(void)
{
    Adc_ReadGroup(AdcGroupSoftwareOneShot, ResultBuffer);
    g_raw_Accel1 = ResultBuffer[0];
    g_raw_Accel2 = ResultBuffer[1];
    g_mV_Accel1 = (uint16_t)(((uint32_t)g_raw_Accel1 * 5000U) / 4096U);
    g_mV_Accel2 = (uint16_t)(((uint32_t)g_raw_Accel2 * 5000U) / 4096U);
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
    Platform_InstallIrqHandler(ADC1_IRQn, Adc_1_Isr, NULL_PTR);
    Platform_SetIrq(ADC1_IRQn, TRUE);

    /* ADC init */
    Adc_Init(NULL_PTR);
//    Adc_CalibrationStatusType CalibStatus;
//    Adc_Calibrate(AdcHwUnit_1, &CalibStatus);
    Adc_SetupResultBuffer(AdcGroupSoftwareOneShot, ResultBuffer);
    Adc_EnableGroupNotification(AdcGroupSoftwareOneShot);

//    volatile uint8_t count = 0U;
//    volatile uint32_t medval = 0U;
//    volatile uint16_t media = 0U;

    /* Main loop — continuously sample */
    while(1)
    {
        g_convDone = 0U;
        Adc_StartGroupConversion(AdcGroupSoftwareOneShot);

        /* Wait for ISR to signal done */
7        while(g_convDone == 0U) {}

//        medval = medval + g_adcMillivolts;
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
