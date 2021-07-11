
#include <string.h>
#include "as608.h"

u32 AS608Addr = 0XFFFFFFFF; //Ä¬ÈÏ
#define HEAD_H 0xef
#define HEAD_L 0x01
#define PACK_CMD 0x01

u16 ValidN;

//³õÊ¼»¯PA6ÎªÏÂÀ­ÊäÈë		    
//¶ÁÃş³ö¸ĞÓ¦×´Ì¬(´¥Ãş¸ĞÓ¦Ê±Êä³ö¸ßµçÆ½ĞÅºÅ)
//void PS_StaGPIO_Init(void)
//{   
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//Ê¹ÄÜGPIOAÊ±ÖÓ
//  //³õÊ¼»¯¶Á×´Ì¬Òı½ÅGPIOA
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//ÊäÈëÏÂÀ­Ä£Ê½
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIO	
//}
//´®¿Ú·¢ËÍÒ»¸ö×Ö½Ú
//static void MYUSART_SendData(u8 data)
//{
//	while((USART2->SR&0X40)==0); 
//	USART2->DR = data;
//}
//·¢ËÍ°üÍ·
//static void SendHead(void)
//{
//	MYUSART_SendData(0xEF);
//	MYUSART_SendData(0x01);
//}
//·¢ËÍµØÖ·
//static void SendAddr(void)
//{
//	MYUSART_SendData(AS608Addr>>24);
//	MYUSART_SendData(AS608Addr>>16);
//	MYUSART_SendData(AS608Addr>>8);
//	MYUSART_SendData(AS608Addr);
//}
//·¢ËÍ°ü±êÊ¶,
//static void SendFlag(u8 flag)
//{
//	MYUSART_SendData(flag);
//}
//·¢ËÍ°ü³¤¶È
//static void SendLength(int length)
//{
//	MYUSART_SendData(length>>8);
//	MYUSART_SendData(length);
//}
//·¢ËÍÖ¸ÁîÂë
//static void Sendcmd(u8 cmd)
//{
//	MYUSART_SendData(cmd);
//}
//·¢ËÍĞ£ÑéºÍ
//static void SendCheck(u16 check)
//{
//	MYUSART_SendData(check>>8);
//	MYUSART_SendData(check);
//}
//ÅĞ¶ÏÖĞ¶Ï½ÓÊÕµÄÊı×éÓĞÃ»ÓĞÓ¦´ğ°ü
//waittimeÎªµÈ´ıÖĞ¶Ï½ÓÊÕÊı¾İµÄÊ±¼ä£¨µ¥Î»1ms£©
//·µ»ØÖµ£ºÊı¾İ°üÊ×µØÖ·
static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';
//	USART2_RX_STA=0;
//	while(--waittime)
//	{
//		HAL_Delay(1);
//		if(USART2_RX_STA&0X8000)//½ÓÊÕµ½Ò»´ÎÊı¾İ
//		{
//			USART2_RX_STA=0;
//			data=strstr((const char*)USART2_RX_BUF,(const char*)str);
//			if(data)
//				return (u8*)data;	
//		}
//	}
	while(--waittime)
	{
		HAL_Delay(1);
		if(recv_end_flag_3)//½ÓÊÕµ½Ò»´ÎÊı¾İ
		{
			recv_end_flag_3=0;
			rx_buffer_3[rx_len_3]='\0';
			data=strstr((const char*)rx_buffer_3,(const char*)str);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}
//Â¼ÈëÍ¼Ïñ PS_GetImage
//¹¦ÄÜ:Ì½²âÊÖÖ¸£¬Ì½²âµ½ºóÂ¼ÈëÖ¸ÎÆÍ¼Ïñ´æÓÚImageBuffer¡£ 
//Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_GetImage(void)
{
//  u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x01);
//  temp =  0x01+0x03+0x01;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x3;
	u16 temp = PACK_CMD+length+0x01;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x01,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//Éú³ÉÌØÕ÷ PS_GenChar
//¹¦ÄÜ:½«ImageBufferÖĞµÄÔ­Ê¼Í¼ÏñÉú³ÉÖ¸ÎÆÌØÕ÷ÎÄ¼ş´æÓÚCharBuffer1»òCharBuffer2			 
//²ÎÊı:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_GenChar(u8 BufferID)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x04);
//	Sendcmd(0x02);
//	MYUSART_SendData(BufferID);
//	temp = 0x01+0x04+0x02+BufferID;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x4;
	u16 temp = PACK_CMD+length+0x02+BufferID;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x02,BufferID,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//¾«È·±È¶ÔÁ½Ã¶Ö¸ÎÆÌØÕ÷ PS_Match
//¹¦ÄÜ:¾«È·±È¶ÔCharBuffer1 ÓëCharBuffer2 ÖĞµÄÌØÕ÷ÎÄ¼ş 
//Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_Match(void)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x03);
//	temp = 0x01+0x03+0x03;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x3;
	u16 temp = PACK_CMD+length+0x03;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x03,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//ËÑË÷Ö¸ÎÆ PS_Search
//¹¦ÄÜ:ÒÔCharBuffer1»òCharBuffer2ÖĞµÄÌØÕ÷ÎÄ¼şËÑË÷Õû¸ö»ò²¿·ÖÖ¸ÎÆ¿â.ÈôËÑË÷µ½£¬Ôò·µ»ØÒ³Âë¡£			
//²ÎÊı:  BufferID @ref CharBuffer1	CharBuffer2
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö£¬Ò³Âë£¨ÏàÅäÖ¸ÎÆÄ£°å£©
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x08);
//	Sendcmd(0x04);
//	MYUSART_SendData(BufferID);
//	MYUSART_SendData(StartPage>>8);
//	MYUSART_SendData(StartPage);
//	MYUSART_SendData(PageNum>>8);
//	MYUSART_SendData(PageNum);
//	temp = 0x01+0x08+0x04+BufferID
//	+(StartPage>>8)+(u8)StartPage
//	+(PageNum>>8)+(u8)PageNum;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x08;
	u16 temp = PACK_CMD+length+0x04+BufferID+(StartPage>>8)+(u8)StartPage+(PageNum>>8)+(u8)PageNum;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x04,BufferID,StartPage>>8,StartPage,PageNum>>8,PageNum,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//ºÏ²¢ÌØÕ÷£¨Éú³ÉÄ£°å£©PS_RegModel
//¹¦ÄÜ:½«CharBuffer1ÓëCharBuffer2ÖĞµÄÌØÕ÷ÎÄ¼şºÏ²¢Éú³É Ä£°å,½á¹û´æÓÚCharBuffer1ÓëCharBuffer2	
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_RegModel(void)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x05);
//	temp = 0x01+0x03+0x05;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x3;
  u16 temp = PACK_CMD+length+0x05;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x05,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}
