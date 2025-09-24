/*******************************************************************************
* File Name: app_bt_periodic_adv.h
*
* Description: This file contains the macros and function prototypes for the  
*              Bluetooth periodic advertiser functions.

*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/
#ifndef __APP_BT_PERIODIC_ADV_H__
#define __APP_BT_PERIODIC_ADV_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Header Files
 *******************************************************************************/
#include "wiced_bt_ble.h"


/*******************************************************************************
 * Constants
 ******************************************************************************/
#define APP_BT_PERIODIC_CHANGE_ADV_DATA  (1U)
#define APP_BT_PERIODIC_ADV_HANDLE       (0x01U)
#define APP_BT_PERIODIC_ADV_EVENT_PROP   (0x00U)
#define APP_BT_PERIODIC_ADV_PRI_ADV_MIN  (800U)
#define APP_BT_PERIODIC_ADV_PRI_ADV_MAX  (800U)
#define APP_BT_PERIODIC_ADV_TX_POWER     (0x7FU)
#define APP_BT_PERIODIC_SEC_ADV_MAX_SKIP (0x00U)
#define APP_BT_PERIODIC_ADV_SET_ID       (0x01U)
#define APP_BT_PERIODIC_ADV_INT_MIN      (500U)
#define APP_BT_PERIODIC_ADV_INT_MAX      (500U)
#define APP_BT_PERIODIC_ADV_PRI_CH_MAP  \
     (BTM_BLE_ADVERT_CHNL_37 | BTM_BLE_ADVERT_CHNL_38 | BTM_BLE_ADVERT_CHNL_39)

/*******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/
void app_bt_periodic_adv_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __APP_BT_PERIODIC_ADV_H__ */

/* [] END OF FILE */
