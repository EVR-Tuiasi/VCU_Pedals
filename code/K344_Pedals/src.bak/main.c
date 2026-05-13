/*
*   Copyright 2020 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

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
#include "Dio.h"
#include "Adc.h"
#include "Platform.h"

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

extern ISR(Adc_1_Isr);
/* Maybe make a macro for raw to mV conversion?
// #define J4_NUM_CHANNELS     (6U)
//#define ADC_VREF_MV         (3300U)
//#define ADC_RESOLUTION      (4096U)
//#define ADC_RAW_TO_MV(raw)  ((uint32_t)(raw) * ADC_VREF_MV) / ADC_RESOLUTION)

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

Adc_ValueGroupType ResultBuffer[] = {0xaaaa};

volatile uint16_t g_adcRaw        = 0U;  /* 0–4095        */
volatile int      g_adcMillivolts = 0;   /* 0–3300 mV     */
volatile uint8_t  g_convDone      = 0U;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
void Notification_0(void);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void Notification_0(void)
{
    Adc_ReadGroup(AdcGroupSoftwareOneShot, ResultBuffer);
    g_adcRaw = ResultBuffer[0];
    g_adcMillivolts = (int)((g_adcRaw * 5000U) / 4096U);
    g_convDone = 1U;
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/



void TestDelay(uint32 delay);
void TestDelay(uint32 delay)
{
    static volatile uint32 DelayTimer = 0;
    while(DelayTimer < delay)
    {
        DelayTimer++;
    }
    DelayTimer = 0;
}


int main(void)
{

	// Clock init
    Mcu_Init(NULL_PTR);
    Mcu_InitClock(McuClockSettingConfig_0);
    //while(MCU_PLL_LOCKED != Mcu_GetPllStatus());
    Mcu_SetMode(McuModeSettingConf_0);

    // ADC interrupt
    Platform_Init(NULL_PTR);
    Platform_InstallIrqHandler(ADC1_IRQn, Adc_1_Isr, NULL_PTR);
    Platform_SetIrq(ADC1_IRQn, TRUE);

    // ADC init
    Adc_Init(NULL_PTR);
    Adc_CalibrationStatusType CalibStatus;
    Adc_Calibrate(AdcHwUnit_0, &CalibStatus);
    Adc_SetupResultBuffer(AdcGroupSoftwareOneShot, ResultBuffer);
    Adc_EnableGroupNotification(AdcGroupSoftwareOneShot);

    while(1)
    {
        g_convDone = 0U;
        Adc_StartGroupConversion(AdcGroupSoftwareOneShot);

        while(g_convDone == 0U) {}

        // g_adcRaw        = raw 12-bit value (e.g. 2048)
        // g_adcMillivolts = voltage in mV    (e.g. 1650)
    }

  return 0;
}


#ifdef __cplusplus
}
#endif

/** @} */
