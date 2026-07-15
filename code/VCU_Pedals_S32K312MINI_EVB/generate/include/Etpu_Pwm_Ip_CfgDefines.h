/*==================================================================================================
*   Project              : RTD AUTOSAR 4.9
*   Platform             : CORTEXM
*   Peripheral           : Emios Flexio FlexPwm eTpu
*   Dependencies         : none
*
*   Autosar Version      : 4.9.0
*   Autosar Revision     : ASR_REL_4_9_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 7.0.1
*   Build Version        : S32K3_RTD_7_0_1_D2602_ASR_REL_4_9_REV_0000_20260206
*
*   Copyright 2020 - 2026 NXP
*
*   NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be 
*   used strictly in accordance with the applicable license terms. By expressly 
*   accepting such terms or by downloading, installing, activating and/or otherwise 
*   using the software, you are agreeing that you have read, and that you agree to 
*   comply with and are bound by, such license terms. If you do not agree to be 
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

#ifndef ETPU_PWM_IP_CFGDEFINES_H
#define ETPU_PWM_IP_CFGDEFINES_H

/**
*   @file       Eptu_Pwm_Ip_CfgDefines.h
*
*   @addtogroup etpu_pwm_ip ETPU Pwm IPL
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/* Standard Types */
#include "Std_Types.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETPU_PWM_IP_CFGDEFINES_VENDOR_ID                    43
#define ETPU_PWM_IP_CFGDEFINES_MODULE_ID                    121
#define ETPU_PWM_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION     4
#define ETPU_PWM_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION     9
#define ETPU_PWM_IP_CFGDEFINES_AR_RELEASE_REVISION_VERSION  0
#define ETPU_PWM_IP_CFGDEFINES_SW_MAJOR_VERSION             7
#define ETPU_PWM_IP_CFGDEFINES_SW_MINOR_VERSION             0
#define ETPU_PWM_IP_CFGDEFINES_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Std_Types.h are of the same AUTOSAR version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((ETPU_PWM_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
         (ETPU_PWM_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR version numbers of EMIOS_Pwm_Ip_CfgDefines.h and Std_Types.h are different."
    #endif
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** @brief      Enable the Etpu Ip */
#define ETPU_PWM_IP_USED                            (STD_OFF)

#if (ETPU_PWM_IP_USED == STD_ON)

/** @brief        Macros to indicate eTPU channels used. */

/**
* @brief        Macros used to enable ISR for eTPU channel
*
*/

/** @brief Maximum value of the period */
#define ETPU_PWM_IP_MAX_PERIOD                      0x007FFFFF

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
 * @brief       Notification function callback type.
 */
typedef void (* Etpu_Pwm_Ip_CallbackType)(uint8 param);

/**
 * @brief       Structure for notification
 * @details     The structure used to notification
 */
typedef struct
{
    /** @brief Callback function pointer */
    Etpu_Pwm_Ip_CallbackType CbFunction;
    /** @brief Callback function parameter pointer */
    uint8 CbParameter;
} Etpu_Pwm_Ip_NotificationType;

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#endif /* ETPU_PWM_IP_USED == STD_ON */
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ETPU_PWM_IP_CFGDEFINES_H */
