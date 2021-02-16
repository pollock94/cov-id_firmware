/*
 * wifi_driver.h
 *
 *  Created on: 12 Mar 2019
 *      Author: Sadequr Rahman Rabby
 */

#ifndef __WIFIDRIVER_H__
#define __WIFIDRIVER_H__

#include "stdlib.h"
#include "stdint.h"

#define WIFI_SSID						"CucianHebat"//CONFIG_WIFI_SSID
#define WIFI_PASSWORD					"dobiqueen109"//CONFIG_WIFI_PASSWORD

typedef enum{
	WIFI_CONNECTED,
	WIFI_DISCONNECTED
}wifi_driver_event_t;

typedef void(*wifiCallback_t)(wifi_driver_event_t);

void wifiDriver_init(void);
int wifi_get_rssi(void);
void wifiDriver_addListener(wifiCallback_t callback);
uint8_t wifi_isConnected(void);




#endif /* MAIN_WIFIMANAGER_H_ */
