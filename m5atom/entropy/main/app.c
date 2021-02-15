



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "ble_manager.h"
#include "sensor_manager.h"
#include "display_driver.h"
#include "MessageQueue.h"
#include "esp_flash_encrypt.h"
#include <stdlib.h>
#include <string.h>


static const char *TAG = __FILE__;

static void sensor_data_callback(void *msg) {
	ESP_LOGI(TAG, "FLOAT Val : %lf\r\n", *((float* )msg));

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

        esp_flash_enc_mode_t mode = esp_get_flash_encryption_mode();
        if (mode == ESP_FLASH_ENC_MODE_DISABLED) {
        	ESP_LOGI(TAG,"Flash encryption feature is disabled\n");
        } else {
        	ESP_LOGI(TAG,"Flash encryption feature is enabled in %s mode\n",
                mode == ESP_FLASH_ENC_MODE_DEVELOPMENT ? "DEVELOPMENT" : "RELEASE");
        }

	MessageQueue_Init(MSG_QUEUE_LEN);
	MessageQueue_RegisterMsg(sensor, sensor_data_callback);
  sensor_manager_init(); //maybe should be after BLE


}