//´¢´æÄ£°å PS_StoreChar
//¹¦ÄÜ:½« CharBuffer1 »ò CharBuffer2 ÖĞµÄÄ£°åÎÄ¼ş´æµ½ PageID ºÅflashÊı¾İ¿âÎ»ÖÃ¡£			
//²ÎÊı:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID£¨Ö¸ÎÆ¿âÎ»ÖÃºÅ£©
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_StoreChar(u8 BufferID,u16 PageID)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x06);
//	Sendcmd(0x06);
//	MYUSART_SendData(BufferID);
//	MYUSART_SendData(PageID>>8);
//	MYUSART_SendData(PageID);
//	temp = 0x01+0x06+0x06+BufferID
//	+(PageID>>8)+(u8)PageID;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x06;
  u16 temp = PACK_CMD+length+0x06+BufferID+(PageID>>8)+(u8)PageID;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x06,BufferID,PageID>>8,PageID,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}
//É¾³ıÄ£°å PS_DeletChar
//¹¦ÄÜ:  É¾³ıflashÊı¾İ¿âÖĞÖ¸¶¨IDºÅ¿ªÊ¼µÄN¸öÖ¸ÎÆÄ£°å
//²ÎÊı:  PageID(Ö¸ÎÆ¿âÄ£°åºÅ)£¬NÉ¾³ıµÄÄ£°å¸öÊı¡£
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_DeletChar(u16 PageID,u16 N)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x07);
//	Sendcmd(0x0C);
//	MYUSART_SendData(PageID>>8);
//	MYUSART_SendData(PageID);
//	MYUSART_SendData(N>>8);
//	MYUSART_SendData(N);
//	temp = 0x01+0x07+0x0C
//	+(PageID>>8)+(u8)PageID
//	+(N>>8)+(u8)N;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x07;
  u16 temp = PACK_CMD+length+0x0C+(PageID>>8)+(u8)PageID+(N>>8)+(u8)N;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x0C,PageID>>8,PageID,N>>8,N,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//Çå¿ÕÖ¸ÎÆ¿â PS_Empty
