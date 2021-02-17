



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sensor_manager.h"
#include "display_driver.h"
#include "MessageQueue.h"
#include "uart_driver.h"
#include "esp_flash_encrypt.h"
#include "wifi_driver.h"
#include "led.h"
#include <stdlib.h>
#include <string.h>
#include "json_parser.h"
#include "https_manager.h"

static const char *TAG = __FILE__;

static void uart_data_callback(void *msg) {
	ESP_LOGI(TAG, "UART Data : %s\r\n", (char *)msg);
	https_post_data(json_covid_body_generator((char *)msg , sensor_manager_get_temperature()));


}

void app_main()
{
    esp_err_t ret;

    /* Initialize NVS. */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK( ret );
    //startup logs
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());


	MessageQueue_Init(MSG_QUEUE_LEN);
	uart_main_init();
	wifiDriver_init();
	MessageQueue_RegisterMsg(uart, uart_data_callback);
	sensor_manager_init();




}
