/*
 * gnss.c
 *
 *  Created on: Feb 11, 2026
 *      Author: Yoann Hervagault
 */

#include "gnss.h"

GNSS_Handler GNSS_Handle;

GNSS_Result gnss_parse(GNSS_Handler* fhandler, uint32_t size);

GNSS_Result gnss_init(GNSS_Handler* fhandler, UART_HandleTypeDef* pSelectUart)
{
	GNSS_Result eResult = GNSS_OK;
	if(fhandler!=0 && pSelectUart!=0)
	{
		fhandler->sgnss_uart = pSelectUart;
		memset(fhandler->u8gnssBuffer, 0, GNSSBufferSize);
		fhandler->dLatitude=0;
		fhandler->dLongitude=0;
		fhandler->fSpeed=0;
		fhandler->fAltitude=0;
		fhandler->fCog=0;
		fhandler->fpdop=0;
		fhandler->fhdop=0;
		fhandler->fvdop=0;
		fhandler->u32Date=0;
		fhandler->u8TimeHour=0;
		fhandler->u8TimeMin=0;
		fhandler->u8TimeSec=0;
		fhandler->u32TimeMicro=0;
		fhandler->u8Fix=0;
		fhandler->u8SatInView=0;
		fhandler->u8SatUsed=0;
		HAL_UARTEx_ReceiveToIdle_DMA(fhandler->sgnss_uart, fhandler->u8gnssBuffer, GNSSBufferSize);
	}
	else
	{
		eResult=GNSS_PARAMETER_ERROR;
	}
	return eResult;
}

GNSS_Result gnss_parse(GNSS_Handler* fhandler, uint32_t size)
{
	GNSS_Result eResult = GNSS_OK;
	enum minmea_sentence_id frametype = minmea_sentence_id(fhandler->u8gnssBuffer, true);
	if (frametype != MINMEA_UNKNOWN)
	{
		switch (frametype) {
			case MINMEA_SENTENCE_RMC: {
				struct minmea_sentence_rmc frame;
				if (minmea_parse_rmc(&frame, fhandler->u8gnssBuffer)) {
					fhandler->dLatitude = (double)(frame.latitude.value)/frame.latitude.scale;
					fhandler->dLongitude = (double)(frame.longitude.value)/frame.longitude.scale;
					fhandler->fSpeed = minmea_tofloat(&frame.speed);
					fhandler->u8TimeHour = frame.time.hours;
					fhandler->u8TimeMin = frame.time.minutes;
					fhandler->u8TimeSec = frame.time.seconds;
					fhandler->u32TimeMicro = frame.time.microseconds;
					fhandler->fCog = minmea_tofloat(&frame.course);
					fhandler->u32Date = frame.date.day*10000 + frame.date.month*100 + frame.date.year;
				}
			} break;
		    case MINMEA_SENTENCE_GGA: {
		    	struct minmea_sentence_gga frame;
		        if (minmea_parse_gga(&frame, fhandler->u8gnssBuffer)) {
		        	fhandler->dLatitude = (double)(frame.latitude.value)/frame.latitude.scale;
		        	fhandler->dLongitude = (double)(frame.longitude.value)/frame.longitude.scale;
		        	fhandler->u8TimeHour = frame.time.hours;
		        	fhandler->u8TimeMin = frame.time.minutes;
		        	fhandler->u8TimeSec = frame.time.seconds;
		        	fhandler->u32TimeMicro = frame.time.microseconds;
		        	fhandler->u8Fix = frame.fix_quality;
		        	fhandler->u8SatUsed = frame.satellites_tracked;
		        	fhandler->fhdop = minmea_tofloat(&frame.hdop);
		        	if(frame.altitude_units=='M')
		        		fhandler->fAltitude = minmea_tofloat(&frame.altitude);
		        }
		    } break;
		    case MINMEA_SENTENCE_GST: {
		    	struct minmea_sentence_gst frame;
		        if (minmea_parse_gst(&frame, fhandler->u8gnssBuffer)) {
		        	fhandler->u8TimeHour = frame.time.hours;
		        	fhandler->u8TimeMin = frame.time.minutes;
		        	fhandler->u8TimeSec = frame.time.seconds;
		        	fhandler->u32TimeMicro = frame.time.microseconds;
		        }
		    } break;
		    case MINMEA_SENTENCE_GSV: {
		    	struct minmea_sentence_gsv frame;
		        if (minmea_parse_gsv(&frame, fhandler->u8gnssBuffer)) {
		        	fhandler->u8SatInView = frame.total_sats;
		        }
		    } break;
		    case MINMEA_SENTENCE_VTG: {
		    	struct minmea_sentence_vtg frame;
		        if (minmea_parse_vtg(&frame, fhandler->u8gnssBuffer)) {
		        }
		    } break;
		    case MINMEA_SENTENCE_ZDA: {
		    	struct minmea_sentence_zda frame;
		        if (minmea_parse_zda(&frame, fhandler->u8gnssBuffer)) {
		        	fhandler->u8TimeHour = frame.time.hours;
		        	fhandler->u8TimeMin = frame.time.minutes;
		        	fhandler->u8TimeSec = frame.time.seconds;
		        	fhandler->u32TimeMicro = frame.time.microseconds;
		        	fhandler->u32Date = frame.date.day*10000 + frame.date.month*100 + frame.date.year;
		        }
		    } break;
		    case MINMEA_INVALID: {
		    	eResult =GNSS_INTERNAL_ERROR;
		    } break;
		    default: {
		    	eResult =GNSS_INTERNAL_ERROR;
		    } break;
		}
	}
	return eResult;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if(huart->Instance==GNSS_Handle.sgnss_uart->Instance)
	{

		//HAL_UART_Transmit(&huart2, uart_rx_data, Size, 100);
		gnss_parse(&GNSS_Handle,Size);
		HAL_UARTEx_ReceiveToIdle_DMA(GNSS_Handle.sgnss_uart, GNSS_Handle.u8gnssBuffer, GNSSBufferSize);
	}

}
