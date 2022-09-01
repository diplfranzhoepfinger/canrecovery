/* Hello World Example

*/
#include <stdio.h>
#include "gpio_abstraction.h"
#include "statusLED.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/twai.h"

extern void canbus_main(void);


static const char * const TAG = "main";

void app_main(void)
{
	ESP_LOGI(TAG, "Hello world!");

	canbus_main();

    gpio_init_DI1_DI2();

    vTaskDelay(pdMS_TO_TICKS(100)); //Wait until Debouncing is done. Depends on sdkconfig.




	ESP_LOGI(TAG, "gpio_get_level_DI1 ************* = %i", gpio_get_level_DI1());
	ESP_LOGI(TAG, "gpio_get_level_DI2 ************* = %i", gpio_get_level_DI2());


    twai_message_t tx_msg = {.data_length_code = 4, .identifier = 0x100, };
    tx_msg.data[0] = 0xFF;
	while(1)
	{
	    vTaskDelay(pdMS_TO_TICKS(100));

        tx_msg.data[0]++; //msg Counter
	    tx_msg.data[1] = gpio_get_level_DI1();
	    tx_msg.data[2] = gpio_get_level_DI2();
        tx_msg.data[3] = 0xFF;



        esp_err_t err = twai_transmit(&tx_msg, portMAX_DELAY);
        if (err != ESP_OK)
        {
          ESP_LOGE(TAG, "twai_transmit failed, %i %s", err, esp_err_to_name(err));
        }

	}



}
