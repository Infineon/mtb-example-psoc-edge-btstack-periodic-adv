/*******************************************************************************
* File Name: app_bt_periodic_adv.c
*
* Description: This file contains the definitions for the Bluetooth periodic 
*              advertiser functions.
*
* Related Document: See README.md
*
********************************************************************************
* (c) 2024-2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
/* FreeRTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "timers.h"
#include "stdio.h"
#include "app_bt_periodic_adv.h"
#include "wiced_bt_dev.h"
#include "retarget_io_init.h"

/*******************************************************************************
* Defines
*******************************************************************************/
#define SIMULATE_SENSOR_TIMEOUT_IN_MS (5000U)
#define BDA_RESET_VAL                 (0U)
#define SENSOR_DATA_RESET_VAL         (0U)
#define SENSOR_DATA_ARRAY_SIZE        (240U)
#define ENABLE_PERIODIC_CHANGE        (1U)
#define ENABLE_PERIODIC_ADV           (1U)
#define INDEX_INIT_VAL                (0U)
#define ADV_DURATIION                 (0U)
#define MAX_EXT_ADV_EVENTS            (0U)
#define ADVERTISING_PROPERTIES_NULL   (0U)
#define TIMER_TICKS_TO_WAIT           (10U)
#define EXT_ADV_NUM_SETS              (1U)
#define DEVICE_BDA_0                  (0x20U)
#define DEVICE_BDA_1                  (0x82U)
#define DEVICE_BDA_2                  (0x9BU)
#define DEVICE_BDA_3                  (0x01U)
#define DEVICE_BDA_4                  (0x22U)
#define DEVICE_BDA_5                  (0x33U)
#define LOCAL_ADDR_INDEX_0            (0U)
#define LOCAL_ADDR_INDEX_1            (1U)
#define LOCAL_ADDR_INDEX_2            (2U)
#define LOCAL_ADDR_INDEX_3            (3U)
#define LOCAL_ADDR_INDEX_4            (4U)
#define LOCAL_ADDR_INDEX_5            (5U)


/******************************************************************************
* Global Variables
******************************************************************************/
wiced_bt_device_address_t NULL_BDA = {BDA_RESET_VAL};
uint8_t simulate_sensor_data[SENSOR_DATA_ARRAY_SIZE] = {SENSOR_DATA_RESET_VAL};

#if (ENABLE_PERIODIC_CHANGE == APP_BT_PERIODIC_CHANGE_ADV_DATA)
static TimerHandle_t simulate_sensor_timer = NULL;
#endif

wiced_ble_ext_adv_params_t ext_adv_param=
{
    .event_properties = APP_BT_PERIODIC_ADV_EVENT_PROP,
    .primary_adv_int_min = APP_BT_PERIODIC_ADV_PRI_ADV_MIN,
    .primary_adv_int_max = APP_BT_PERIODIC_ADV_PRI_ADV_MAX,
    .primary_adv_channel_map = APP_BT_PERIODIC_ADV_PRI_CH_MAP,
    .own_addr_type = BLE_ADDR_PUBLIC,
    .peer_addr_type = BLE_ADDR_PUBLIC,
    .peer_addr = {BDA_RESET_VAL},
    .adv_filter_policy = BTM_BLE_ADV_POLICY_ACCEPT_CONN_AND_SCAN,
    .adv_tx_power = APP_BT_PERIODIC_ADV_TX_POWER,
    .primary_adv_phy = WICED_BLE_EXT_ADV_PHY_1M,
    .secondary_adv_max_skip =APP_BT_PERIODIC_SEC_ADV_MAX_SKIP,
    .secondary_adv_phy =WICED_BLE_EXT_ADV_PHY_2M,
    .adv_sid = APP_BT_PERIODIC_ADV_SET_ID,
    .scan_request_not =WICED_BLE_EXT_ADV_SCAN_REQ_NOTIFY_DISABLE,
    .primary_phy_opts = WICED_BLE_EXT_ADV_PHY_OPTIONS_NO_PREFERENCE,
    .secondary_phy_opts = WICED_BLE_EXT_ADV_PHY_OPTIONS_NO_PREFERENCE,
};
wiced_ble_padv_params_t padv_params=
{
    .adv_int_min =APP_BT_PERIODIC_ADV_INT_MIN,
    .adv_int_max=APP_BT_PERIODIC_ADV_INT_MAX,
    .adv_properties =ADVERTISING_PROPERTIES_NULL,
};

/*******************************************************************************
* Function Definitions
*******************************************************************************/

