/* TWAI Network Master Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"

/* --------------------- Definitions and static variables ------------------ */
//Example Configuration
#define TX_GPIO_NUM             CONFIG_EXAMPLE_TX_GPIO_NUM
#define RX_GPIO_NUM             CONFIG_EXAMPLE_RX_GPIO_NUM
#define CTRL_TASK_PRIO          10
#define EXAMPLE_TAG             "TWAI Master"





static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
static twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);





/* --------------------------- Tasks and Functions -------------------------- */



union bitsplit
{
 uint8_t b[4];
 uint32_t u;
};


// CONFIG_TWAI_ISR_IN_IRAM must NOT be defined !

void start() {
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(EXAMPLE_TAG, "Driver started");
    ESP_LOGI(EXAMPLE_TAG, "Starting transmissions");
    //Prepare to trigger errors, reconfigure alerts to detect change in error state
    twai_reconfigure_alerts( TWAI_ALERT_ALL , NULL); //all Alerts.
}

static void ctrl_task(void *arg)
{
    start();
    twai_message_t tx_msg = {.data_length_code = 5, .identifier = 0x300, };
    twai_message_t tx_cnt1_msg = {.data_length_code = 8, .identifier = 0x301, };
    twai_message_t tx_cnt2_msg = {.data_length_code = 8, .identifier = 0x302, };
    twai_message_t tx_cnt3_msg = {.data_length_code = 8, .identifier = 0x303, };
    twai_message_t tx_stat1_msg = {.data_length_code = 8, .identifier = 0x401, };
    twai_message_t tx_stat2_msg = {.data_length_code = 8, .identifier = 0x402, };
    twai_message_t tx_stat3_msg = {.data_length_code = 8, .identifier = 0x403, };
    twai_message_t tx_stat4_msg = {.data_length_code = 8, .identifier = 0x404, };
    twai_message_t tx_stat5_msg = {.data_length_code = 8, .identifier = 0x405, };
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(100));
        union bitsplit alerts;
        twai_read_alerts(&alerts.u, portMAX_DELAY);
        tx_msg.data[0]++; //msg Counter
        tx_msg.data[1] = alerts.b[0];
        tx_msg.data[2] = alerts.b[1];
        tx_msg.data[3] = alerts.b[2];
        tx_msg.data[4] = alerts.b[3];


        tx_cnt1_msg.data[0]++; //msg Counter
        tx_cnt2_msg.data[0]++; //msg Counter
        tx_cnt3_msg.data[0]++; //msg Counter

        if (alerts.u & TWAI_ALERT_TX_IDLE) {
            tx_cnt1_msg.data[1]++;
        }
        if (alerts.u & TWAI_ALERT_TX_SUCCESS) {
            tx_cnt1_msg.data[2]++;
        }
        if (alerts.u & TWAI_ALERT_RX_DATA) {
            tx_cnt1_msg.data[3]++;
        }
        if (alerts.u & TWAI_ALERT_BELOW_ERR_WARN) {
            tx_cnt1_msg.data[4]++;
        }
        if (alerts.u & TWAI_ALERT_ERR_ACTIVE) {
            tx_cnt1_msg.data[5]++;
        }
        if (alerts.u & TWAI_ALERT_RECOVERY_IN_PROGRESS) {
            tx_cnt1_msg.data[6]++;
        }
        if (alerts.u & TWAI_ALERT_BUS_RECOVERED) {
            tx_cnt1_msg.data[7]++;
        }

        if (alerts.u & TWAI_ALERT_ARB_LOST) {
            tx_cnt2_msg.data[1]++;
        }
        if (alerts.u & TWAI_ALERT_ABOVE_ERR_WARN) {
            tx_cnt2_msg.data[2]++;
        }
        if (alerts.u & TWAI_ALERT_BUS_ERROR) {
            tx_cnt2_msg.data[3]++;
        }
        if (alerts.u & TWAI_ALERT_TX_FAILED) {
            tx_cnt2_msg.data[4]++;
        }
        if (alerts.u & TWAI_ALERT_RX_QUEUE_FULL) {
            tx_cnt2_msg.data[5]++;
        }
        if (alerts.u & TWAI_ALERT_ERR_PASS) {
            tx_cnt2_msg.data[6]++;
        }
        if (alerts.u & TWAI_ALERT_BUS_OFF) {
            tx_cnt2_msg.data[7]++;
        }

        if (alerts.u & TWAI_ALERT_RX_FIFO_OVERRUN) {
            tx_cnt3_msg.data[1]++;
        }
        if (alerts.u & TWAI_ALERT_TX_RETRIED) {
            tx_cnt3_msg.data[2]++;
        }
        if (alerts.u & TWAI_ALERT_PERIPH_RESET) {
            tx_cnt3_msg.data[3]++;
        }


        //Wait for bus to become free
        twai_status_info_t status_info;
        twai_get_status_info(&status_info);

        union bitsplit t;

        t.u = status_info.msgs_to_tx;
        tx_stat1_msg.data[0] = t.b[0];
        tx_stat1_msg.data[1] = t.b[1];
        tx_stat1_msg.data[2] = t.b[2];
        tx_stat1_msg.data[3] = t.b[3];

        t.u = status_info.msgs_to_rx;
        tx_stat1_msg.data[4] = t.b[0];
        tx_stat1_msg.data[5] = t.b[1];
        tx_stat1_msg.data[6] = t.b[2];
        tx_stat1_msg.data[7] = t.b[3];


        t.u = status_info.tx_error_counter;
        tx_stat2_msg.data[0] = t.b[0];
        tx_stat2_msg.data[1] = t.b[1];
        tx_stat2_msg.data[2] = t.b[2];
        tx_stat2_msg.data[3] = t.b[3];

        t.u = status_info.rx_error_counter;
        tx_stat2_msg.data[4] = t.b[0];
        tx_stat2_msg.data[5] = t.b[1];
        tx_stat2_msg.data[6] = t.b[2];
        tx_stat2_msg.data[7] = t.b[3];


        t.u = status_info.tx_failed_count;
        tx_stat3_msg.data[0] = t.b[0];
        tx_stat3_msg.data[1] = t.b[1];
        tx_stat3_msg.data[2] = t.b[2];
        tx_stat3_msg.data[3] = t.b[3];

        t.u = status_info.rx_missed_count;
        tx_stat3_msg.data[4] = t.b[0];
        tx_stat3_msg.data[5] = t.b[1];
        tx_stat3_msg.data[6] = t.b[2];
        tx_stat3_msg.data[7] = t.b[3];


        t.u = status_info.rx_overrun_count;
        tx_stat4_msg.data[0] = t.b[0];
        tx_stat4_msg.data[1] = t.b[1];
        tx_stat4_msg.data[2] = t.b[2];
        tx_stat4_msg.data[3] = t.b[3];

        t.u = status_info.arb_lost_count;
        tx_stat4_msg.data[4] = t.b[0];
        tx_stat4_msg.data[5] = t.b[1];
        tx_stat4_msg.data[6] = t.b[2];
        tx_stat4_msg.data[7] = t.b[3];

        t.u = status_info.bus_error_count;
        tx_stat5_msg.data[0] = t.b[0];
        tx_stat5_msg.data[1] = t.b[1];
        tx_stat5_msg.data[2] = t.b[2];
        tx_stat5_msg.data[3] = t.b[3];

        tx_stat5_msg.data[4]++; //msg Counter
        tx_stat5_msg.data[5] = status_info.state;



        if (status_info.state == TWAI_STATE_RUNNING)
        {
            //Send Diagnosic Messages.
            twai_transmit(&tx_msg, 0);
            twai_transmit(&tx_cnt1_msg, 0);
            twai_transmit(&tx_cnt2_msg, 0);
            twai_transmit(&tx_cnt3_msg, 0);
            twai_transmit(&tx_stat1_msg, 0);
            twai_transmit(&tx_stat2_msg, 0);
            twai_transmit(&tx_stat3_msg, 0);
            twai_transmit(&tx_stat4_msg, 0);
            twai_transmit(&tx_stat5_msg, 0);
        }


        if (alerts.u & TWAI_ALERT_ABOVE_ERR_WARN) {
            ESP_LOGI(EXAMPLE_TAG, "Surpassed Error Warning Limit");
        }
        if (alerts.u & TWAI_ALERT_ERR_PASS) {
            ESP_LOGI(EXAMPLE_TAG, "Entered Error Passive state");
        }
        if (alerts.u & TWAI_ALERT_BUS_OFF) {
            ESP_LOGI(EXAMPLE_TAG, "Bus Off state");
            //Prepare to initiate bus recovery, reconfigure alerts to detect bus recovery completion
            twai_reconfigure_alerts(TWAI_ALERT_BUS_RECOVERED, NULL);
            ESP_LOGW(EXAMPLE_TAG, "Initiate bus recovery in 50ms");
            vTaskDelay(pdMS_TO_TICKS(10));
            ESP_LOGW(EXAMPLE_TAG, "Initiate bus recovery in 40ms");
            vTaskDelay(pdMS_TO_TICKS(10));
            ESP_LOGW(EXAMPLE_TAG, "Initiate bus recovery in 30ms");
            vTaskDelay(pdMS_TO_TICKS(10));
            ESP_LOGW(EXAMPLE_TAG, "Initiate bus recovery in 20ms");
            vTaskDelay(pdMS_TO_TICKS(10));
            ESP_LOGW(EXAMPLE_TAG, "Initiate bus recovery in 10ms");
            vTaskDelay(pdMS_TO_TICKS(10));
            twai_initiate_recovery();    //Needs 128 occurrences of bus free signal
            ESP_LOGI(EXAMPLE_TAG, "Initiate bus recovery");
        }
        if (alerts.u & TWAI_ALERT_BUS_RECOVERED) {
            //Bus recovery was successful, exit control task to uninstall driver
            ESP_LOGI(EXAMPLE_TAG, "Bus Recovered");
            start();
        }
    }
}

void canbus_main(void)
{

#if defined(CONFIG_TWAI_ISR_IN_IRAM)
    g_config.intr_flags = ESP_INTR_FLAG_IRAM;
#endif

    g_config.tx_queue_len = 20;

    //Install TWAI driver
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(EXAMPLE_TAG, "Driver installed");

    xTaskCreatePinnedToCore(ctrl_task, "TWAI_ctrl", 4096, NULL, CTRL_TASK_PRIO, NULL, tskNO_AFFINITY);


}