//¹¦ÄÜ:  É¾³ıflashÊı¾İ¿âÖĞËùÓĞÖ¸ÎÆÄ£°å
//²ÎÊı:  ÎŞ
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_Empty(void)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x0D);
//	temp = 0x01+0x03+0x0D;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u16 length = 0x3;
  u16 temp = PACK_CMD+length+0x0D;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,0x0D,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//Ğ´ÏµÍ³¼Ä´æÆ÷ PS_WriteReg
//¹¦ÄÜ:  Ğ´Ä£¿é¼Ä´æÆ÷
//²ÎÊı:  ¼Ä´æÆ÷ĞòºÅRegNum:4\5\6
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_WriteReg(u8 RegNum,u8 DATA)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x05);
//	Sendcmd(0x0E);
//	MYUSART_SendData(RegNum);
//	MYUSART_SendData(DATA);
//	temp = RegNum+DATA+0x01+0x05+0x0E;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u8 cmd = 0x0E;
	u16 length = 0x05;
  u16 temp = PACK_CMD+length+cmd+RegNum+DATA;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,cmd,RegNum,DATA,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	if(ensure==0)
		;//printf("\r\nÉèÖÃ²ÎÊı³É¹¦£¡");
	else
		;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//¶ÁÏµÍ³»ù±¾²ÎÊı PS_ReadSysPara
//¹¦ÄÜ:  ¶ÁÈ¡Ä£¿éµÄ»ù±¾²ÎÊı£¨²¨ÌØÂÊ£¬°ü´óĞ¡µÈ)
//²ÎÊı:  ÎŞ
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö + »ù±¾²ÎÊı£¨16bytes£©
u8 PS_ReadSysPara(SysPara *p)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x0F);
//	temp = 0x01+0x03+0x0F;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u8 cmd = 0x0F;
	u16 length = 0x3;
  u16 temp = PACK_CMD+length+cmd;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,cmd,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(1000);
	if(data)
	{
		ensure = data[9];
		p->PS_max = (data[14]<<8)+data[15];
		p->PS_level = data[17];
		p->PS_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}		
	else
		ensure=0xff;
	if(ensure==0x00)
	{
		;//printf("\r\nÄ£¿é×î´óÖ¸ÎÆÈİÁ¿=%d",p->PS_max);
		//printf("\r\n¶Ô±ÈµÈ¼¶=%d",p->PS_level);
		//printf("\r\nµØÖ·=%x",p->PS_addr);
		//printf("\r\n²¨ÌØÂÊ=%d",p->PS_N*9600);
	}
	else 
			;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//ÉèÖÃÄ£¿éµØÖ· PS_SetAddr
