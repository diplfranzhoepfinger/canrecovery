

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/***************** Inclusions **************************/
/*******************************************************/
#include "gpio_abstraction.h"
#include "esp_log.h"
#include "iot_button.h"
#include "driver/twai.h"

/*******************************************************/
/***************** Local Macros ************************/
/*******************************************************/

static const uint8_t active_level = 0;


/*******************************************************/
/***************** Local types *************************/
/*******************************************************/


/*******************************************************/
/***************** Local variables *********************/
/*******************************************************/
static button_handle_t g_btn_DI1 = 0;
static button_handle_t g_btn_DI2 = 0;
static int gpio_level_DI1 = 0;
static int gpio_level_DI2 = 0;

/*******************************************************/
/***************** Local const *********************/
/*******************************************************/
static const gpio_num_t GPIO_INPUT_DI1               = GPIO_NUM_26;
static const gpio_num_t GPIO_INPUT_DI2               = GPIO_NUM_32;

static const char * const TAG = "gpio_abstraction";

/*******************************************************/
/************* Local function declarations *************/
/*******************************************************/

static void button_press_cb_DI1(void *hardware_data, void *usr_data)
{
    switch(iot_button_get_event((button_handle_t)hardware_data))
    {
        case BUTTON_PRESS_DOWN:
            ESP_LOGI(TAG, "DI1: BUTTON_PRESS_DOWN");
            gpio_level_DI1 = 1;
            break;
        default:
            ESP_LOGI(TAG, "DI1: BUTTON_PRESS_UP");
            gpio_level_DI1 = 0;
            break;
    }
}


static void button_press_cb_DI2(void *hardware_data, void *usr_data)
{
    switch(iot_button_get_event((button_handle_t)hardware_data))
    {
        case BUTTON_PRESS_DOWN:
            ESP_LOGI(TAG, "DI2: BUTTON_PRESS_DOWN");
            gpio_level_DI2 = 1;
            break;
        default:
            ESP_LOGI(TAG, "DI2: BUTTON_PRESS_UP");
            gpio_level_DI2 = 0;
            break;
    }
}


static void button_press_down_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_PRESS_DOWN, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_PRESS_DOWN;
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_PRESS_DOWN");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_press_up_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_PRESS_UP, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_PRESS_UP;
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_PRESS_UP");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_press_repeat_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_PRESS_REPEAT, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_PRESS_REPEAT;
    tx_msg.data[2] = iot_button_get_repeat((button_handle_t )hardware_data);
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_PRESS_REPEAT[%d]", tx_msg.data[2]);
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_single_click_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_SINGLE_CLICK, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_SINGLE_CLICK;
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_SINGLE_CLICK");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_double_click_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_DOUBLE_CLICK, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_DOUBLE_CLICK;
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_DOUBLE_CLICK");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_long_press_start_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_LONG_PRESS_START, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_LONG_PRESS_START;
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_LONG_PRESS_START");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_long_press_hold_cb_DI1(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x100 + BUTTON_LONG_PRESS_HOLD, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_LONG_PRESS_HOLD;
    ESP_LOGI(TAG, "BTN_DI1: BUTTON_LONG_PRESS_HOLD");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_press_down_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_PRESS_DOWN, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_PRESS_DOWN;
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_PRESS_DOWN");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_press_up_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_PRESS_UP, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_PRESS_UP;
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_PRESS_UP");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_press_repeat_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_PRESS_REPEAT, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_PRESS_REPEAT;
    tx_msg.data[2] = iot_button_get_repeat((button_handle_t )hardware_data);
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_PRESS_REPEAT[%d]", tx_msg.data[2]);
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_single_click_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_SINGLE_CLICK, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_SINGLE_CLICK;
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_SINGLE_CLICK");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_double_click_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_DOUBLE_CLICK, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_DOUBLE_CLICK;
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_DOUBLE_CLICK");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_long_press_start_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_LONG_PRESS_START, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_LONG_PRESS_START;
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_LONG_PRESS_START");
    twai_transmit(&tx_msg, portMAX_DELAY);
}

