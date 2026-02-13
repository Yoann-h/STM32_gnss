/*
 * gnss.c
 *
 *  Created on: Feb 11, 2026
 *      Author: Yoann Hervagault
 */

#include "gnss.h"

UART_HandleTypeDef* sgnss_uart;
uint8_t u8gnssBuffer[GNSSBufferSize]={0};
double dLatitude;
double dLongitude;
double dSpeed;
float fAltitude;
float fCog;
float fpdop;
float fhdop;
float fvdop;
uint32_t u32Date;
uint32_t u32Time;
uint8_t u8Fix;
uint8_t u8SatInView;
uint8_t u8SatUsed;

GNSS_Result gnss_parse(uint32_t size);

GNSS_Result gnss_init(UART_HandleTypeDef* pSelectUart)
{
	GNSS_Result eResult = GNSS_OK;
	sgnss_uart = pSelectUart;
	HAL_UARTEx_ReceiveToIdle_DMA(sgnss_uart, u8gnssBuffer, BufferSize);
	return eResult;
}

GNSS_Result gnss_parse(uint32_t size)
{
	GNSS_Result eResult = GNSS_OK;
	enum minmea_sentence_id frametype = minmea_sentence_id(u8gnssBuffer, true);
	if (frametype != MINMEA_UNKNOWN)
	{
		switch (frametype) {
			case MINMEA_SENTENCE_RMC: {
				struct minmea_sentence_rmc frame;
				if (minmea_parse_rmc(&frame, u8gnssBuffer)) {

				}
			} break;
		    case MINMEA_SENTENCE_GGA: {
		    	struct minmea_sentence_gga frame;
		        if (minmea_parse_gga(&frame, u8gnssBuffer)) {
		        }
		    } break;
		    case MINMEA_SENTENCE_GST: {
		    	struct minmea_sentence_gst frame;
		        if (minmea_parse_gst(&frame, u8gnssBuffer)) {
		        }
		    } break;
		    case MINMEA_SENTENCE_GSV: {
		    	struct minmea_sentence_gsv frame;
		        if (minmea_parse_gsv(&frame, lu8gnssBufferine)) {
		        }
		    } break;
		    case MINMEA_SENTENCE_VTG: {
		    	struct minmea_sentence_vtg frame;
		        if (minmea_parse_vtg(&frame, u8gnssBuffer)) {
		        }
		    } break;
		    case MINMEA_SENTENCE_ZDA: {
		    	struct minmea_sentence_zda frame;
		        if (minmea_parse_zda(&frame, u8gnssBuffer)) {

		        }
		    } break;
		    case MINMEA_INVALID: {
		    	eResult =GNSS_INTERNAL_ERROR
		    } break;
		    default: {
		    	eResult =GNSS_INTERNAL_ERROR
		    } break;
		}
	}
	return eResult;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if(huart->Instance==sgnss_uart->Instance)
	{

		//HAL_UART_Transmit(&huart2, uart_rx_data, Size, 100);
		gnss_parse(size);
		HAL_UARTEx_ReceiveToIdle_DMA(sgnss_uart, uart_rx_data, BufferSize);
	}

}
