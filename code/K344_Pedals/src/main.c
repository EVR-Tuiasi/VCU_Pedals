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
#include "Adc.h"  // Find out how to generate the Adc.h header

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef struct {
  uint16_t rawl;        // Raw value of the sensor
  uint_t milivolts;     // Converted mV
} AdcChannelResult_t;

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*
* (ASK AND SPECIFY ABOUT THE CHANNEL THINGS!!!)
* J4 Connector Mapping :
* J4 Pin 1 -> PTD4 -> (ADC0) ADC0_SE8 (Channel 8)
* J4 Pin 3 -> PTB12 -> (ADC1) ADC0_SE9 (Channel 9)
* J4 Pin 5 -> PTB0 -> (ADC2) ADC1_SE8
* J4 Pin 7 -> PTB1 -> (ADC3) ADC1_SE9
* J4 Pin 9 -> PTA6/PTE11/PTA2 -> (ADC4) ADC0_SE12
* J4 Pin 11 -> PTC0/PTE10/PTA3 -> (ADC5) ADC0_SE13
*/

#define J4_NUM_CHANNELS     (6U)
#define ADC_VREF_MV         (3300U) //milivolts full-scale
#define ADC_RESOLUTION      (4096U) //12 bit ADC

// temp
#define ADC_RAW_TO_MV(raw)  ((uint32_t)(raw) * ADC_VREF_MV) / ADC_RESOLUTION)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

// Setup for J4 connector pins
static const Adc_ChannelType s_j4AdcChannels[J4_NUM_CHANNELS] =
{
  AdcConf_AdcChannel_J4_Pin1,
  AdcConf_AdcChannel_J4_Pin3,
  AdcConf_AdcChannel_J4_Pin5,
  AdcConf_AdcChannel_J4_Pin7,
  AdcConf_AdcChannel_J4_Pin9,
  AdcConf_AdcChannel_J4_Pin11
}

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
volatile AdcChannelResult_t g_j4VoltageResults[J4_NUM_CHANNELS];

volatile uint8_t g_adcConversationDone = 0U;    // Flag cleared after processing
static volatile uint8_t s_currentChannel = 0U;  // Cannel currently used

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static void ADC_StartNextConversion(void);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
// Kick-off converion for next Channel
static void ADC_StartNextConverion(void)
{
  // TO DO: Check about using Adc_StartGroupConverion / Adc_ReadChannel() helpers
  Adc_StartGroupConverion(s_j4AdcChannels[s_currentChannel]);
}

void Adc_J4_ConverionComplete_Notification(void)
{
  uint16_t rawResult = 0U;

  Adc_ReadGroup(s_j4AdcChannels[s_currentChannel], &rawResult); // read result of finished channel

  g_j4VoltageResults[s_currentChannel].raw = rawResultl
  g_j4VoltageResults[s_currentChannel].milivolts = (uint16_t)ADC_RAW_TO_MV(rawResult);

  s_currentChannel = (uint8_t)((s_currentChannel + 1U) % J4_NUM_CHANNELS);

  if (s_currentChannel == 0U) {
    g_adcConversationDone = 1U;   // All 6 channels have been read
  }

  ADC_StartNextConversion();
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

/**
* @brief        Main function of the example
* @details      Initialize the used drivers and uses the Icu
*               and Dio drivers to toggle a LED on a push button
*/
int main(void)
{
  Mcu_Init(NULL_PTR);
  Mcu_InitClock(McuConf_McuClockSettingConfig_McuClockSettingConfig_0);
  
  while(MCU_PLL_LOCKED != Mcu_GetPllStatus()){
    /* Spin */
  }

  Mcu_DistributePllClock();
  Mcu_SetMode(McuConf_McuModeSettingConf_McuModeSettingConf_0);

  Port_Init(NULL_PTR);
  Adc_Init(NULL_PTR);

  s_currentChannel = 0U;
  g_adcConversationDone = 0U;
  ADC_StartNextConversion();
  
  while(1){
    if (g_adcConverionDone != 0U){
      g_adcConverionDone != 0U;           // Clear first Flag
      
      /*
      * BLA BLA BLA all the sensor math goes here
      */
    }
  }

  return 0;
}


#ifdef __cplusplus
}
#endif

/** @} */
