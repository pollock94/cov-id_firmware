/*
 * json_parser.h
 *
 *  Created on: Jan 26, 2020
 *      Author: Pollock
 */

#ifndef INC_JSON_PARSER_H_
#define INC_JSON_PARSER_H_

#include <stdint.h>

typedef struct covid_params
{
  char *cov_id;
  float temperature;
} covIDParams;

char* json_covid_body_generator(char* _covid, float _temp);

#endif /* INC_JSON_PARSER_H_ */