//¹¦ÄÜ:  ÉèÖÃÄ£¿éµØÖ·
//²ÎÊı:  PS_addr
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_SetAddr(u32 PS_addr)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x07);
//	Sendcmd(0x15);
//	MYUSART_SendData(PS_addr>>24);
//	MYUSART_SendData(PS_addr>>16);
//	MYUSART_SendData(PS_addr>>8);
//	MYUSART_SendData(PS_addr);
//	temp = 0x01+0x07+0x15
//	+(u8)(PS_addr>>24)+(u8)(PS_addr>>16)
//	+(u8)(PS_addr>>8) +(u8)PS_addr;				
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u8 cmd = 0x15;
	u16 length = 0x07;
  u16 temp = PACK_CMD+length+cmd+(u8)(PS_addr>>24)+(u8)(PS_addr>>16)+(u8)(PS_addr>>8) +(u8)PS_addr;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,cmd,PS_addr>>24,PS_addr>>16,PS_addr>>8,PS_addr,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);

	AS608Addr=PS_addr;//·¢ËÍÍêÖ¸Áî£¬¸ü»»µØÖ·
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
	if(ensure==0x00)
		;//printf("\r\nÉèÖÃµØÖ·³É¹¦£¡");
	else
		;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
#if 0
//¹¦ÄÜ£º Ä£¿éÄÚ²¿ÎªÓÃ»§¿ª±ÙÁË256bytesµÄFLASH¿Õ¼äÓÃÓÚ´æÓÃ»§¼ÇÊÂ±¾,
//	¸Ã¼ÇÊÂ±¾Âß¼­ÉÏ±»·Ö³É 16 ¸öÒ³¡£
//²ÎÊı:  NotePageNum(0~15),Byte32(ÒªĞ´ÈëÄÚÈİ£¬32¸ö×Ö½Ú)
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö
u8 PS_WriteNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
  u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
	SendLength(36);
	Sendcmd(0x18);
	MYUSART_SendData(NotePageNum);
	for(i=0;i<32;i++)
	 {
		 MYUSART_SendData(Byte32[i]);
		 temp += Byte32[i];
	 }
  temp =0x01+36+0x18+NotePageNum+temp;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//¶Á¼ÇÊÂPS_ReadNotepad
//¹¦ÄÜ£º  ¶ÁÈ¡FLASHÓÃ»§ÇøµÄ128bytesÊı¾İ
//²ÎÊı:  NotePageNum(0~15)
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö+ÓÃ»§ĞÅÏ¢
u8 PS_ReadNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
  u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
	SendLength(0x04);
	Sendcmd(0x19);
	MYUSART_SendData(NotePageNum);
	temp = 0x01+0x04+0x19+NotePageNum;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		for(i=0;i<32;i++)
		{
			Byte32[i]=data[10+i];
		}
	}
	else
		ensure=0xff;
	return ensure;
}
#endif
//¸ßËÙËÑË÷PS_HighSpeedSearch
//¹¦ÄÜ£ºÒÔ CharBuffer1»òCharBuffer2ÖĞµÄÌØÕ÷ÎÄ¼ş¸ßËÙËÑË÷Õû¸ö»ò²¿·ÖÖ¸ÎÆ¿â¡£
//		  ÈôËÑË÷µ½£¬Ôò·µ»ØÒ³Âë,¸ÃÖ¸Áî¶ÔÓÚµÄÈ·´æÔÚÓÚÖ¸ÎÆ¿âÖĞ £¬ÇÒµÇÂ¼Ê±ÖÊÁ¿
//		  ºÜºÃµÄÖ¸ÎÆ£¬»áºÜ¿ì¸ø³öËÑË÷½á¹û¡£
//²ÎÊı:  BufferID£¬ StartPage(ÆğÊ¼Ò³)£¬PageNum£¨Ò³Êı£©
//ËµÃ÷:  Ä£¿é·µ»ØÈ·ÈÏ×Ö+Ò³Âë£¨ÏàÅäÖ¸ÎÆÄ£°å£©
u8 PS_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x08);
//	Sendcmd(0x1b);
//	MYUSART_SendData(BufferID);
//	MYUSART_SendData(StartPage>>8);
//	MYUSART_SendData(StartPage);
//	MYUSART_SendData(PageNum>>8);
//	MYUSART_SendData(PageNum);
//	temp = 0x01+0x08+0x1b+BufferID
//	+(StartPage>>8)+(u8)StartPage
//	+(PageNum>>8)+(u8)PageNum;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u8 cmd = 0x1b;
	u16 length = 0x08;
  u16 temp = PACK_CMD+length+cmd+BufferID+(StartPage>>8)+(u8)StartPage+(PageNum>>8)+(u8)PageNum;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,cmd,BufferID,StartPage>>8,StartPage,PageNum>>8,PageNum,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
	data=JudgeStr(2000);
 	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[10]<<8) +data[11];
		p->mathscore=(data[12]<<8) +data[13];
	}
	else
		ensure=0xff;
	return ensure;
}
//¶ÁÓĞĞ§Ä£°å¸öÊı PS_ValidTempleteNum
//¹¦ÄÜ£º¶ÁÓĞĞ§Ä£°å¸öÊı
//²ÎÊı: ÎŞ
//ËµÃ÷: Ä£¿é·µ»ØÈ·ÈÏ×Ö+ÓĞĞ§Ä£°å¸öÊıValidN
u8 PS_ValidTempleteNum(u16 *ValidN)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x1d);
//	temp = 0x01+0x03+0x1d;
//	SendCheck(temp);
	
	recv_end_flag_3=0;
	u8 cmd = 0x1d;
	u16 length = 0x3;
  u16 temp = PACK_CMD+length+cmd;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,cmd,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
	if(ensure==0x00)
	{
		;//printf("\r\nÓĞĞ§Ö¸ÎÆ¸öÊı=%d",(data[10]<<8)+data[11]);
	}
	else
		;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//ÓëAS608ÎÕÊÖ PS_HandShake