#if (ENABLE_PERIODIC_CHANGE == APP_BT_PERIODIC_CHANGE_ADV_DATA)
/*******************************************************************************
* Function Name: app_bt_periodic_adv_generate_simulate_data_cb
********************************************************************************
* Summary:
*  This Function is the callback of 5second periodic timer expired to update
*  the advertiser data
*
* Parameters:
*  TimerHandle_t cb_params: rtos timer callback in-parameters
*
* Return:
*  None
*******************************************************************************/
void app_bt_periodic_adv_generate_simulate_data_cb(TimerHandle_t cb_params)
{
    uint16_t i = INDEX_INIT_VAL;
    wiced_bt_dev_status_t result = WICED_BT_SUCCESS;
    static uint8_t simulated_data;
    simulated_data++;
    for (i = INDEX_INIT_VAL; i < sizeof(simulate_sensor_data); i++)
    {
        simulate_sensor_data[i] = simulated_data;
    }

    /* Update periodic advertisement data */
    result=wiced_ble_padv_set_adv_data(APP_BT_PERIODIC_ADV_HANDLE,
            sizeof(simulate_sensor_data), simulate_sensor_data);
    if (result != WICED_BT_SUCCESS)
    {
        printf("%s set periodic adv data result:0x%04x\n",
                __FUNCTION__, result);
    }
    else
    {
        printf("%s set periodic adv data simulated_data:%d\n",
                __FUNCTION__, simulated_data);
    }
}
#endif

/*******************************************************************************
* Function Name: app_bt_periodic_adv_init
********************************************************************************
* Summary:
*  Initialize ble periodic adv, set the necessary parameters.
*******************************************************************************/
void app_bt_periodic_adv_init(void)
{
    uint8_t i = INDEX_INIT_VAL;
    wiced_bt_dev_status_t result = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bt_dev_bda = {DEVICE_BDA_0, DEVICE_BDA_1,
            DEVICE_BDA_2,DEVICE_BDA_3, DEVICE_BDA_4, DEVICE_BDA_5};
    wiced_bt_device_address_t local_addr;
    wiced_ble_ext_adv_duration_config_t ext_cfg =
    {APP_BT_PERIODIC_ADV_HANDLE, ADV_DURATIION, MAX_EXT_ADV_EVENTS};
    for (i = INDEX_INIT_VAL; i < sizeof(simulate_sensor_data); i++)
    {
        simulate_sensor_data[i] = i;
    }
    result = wiced_bt_set_local_bdaddr(bt_dev_bda, BLE_ADDR_PUBLIC);
    if (result != WICED_BT_SUCCESS)
    {
        printf("%s set local address result:0x%04x\n",
                __FUNCTION__, result);
        handle_app_error();
    }
    wiced_bt_dev_read_local_addr(local_addr);
    printf("%s local address info:0x%02x:%02x:%02x:%02x:%02x:%02x\n",
            __FUNCTION__, local_addr[LOCAL_ADDR_INDEX_0], local_addr
            [LOCAL_ADDR_INDEX_1],
            local_addr[LOCAL_ADDR_INDEX_2], local_addr[LOCAL_ADDR_INDEX_3],
            local_addr[LOCAL_ADDR_INDEX_4], local_addr[LOCAL_ADDR_INDEX_5]);

    /* Set the Extended Adv paramameters */
    result = wiced_ble_ext_adv_set_params(APP_BT_PERIODIC_ADV_HANDLE,
            &ext_adv_param);
    if (WICED_BT_SUCCESS != result)
    {
        printf("%s set extern adv params result:0x%04x\n", __FUNCTION__, result);
        handle_app_error();
    }

    /* Set Periodic advertisement parameters */
    result = wiced_ble_padv_set_adv_params(APP_BT_PERIODIC_ADV_HANDLE,
            &padv_params);
    if (WICED_BT_SUCCESS == result)
    {
        /*Set periodic advertisment data */
        result = wiced_ble_padv_set_adv_data(APP_BT_PERIODIC_ADV_HANDLE,
                sizeof(simulate_sensor_data), simulate_sensor_data);
        if (WICED_BT_SUCCESS == result)
        {
            /*Enable periodic advertisment */
            result = wiced_ble_padv_enable_adv(APP_BT_PERIODIC_ADV_HANDLE,
                    ENABLE_PERIODIC_ADV);
            if (WICED_BT_SUCCESS != result)
            {
                printf("%s enable periodic adv result:0x%04x\n",
                        __FUNCTION__, result);
            }
        }
        else
        {
            printf("%s set periodic adv data result:0x%04x\n",
                    __FUNCTION__, result);
        }
    }
    else
    {
        printf("%s set periodic adv params result:0x%04x\n",
                __FUNCTION__, result);
    }

    /* Set extended ADV enable */
    wiced_ble_ext_adv_enable(WICED_TRUE, EXT_ADV_NUM_SETS, &ext_cfg);

#if (ENABLE_PERIODIC_CHANGE == APP_BT_PERIODIC_CHANGE_ADV_DATA)
    simulate_sensor_timer = xTimerCreate("Simulate Sensor Timer",
          SIMULATE_SENSOR_TIMEOUT_IN_MS, pdTRUE, NULL,
          app_bt_periodic_adv_generate_simulate_data_cb);
    if ((NULL != simulate_sensor_timer) &&
            (pdPASS != xTimerStart(simulate_sensor_timer, TIMER_TICKS_TO_WAIT)))
    {
        printf("Failed to start simulate_sensor_timer!\n");
        handle_app_error();
    }
#endif
}

/*  END OF FILE [] */
