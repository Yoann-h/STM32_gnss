/*
 * gnss.c
 *
 *  Created on: Feb 11, 2026
 *      Author: Yoann Hervagault
 */

#include "gnss.h"

UART_HandleTypeDef* sgnss_uart;

GNSS_Result gnss_init(UART_HandleTypeDef* pSelectUart)
{
	GNSS_Result eResult = GNSS_OK;
	sgnss_uart = pSelectUart;
	return eResult;
}