static void button_long_press_hold_cb_DI2(void *hardware_data, void *usr_data) {
    static twai_message_t tx_msg = { .data_length_code = 4, .identifier = 0x200 + BUTTON_LONG_PRESS_HOLD, };
    tx_msg.data[0]++; //msg Counter
    tx_msg.data[1] = BUTTON_LONG_PRESS_HOLD;
    ESP_LOGI(TAG, "BTN_DI2: BUTTON_LONG_PRESS_HOLD");
    twai_transmit(&tx_msg, portMAX_DELAY);
}



/*******************************************************/
/***************** Interface functions *****************/
/*******************************************************/



esp_err_t gpio_init_DI1_DI2(void)
{
    printf("Button Example!\n");

    button_config_t cfg_DI1 = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = GPIO_INPUT_DI1,
            .active_level = active_level,
        },
    };
    button_config_t cfg_DI2 = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = GPIO_INPUT_DI2,
            .active_level = active_level,
        },
    };
    gpio_level_DI1 = 0;
    gpio_level_DI2 = 0;
    g_btn_DI1 = iot_button_create(&cfg_DI1);
    g_btn_DI2 = iot_button_create(&cfg_DI2);
    iot_button_register_cb(g_btn_DI1, BUTTON_PRESS_NORMAL,     button_press_cb_DI1,            0);
    iot_button_register_cb(g_btn_DI1, BUTTON_PRESS_DOWN,       button_press_down_cb_DI1,       0);
    iot_button_register_cb(g_btn_DI1, BUTTON_PRESS_UP,         button_press_up_cb_DI1,         0);
    iot_button_register_cb(g_btn_DI1, BUTTON_PRESS_REPEAT,     button_press_repeat_cb_DI1,     0);
    iot_button_register_cb(g_btn_DI1, BUTTON_SINGLE_CLICK,     button_single_click_cb_DI1,     0);
    iot_button_register_cb(g_btn_DI1, BUTTON_DOUBLE_CLICK,     button_double_click_cb_DI1,     0);
    iot_button_register_cb(g_btn_DI1, BUTTON_LONG_PRESS_START, button_long_press_start_cb_DI1, 0);
    iot_button_register_cb(g_btn_DI1, BUTTON_LONG_PRESS_HOLD,  button_long_press_hold_cb_DI1,  0);

    iot_button_register_cb(g_btn_DI2, BUTTON_PRESS_NORMAL,     button_press_cb_DI2,            0);
    iot_button_register_cb(g_btn_DI2, BUTTON_PRESS_DOWN,       button_press_down_cb_DI2,       0);
    iot_button_register_cb(g_btn_DI2, BUTTON_PRESS_UP,         button_press_up_cb_DI2,         0);
    iot_button_register_cb(g_btn_DI2, BUTTON_PRESS_REPEAT,     button_press_repeat_cb_DI2,     0);
    iot_button_register_cb(g_btn_DI2, BUTTON_SINGLE_CLICK,     button_single_click_cb_DI2,     0);
    iot_button_register_cb(g_btn_DI2, BUTTON_DOUBLE_CLICK,     button_double_click_cb_DI2,     0);
    iot_button_register_cb(g_btn_DI2, BUTTON_LONG_PRESS_START, button_long_press_start_cb_DI2, 0);
    iot_button_register_cb(g_btn_DI2, BUTTON_LONG_PRESS_HOLD,  button_long_press_hold_cb_DI2,  0);


    return ESP_OK;
}


esp_err_t gpio_delete_DI1_DI2(void)
{
    iot_button_delete(g_btn_DI1);
    iot_button_delete(g_btn_DI2);
    g_btn_DI1 = NULL;
    g_btn_DI1 = NULL;
    gpio_level_DI1 = 0;
    gpio_level_DI2 = 0;
    return ESP_OK;
}




int gpio_get_level_DI1(void)
{
    return gpio_level_DI1;
}

int gpio_get_level_DI2(void)
{
    return gpio_level_DI2;
}

/*******************************************************/
/************* Local function definitions **************/
/*******************************************************/

#ifdef __cplusplus
} /* end of extern "C" */
#endif
