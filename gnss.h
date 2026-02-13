/*
 * gnss.h
 *
 *  Created on: Feb 11, 2026
 *      Author: Yoann Hervagault
 */

#ifndef GNSS_GNSS_H_
#define GNSS_GNSS_H_

#include "gnss_cfg.h"
#include "resources/minmea.h"

typedef enum {
	GNSS_OK, GNSS_PARAMETER_ERROR, GNSS_INTERNAL_ERROR, GNSS_RESULTNB
} GNSS_Result;

GNSS_Result gnss_init(UART_HandleTypeDef* pSelectUart);

#endif /* GNSS_GNSS_H_ */
