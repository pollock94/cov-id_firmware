/*
 * wifi_driver.c
 *
 *  Created on: 12 Mar 2019
 *      Author: Sadequr Rahman Rabby
 */
#include "wifi_driver.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_log.h"
#include "tcpip_adapter.h"
#include "custom_list.h"
#include "MessageQueue.h"
#include "https_manager.h"

#define WIFI_DISCONNET_EVENT_BIT        ( 1UL << 8UL)
#define WIFI_CONNET_EVENT_BIT           ( 1UL << 7UL)

static const char *TAG = "wifi";

static list_t *list = NULL;
static TaskHandle_t xWifiEventTaskHandler = NULL;
static bool _isConnected = false;
static void wifi_event_task(void *param);
//static void vProcessConnectEvent(void);
//static void vProcessDisconnectEvent(void);

static void event_handler(void *handler_arg, esp_event_base_t base, int32_t id,
		void *event_data) {
	switch (id) {
	case WIFI_EVENT_STA_START:
		esp_wifi_connect();
		break;
	case WIFI_EVENT_STA_CONNECTED:
		_isConnected = true;
		break;
	case IP_EVENT_STA_GOT_IP:
		xTaskNotify( xWifiEventTaskHandler, WIFI_CONNET_EVENT_BIT , eSetBits );
		break;
	case WIFI_EVENT_STA_DISCONNECTED:
		_isConnected = false;
		xTaskNotify( xWifiEventTaskHandler, WIFI_DISCONNET_EVENT_BIT , eSetBits );
		esp_wifi_connect();
		break;
	default:
		break;
	}
}

int wifi_get_rssi()
{
  wifi_ap_record_t ap;
  esp_wifi_sta_get_ap_info(&ap);
  return ap.rssi;
}

static void wifi_event_callback(void *msg)
{
	int wifi_stat = *((int*)msg);

  ESP_LOGI(TAG, "ID received %d", wifi_stat);

  if(wifi_stat == 128)
  {
	  char bearer_buff[100];
	  https_get_bearer_token(bearer_buff);
  }
}

static void wifi_restart()
{
  ESP_ERROR_CHECK(esp_wifi_disconnect());
  ESP_ERROR_CHECK(esp_wifi_connect());
}
static void wifiResetCallback(void *msg)
{
  //if(*((int*)msg ==0)
  if (wifi_isConnected() == true)
    {
      wifi_restart();
    }
}

void wifiDriver_init(void) {
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL ));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, event_handler, NULL ));
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT()
	;
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	wifi_config_t wifi_config = { .sta = {
			.ssid = WIFI_SSID,
			.password = WIFI_PASSWORD,
		},
	};
	//list = list_new();
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());
	xTaskCreate(wifi_event_task, "wifiTask", 1024*4, NULL, configMAX_PRIORITIES-4, &xWifiEventTaskHandler);
	MessageQueue_RegisterMsg (wifi, wifi_event_callback);
//	MessageQueue_RegisterMsg(timer, wifiResetCallback);
//	MessageQueue_RegisterMsg(mqtt_error, wifiResetCallback);



}

//
//void wifiDriver_addListener(wifiCallback_t callback) {
//	list_node_t *node = list_node_new((void*) callback);
//	list_lpush(list, node);
//}

uint8_t wifi_isConnected(void) {
	return _isConnected;
}

static void wifi_event_task(void *param) {
	uint32_t ulNotifiedValue;
	while(1){
	    	vTaskDelay(50 / portTICK_PERIOD_MS);
		xTaskNotifyWait( ULONG_MAX, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY );
		if( (ulNotifiedValue & WIFI_CONNET_EVENT_BIT) ==  WIFI_CONNET_EVENT_BIT ){
			//vProcessConnectEvent();
			if (MessageQueue_IsValid()) {
				msg_t *m = (msg_t*) heap_caps_malloc(sizeof(msg_t), MALLOC_CAP_DEFAULT);
				m->src = wifi;
				m->msg = (void*) &ulNotifiedValue;
				MessageQueue_Send(m);
				heap_caps_free(m);
			}

		}
		if( (ulNotifiedValue & WIFI_DISCONNET_EVENT_BIT) ==  WIFI_DISCONNET_EVENT_BIT ){

			if (MessageQueue_IsValid()) {
				msg_t *m = (msg_t*) heap_caps_malloc(sizeof(msg_t), MALLOC_CAP_DEFAULT);
				m->src = wifi;
				m->msg = (void*) &ulNotifiedValue;
				MessageQueue_Send(m);
				heap_caps_free(m);
			}
		}
	}
}

//void vProcessConnectEvent(void){
//	list_iterator_t *it = list_iterator_new(list, LIST_HEAD);
//	list_node_t *eL = list_iterator_next(it);
//	wifiCallback_t callback;
//	while (eL) {
//		callback = (wifiCallback_t) eL->val;
//		callback(WIFI_CONNECTED);
//		eL = list_iterator_next(it);
//	}
//	list_iterator_destroy(it);
//}
//
//void vProcessDisconnectEvent(void){
//		list_iterator_t *it = list_iterator_new(list, LIST_HEAD);
//		list_node_t *eL = list_iterator_next(it);
//		wifiCallback_t callback;
//		while (eL) {
//			callback = (wifiCallback_t) eL->val;
//			callback(WIFI_DISCONNECTED);
//			eL = list_iterator_next(it);
//		}
//		list_iterator_destroy(it);
//}