//²ÎÊı: PS_AddrµØÖ·Ö¸Õë
//ËµÃ÷: Ä£¿é·µĞÂµØÖ·£¨ÕıÈ·µØÖ·£©	
u8 PS_HandShake(u32 *PS_Addr)
{
	recv_end_flag_3=0;
	u8 cmd = 0x35;
	u16 length = 0x3;
  u16 temp = PACK_CMD+length+cmd;
	u8 send_cmd[]={HEAD_H,HEAD_L,AS608Addr>>24,AS608Addr>>16,AS608Addr>>8,AS608Addr,
	PACK_CMD,length>>8,length,cmd,temp>>8,temp};
	HAL_UART_Transmit(&huart3,send_cmd,sizeof(send_cmd),1000);
	
//	SendHead();
//	SendAddr();
//	MYUSART_SendData(0X01);
//	MYUSART_SendData(0X00);
//	MYUSART_SendData(0X00);	
	HAL_Delay(200);
//	if(USART2_RX_STA&0X8000)//½ÓÊÕµ½Êı¾İ
//	{		
//		if(//ÅĞ¶ÏÊÇ²»ÊÇÄ£¿é·µ»ØµÄÓ¦´ğ°ü				
//					USART2_RX_BUF[0]==0XEF
//				&&USART2_RX_BUF[1]==0X01
//				&&USART2_RX_BUF[6]==0X07
//			)
//			{
//				*PS_Addr=(USART2_RX_BUF[2]<<24) + (USART2_RX_BUF[3]<<16)
//								+(USART2_RX_BUF[4]<<8) + (USART2_RX_BUF[5]);
//				USART2_RX_STA=0;
//				return 0;
//			}
//		USART2_RX_STA=0;					
//	}
	if(recv_end_flag_3)//½ÓÊÕµ½Êı¾İ
	{		
		if(//ÅĞ¶ÏÊÇ²»ÊÇÄ£¿é·µ»ØµÄÓ¦´ğ°ü				
					rx_buffer_3[0]==0XEF
				&&rx_buffer_3[1]==0X01
				&&rx_buffer_3[6]==0X07
			)
			{
				*PS_Addr=(rx_buffer_3[2]<<24) + (rx_buffer_3[3]<<16)
								+(rx_buffer_3[4]<<8) + (rx_buffer_3[5]);
				return 0;
			}			
	}
	return 1;		
}
//Ä£¿éÓ¦´ğ°üÈ·ÈÏÂëĞÅÏ¢½âÎö
//¹¦ÄÜ£º½âÎöÈ·ÈÏÂë´íÎóĞÅÏ¢·µ»ØĞÅÏ¢
//²ÎÊı: ensure
const char *EnsureMessage(u8 ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="Êı¾İ°ü½ÓÊÕ´íÎó";break;
		case  0x02:
			p="´«¸ĞÆ÷ÉÏÃ»ÓĞÊÖÖ¸";break;
		case  0x03:
			p="Â¼ÈëÖ¸ÎÆÍ¼ÏñÊ§°Ü";break;
		case  0x04:
			p="Ö¸ÎÆÍ¼ÏñÌ«¸É¡¢Ì«µ­¶øÉú²»³ÉÌØÕ÷";break;
		case  0x05:
			p="Ö¸ÎÆÍ¼ÏñÌ«Êª¡¢Ì«ºı¶øÉú²»³ÉÌØÕ÷";break;
		case  0x06:
			p="Ö¸ÎÆÍ¼ÏñÌ«ÂÒ¶øÉú²»³ÉÌØÕ÷";break;
		case  0x07:
			p="Ö¸ÎÆÍ¼ÏñÕı³££¬µ«ÌØÕ÷µãÌ«ÉÙ£¨»òÃæ»ıÌ«Ğ¡£©¶øÉú²»³ÉÌØÕ÷";break;
		case  0x08:
			p="Ö¸ÎÆ²»Æ¥Åä";break;
		case  0x09:
			p="Ã»ËÑË÷µ½Ö¸ÎÆ";break;
		case  0x0a:
			p="ÌØÕ÷ºÏ²¢Ê§°Ü";break;
		case  0x0b:
			p="·ÃÎÊÖ¸ÎÆ¿âÊ±µØÖ·ĞòºÅ³¬³öÖ¸ÎÆ¿â·¶Î§";
		case  0x10:
			p="É¾³ıÄ£°åÊ§°Ü";break;
		case  0x11:
			p="Çå¿ÕÖ¸ÎÆ¿âÊ§°Ü";break;	
		case  0x15:
			p="»º³åÇøÄÚÃ»ÓĞÓĞĞ§Ô­Ê¼Í¼¶øÉú²»³ÉÍ¼Ïñ";break;
		case  0x18:
			p="¶ÁĞ´ FLASH ³ö´í";break;
		case  0x19:
			p="Î´¶¨Òå´íÎó";break;
		case  0x1a:
			p="ÎŞĞ§¼Ä´æÆ÷ºÅ";break;
		case  0x1b:
			p="¼Ä´æÆ÷Éè¶¨ÄÚÈİ´íÎó";break;
		case  0x1c:
			p="¼ÇÊÂ±¾Ò³ÂëÖ¸¶¨´íÎó";break;
		case  0x1f:
			p="Ö¸ÎÆ¿âÂú";break;
		case  0x20:
			p="µØÖ·´íÎó";break;
		default :
			p="Ä£¿é·µ»ØÈ·ÈÏÂëÓĞÎó";break;
	}
 return p;	
}

