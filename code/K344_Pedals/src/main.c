/*==================================================================================================
* Project : RTD AUTOSAR 4.7
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.7.0
* Autosar Revision : ASR_REL_4_7_REV_0000
* Autosar Conf.Variant :
* SW Version : 4.0.0
* Build Version : S32K3_RTD_4_0_0_D2311_ASR_REL_4_7_REV_0000_20231128
*
* Copyright 2020 - 2023 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/* Including necessary configuration files. */
#include "Mcal.h"
#include "Mcu.h"
#include "Adc.h"
#include "Port.h"
#include "Platform.h"
#include "IntCtrl_Ip.h"

volatile int exit_code = 0;

extern ISR(Adc_Sar_0_Isr);

Adc_ValueGroupType ResultBuffer[] = {0xaaaa};

volatile uint16_t g_adcRaw        = 0U;  /* 0–4095        */
volatile int      g_adcMillivolts = 0;   /* 0–3300 mV     */
volatile uint8_t  g_convDone      = 0U;

void Notification_0(void)
{
    Adc_ReadGroup(AdcGroup_0, ResultBuffer);
    g_adcRaw = ResultBuffer[0];
    g_adcMillivolts = (int)((g_adcRaw * 5000U) / 4096U);
    g_convDone = 1U;
}

int main(void)
{
	/* Clock init */
	    Mcu_Init(NULL_PTR);
	    Mcu_InitClock(McuClockSettingConfig_0);
	    while(MCU_PLL_LOCKED != Mcu_GetPllStatus());
	    Mcu_SetMode(McuModeSettingConf_0);

	    Port_Init(NULL_PTR);

	    /* ADC interrupt */
//	    Platform_Init(NULL_PTR);
	    Platform_InstallIrqHandler(ADC0_IRQn, Adc_Sar_0_Isr, NULL_PTR);
	    Platform_SetIrq(ADC0_IRQn, TRUE);

	    /* ADC init */
	    Adc_Init(NULL_PTR);
//	    Adc_CalibrationStatusType CalibStatus;
//	    Adc_Calibrate(AdcHwUnit_0, &CalibStatus);
	    Adc_SetupResultBuffer(AdcGroup_0, ResultBuffer);
	    Adc_EnableGroupNotification(AdcGroup_0);

	    while(1)
	    {
	        g_convDone = 0U;
	        Adc_StartGroupConversion(AdcGroup_0);

	        while(g_convDone == 0U) {}

	        /*
	         * g_adcRaw        = raw 12-bit value (e.g. 2048)
	         * g_adcMillivolts = voltage in mV    (e.g. 1650)
	         */
	    }

    for(;;)
    {
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/** @} */
