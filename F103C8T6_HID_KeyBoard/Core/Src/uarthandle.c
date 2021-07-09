#include "main.h"
#include "usart.h"

uint8_t Cmd_Buf[6];
static uint8_t Cmd_Buf_Cnt;

uint8_t rx_buffer_3[BUFFER_SIZE];
__IO uint8_t recv_end_flag_3;
__IO uint8_t rx_len_3;

void CmdHandle(void)
{
	if(aRxBuffer[0]==0xe8)
	{
		Cmd_Buf_Cnt=0;
		Cmd_Buf[Cmd_Buf_Cnt]=0xe8;
	}
	else if(Cmd_Buf[0]==0xe8)
	{
		if(aRxBuffer[0]==0xe9)
		{
			Cmd_Buf[0]=0;
			if(Cmd_Buf_Cnt!=4)
			{
				Cmd_Buf[0]=0;
				return;
			}
			KeyBuff[0] = Cmd_Buf[1];
			KeyBuff[1] = Cmd_Buf[2];
			KeyBuff[2] = Cmd_Buf[3];
			KeyBuff[3] = Cmd_Buf[4];
			CustomDataWriteToFlash(KeyBuff[3]<<24 | KeyBuff[2]<<16 | KeyBuff[1]<<8 | KeyBuff[0]);
			HAL_UART_Transmit(&huart1,KeyBuff,4,1000);
		}
		else
		{
			if(Cmd_Buf_Cnt>=6)
			{
				Cmd_Buf[0]=0;
				return;
			}
			Cmd_Buf_Cnt++;
			Cmd_Buf[Cmd_Buf_Cnt]=aRxBuffer[0];
		}
	}
}

void UART_IRQ(UART_HandleTypeDef *huart)
{
	uint32_t tmp_flag;
	if(huart == &huart3)
	{
		tmp_flag =__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE);
		if((tmp_flag != RESET))
		{
#if INTR==0
			recv_end_flag_3 = 1;
#endif
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			HAL_UART_DMAStop(huart);
			uint32_t temp = __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
			rx_len_3 = BUFFER_SIZE - temp;
		}
	}
}