/*************************************************************************
 DESC:ÏÔÊ¾´íÎóĞÅÏ¢
************************************************************************/
void ShowErrMessage(u8 ensure)
{

}
/***********************************************************************
DESC:Ìí¼ÓÖ¸ÎÆ
************************************************************************/
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				
			  OLED_ShowString(0,6,(unsigned char*)" Touch finger!  ");
			  OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//????
					if(ensure==0x00)
					{
						
						OLED_ShowString(0,6,(unsigned char*)" Figer Correct  ");
						OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
						i=0;
						processnum=1;					
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
			
			  OLED_ShowString(0,6,(unsigned char*)" Touch Again!   ");
			  OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//			
					if(ensure==0x00)
					{
						
						OLED_ShowString(0,6,(unsigned char*)" Figer Correct  ");
						OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
						i=0;
						processnum=2;
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				
			  OLED_ShowString(0,6,(unsigned char*)" Figer Compare  ");
				OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
				ensure=PS_Match();
				if(ensure==0x00) 
				{

					OLED_ShowString(0,6,(unsigned char*)" Figer are same ");
					OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
					processnum=3;
				}
				else 
				{
					
					ShowErrMessage(ensure);
					i=0;
					processnum=0;		
				}
				HAL_Delay(1000);
				break;

			case 3:

			  OLED_ShowString(0,6,(unsigned char*)" Touch Again!   ");
			  OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{

					OLED_ShowString(0,6,(unsigned char*)" Comper Success!");
			    OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");
					processnum=4;//?????
				}else {processnum=0;ShowErrMessage(ensure);}
				HAL_Delay(1000);
				break;
				
			case 4:	
					
				do
					ID=1;
				while(!(ID<300));
				ensure=PS_StoreChar(CharBuffer2,ID);
				if(ensure==0x00) 
				{
          OLED_ShowString(0,6,(unsigned char*)"  ADD Success!  ");
			    OLED_ShowString(0,4,(unsigned char*)" FUN:ADD FRIGER ");					
					PS_ValidTempleteNum(&ValidN);
					HAL_Delay(1500);
					
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		HAL_Delay(800);
		if(i==5)//
		{
		
			break;	
		}				
	}
}

/**************************************************************
DESC:¶Ô±ÈÖ¸ÎÆ
*****************************************************************/
u8 press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	OLED_ShowString(0,6,(unsigned char*)" Begin Compare ");
	ensure=PS_GetImage();
	if(ensure==0x00)// 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//
			{				
				OLED_ShowString(0,6,(unsigned char*)" Comper PASS!   ");
			  OLED_ShowString(0,4,(unsigned char*)" FUN:CPR FRIGER ");
				return 0;
			}
			else 
			{
			    OLED_ShowString(0,6,(unsigned char*)" Comper Fail!   ");
			    OLED_ShowString(0,4,(unsigned char*)" FUN:CPR FRIGER ");
			}	
			
		}
			else 
			{
			    OLED_ShowString(0,6,(unsigned char*)" Comper Fail!   ");
			    OLED_ShowString(0,4,(unsigned char*)" FUN:CPR FRIGER ");
			}	
		
	 HAL_Delay(1000);//

	}
	return 1;
		
}
/******************************************************************************
DESC:É¾³ıÖ¸ÎÆ
*******************************************************************************/

