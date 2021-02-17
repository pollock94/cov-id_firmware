/*
 * json_parser.c
 *
 *  Created on: Jan 26, 2020
 *      Author: Pollock
 */

//Utility Module

#include "json_parser.h"
#include "cJSON.h"




char* json_firmware_version_body_generator(char* _deviceID, char *_version) {
	cJSON *oauth_body;
	oauth_body = cJSON_CreateObject();
	cJSON_AddItemToObject(oauth_body, "terminal", cJSON_CreateString(_deviceID));
	cJSON_AddItemToObject(oauth_body, "firmware_version", cJSON_CreateString(_version));
	char *generated_oauth_body = cJSON_Print(oauth_body);
	return generated_oauth_body;
}

char* json_covid_body_generator(char* _covid, float _temp) {
	cJSON *oauth_body;
	oauth_body = cJSON_CreateObject();
	cJSON_AddItemToObject(oauth_body, "person", cJSON_CreateString(_covid));
	cJSON_AddNumberToObject(oauth_body, "temperature", _temp);
	char *generated_oauth_body = cJSON_Print(oauth_body);
	return generated_oauth_body;
}
