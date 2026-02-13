/*
 * gnss.h
 *
 *  Created on: Feb 11, 2026
 *      Author: Yoann Hervagault
 */

#ifndef GNSS_GNSS_H_
#define GNSS_GNSS_H_

#include "gnss_cfg.h"
#include "resources/minmea/minmea.h"

typedef struct
{
	UART_HandleTypeDef* sgnss_uart;
	uint8_t u8gnssBuffer[GNSSBufferSize];
	double dLatitude;
	double dLongitude;
	float fSpeed;
	float fAltitude;
	float fCog;
	float fpdop;
	float fhdop;
	float fvdop;
	uint32_t u32Date;
	uint8_t u8TimeHour;
	uint8_t u8TimeMin;
	uint8_t u8TimeSec;
	uint32_t u32TimeMicro;
	uint8_t u8Fix;
	uint8_t u8SatInView;
	uint8_t u8SatUsed;
}GNSS_Handler;

typedef enum {
	GNSS_OK, GNSS_PARAMETER_ERROR, GNSS_INTERNAL_ERROR, GNSS_RESULTNB
} GNSS_Result;

GNSS_Result gnss_init(GNSS_Handler* fhandler, UART_HandleTypeDef* pSelectUart);

#endif /* GNSS_GNSS_H_ */