void Del_FR(void)
{
	u8  ensure;
//	u16 num;OLED_ShowStr(0,6,(unsigned char*)" FINGER DELL OK ",2);
	//LCD_Fill(0,100,lcddev.width,160,WHITE);
	//Show_Str_Mid(0,100,"Delete fingerprint",16,240);//??????
	//Show_Str_Mid(0,120,"Input ID and touch Enter!",16,240);//????ID???“Enter”
	//Show_Str_Mid(0,140,"0=< ID <=299",16,240);
	HAL_Delay(50);
	//AS608_load_keyboard(0,170,(u8**)kbd_delFR);
	//num=GET_NUM();//???????
	//if(num==0xFFFF)
		//goto MENU ; //?????
	//else if(num==0xFF00)
		ensure=PS_Empty();//?????
	//else 
		//ensure=PS_DeletChar(num,1);//??????
	if(ensure==0)
	{
		OLED_ShowString(0,6,(unsigned char*)" FINGER DELL OK ");
	  OLED_ShowString(0,4,(unsigned char*)" FUN:DEL FRIGER ");
		//LCD_Fill(0,120,lcddev.width,160,WHITE);
		//Show_Str_Mid(0,140,"Delete fingerprint success!!!",16,240);//??????		
	}
  //else
		//ShowErrMessage(ensure);	
	HAL_Delay(1500);//???????
	//PS_ValidTempleteNum(&ValidN);//??????
	//LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//????????
//MENU:	
	//LCD_Fill(0,100,lcddev.width,160,WHITE);
	//HAL_Delay(50);
	//AS608_load_keyboard(0,170,(u8**)kbd_menu);
}



