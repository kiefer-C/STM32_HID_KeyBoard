#include "main.h"
#include "usart.h"

uint8_t Cmd_Buf[6];
static uint8_t Cmd_Buf_Cnt;

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
