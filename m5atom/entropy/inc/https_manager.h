/*
 * https_manager.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Pollock
 */

#ifndef INC_HTTPS_MANAGER_H_
#define INC_HTTPS_MANAGER_H_

#include <string.h>
#include <stdlib.h>

#define BEARER_BUFFER_SIZE			2048
#define DF_HOST                     "cov-id.dfnest.com"//CONFIG_DF_HOST
#define OAUTH_USER					//CONFIG_OAUTH_USER
#define OAUTH_PASSWORD				//CONFIG_OAUTH_PASSWORD
#define OAUTH_CLIENT_ID 			//CONFIG_OAUTH_CLIENT_ID
#define OAUTH_CLIENT_SECRET 			//CONFIG_OAUTH_SECRET
#define OAUTH_GRANT_TYPE 			//CONFIG_OAUTH_GRANT_TYPE
#define OAUTH_SCOPE 				//CONFIG_OAUTH_SCOPE
#define OAUTH_USER_TYPE 			//CONFIG_OAUTH_USER_TYPE
#define OAUTH_PATH 					"/api/v1/readings"//CONFIG_OAUTH_PATH

void https_get_bearer_token(char bearer_buff[]);
void https_get_request(char* _url, char* _oauth_bearer_token); //#TODO HTTPS_MANAGER_POST_REQ
void https_save_bearer_token(); // #TODO HTTP_MANAGER_INIT/ CHANGE WHOLE MODULE NAME

#endif /* INC_HTTPS_MANAGER_H_ */
