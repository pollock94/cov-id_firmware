/*
 * uart_driver.c
 *
 *  Created on: Nov 30, 2019
 *      Author: Pollock
 */

#include "uart_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "MessageQueue.h"


#define UART_DEBUG

static const char *TAG = "uart_driver";

static QueueHandle_t main_uart_queue;

int uart_transmit(uint8_t data[], size_t _len)
{
  const int txBytes = uart_write_bytes(MAIN_UART, (const char*)data, _len);

#ifdef UART_DEBUG
    ESP_LOGI(TAG, "Wrote %d bytes", txBytes);
#endif
    return txBytes;
}


static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(main_uart_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
#ifdef UART_DEBUG
            ESP_LOGI(TAG, "uart[%d] event, size: %d", MAIN_UART, event.size);
#endif
            switch(event.type) {
                case UART_DATA:
                    uart_read_bytes(MAIN_UART, dtmp, event.size, portMAX_DELAY);
                    uart_write_bytes(MAIN_UART, (const char*) dtmp, event.size);
#ifdef UART_DEBUG
                    ESP_LOGI(TAG, "DTMP DATA: %s\r\n", dtmp);


#endif

				if (event.size == 7) //Cash signal comes as 2 bytes
				{
					char COVID[10];
					sprintf(COVID, "%s", dtmp);
					if (MessageQueue_IsValid()) {
						msg_t *m = (msg_t*) heap_caps_malloc(sizeof(msg_t),	MALLOC_CAP_DEFAULT);
						m->src = uart;
						m->msg = (void*) &COVID;
						MessageQueue_Send(m);
						heap_caps_free(m);
					}

				}
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGE(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(MAIN_UART);
                    xQueueReset(main_uart_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGE(TAG, "ring buffer full");
                    // If buffer full happened, you should consider increasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(MAIN_UART);
                    xQueueReset(main_uart_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGE(TAG, "uart rx break");
                    uart_flush_input(MAIN_UART);
                    xQueueReset(main_uart_queue);
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGE(TAG, "uart parity error");
                    uart_flush_input(MAIN_UART);
                    xQueueReset(main_uart_queue);
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGE(TAG, "uart frame error");
                    uart_flush_input(MAIN_UART);
                    xQueueReset(main_uart_queue);
                    break;
                default:
#ifdef UART_DEBUG
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
#endif
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

void uart_main_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(MAIN_UART, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(MAIN_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    // We won't use a buffer for sending data.
    ESP_ERROR_CHECK(uart_driver_install(MAIN_UART, RX_BUF_SIZE, 0, 20, &main_uart_queue, 0));
    xTaskCreate(uart_event_task, "uart_event_task", 1024*4, NULL, configMAX_PRIORITIES-1, NULL); //#TODO Review the Priorities
}
