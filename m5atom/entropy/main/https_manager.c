/*
 * https_manager.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Pollock
 */


#include "https_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "freertos/queue.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "esp_http_client.h"
#include "esp_tls.h"



/* NOTE:
 * For OAUTH2 token to work properly, change the variable
 * DEFAULT_HTTP_BUF_SIZE (512) to DEFAULT_HTTP_BUF_SIZE (2048)
 * in "esp_http_client.h" file which can be found in
 * esp-idf\components\esp_http_client\include
 */

#define HTTPS_DEBUG

static const char *TAG = "HTTPS_MANAGER";

void https_get_bearer_token(char bearer_buff[])
{
#ifdef HTTPS_DEBUG
	    ESP_LOGI(TAG, "GETTING OAUTH BEARER TOKEN\r\n");
#endif
//		oauthCred c1;
//		c1.client_id = OAUTH_CLIENT_ID;
//		c1.client_secret = OAUTH_CLIENT_SECRET;
//		c1.grant_type = OAUTH_GRANT_TYPE;
//		c1.scope = OAUTH_SCOPE;
//		c1.username = OAUTH_USER;
//		c1.user_type = OAUTH_USER_TYPE;
//		c1.password = OAUTH_PASSWORD;
//
		char *_postdata = " ";// = json_oauth_credentials_body_generator(&c1);



	char *oauth_credentials_buffer = malloc((BEARER_BUFFER_SIZE)*sizeof(char));
	//maybe return successful or not
    esp_http_client_config_t config = {
		.host = DF_HOST,
		.path = OAUTH_PATH,
		.transport_type = HTTP_TRANSPORT_OVER_SSL,

    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err ;
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, _postdata, strlen(_postdata));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    err = esp_http_client_open(client, strlen(_postdata));
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
            esp_http_client_cleanup(client);
           //task_fatal_error(); //#TODO what does this do-> Triggers reset
        }
    	esp_http_client_write(client, _postdata, strlen(_postdata));
        esp_http_client_fetch_headers(client);

#ifdef HTTPS_DEBUG
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",  esp_http_client_get_status_code(client),    esp_http_client_get_content_length(client));
#endif
    int data_read = esp_http_client_read(client, oauth_credentials_buffer, BEARER_BUFFER_SIZE);

    if (data_read < 0)
    {
		ESP_LOGE(TAG, "Error: SSL data read error");

                //#TODO Handle Error Check for HTTPS Response 200
    }
    else if (data_read > 0)
    {
#ifdef HTTPS_DEBUG
		 ESP_LOGI(TAG, "Bearer Token Received\r\n");
#endif

    }

    //char* _bearer = json_parser_parse_data(oauth_credentials_buffer, "access_token");
    free(oauth_credentials_buffer);

    esp_http_client_close(client);
    esp_http_client_cleanup(client);

//    char *temp_bearer_token = malloc((BEARER_BUFFER_SIZE)*sizeof(char));
//    snprintf(temp_bearer_token, BEARER_BUFFER_SIZE, "%s", _bearer);
//    snprintf(bearer_buff, 8, "%s", "Bearer ");
//    for (int i = 0; i<strlen(_bearer); i++)
//    {
//	bearer_buff[i+7] = temp_bearer_token[i+1];
//    }
//    bearer_buff[strlen(_bearer)+5] = '\0'; //5 because the 7 offset. counting from 9, it is 6 and subtracting 1 becomes 5
//
//    free(temp_bearer_token);

}


/*
 *
 *
 *
static char OAUTH_USERNAME[OAUTH_TERMINAL_NAME_SIZE];
static char OAUTH_TERMINAL_PASSWORD[OAUTH_TERMINAL_PASSWORD_SIZE];
 *
void oauth_set_username(char _TERMINAL_NAME[OAUTH_TERMINAL_NAME_SIZE])
{
	memcpy(OAUTH_USERNAME, _TERMINAL_NAME, strlen(_TERMINAL_NAME)+1);

}

void oauth_set_password(char _TERMINAL_PASSWORD[OAUTH_TERMINAL_PASSWORD_SIZE])
{
	memcpy(OAUTH_TERMINAL_PASSWORD, _TERMINAL_PASSWORD, strlen(_TERMINAL_PASSWORD)+1);
}
 */
