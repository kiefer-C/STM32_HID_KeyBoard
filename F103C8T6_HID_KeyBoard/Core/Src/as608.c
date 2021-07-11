
#include <string.h>
#include "as608.h"

u32 AS608Addr = 0XFFFFFFFF; //默认
#define HEAD_H 0xef
#define HEAD_L 0x01
#define PACK_CMD 0x01

u16 ValidN;

//初始化PA6为下拉输入		    
//读摸出感应状态(触摸感应时输出高电平信号)
//void PS_StaGPIO_Init(void)
//{   
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
//  //初始化读状态引脚GPIOA
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入下拉模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
//}
//串口发送一个字节
//static void MYUSART_SendData(u8 data)
//{
//	while((USART2->SR&0X40)==0); 
//	USART2->DR = data;
//}
//发送包头
//static void SendHead(void)
//{
//	MYUSART_SendData(0xEF);
//	MYUSART_SendData(0x01);
//}
//发送地址
//static void SendAddr(void)
//{
//	MYUSART_SendData(AS608Addr>>24);
//	MYUSART_SendData(AS608Addr>>16);
//	MYUSART_SendData(AS608Addr>>8);
//	MYUSART_SendData(AS608Addr);
//}
//发送包标识,
//static void SendFlag(u8 flag)
//{
//	MYUSART_SendData(flag);
//}
//发送包长度
//static void SendLength(int length)
//{
//	MYUSART_SendData(length>>8);
//	MYUSART_SendData(length);
//}
//发送指令码
//static void Sendcmd(u8 cmd)
//{
//	MYUSART_SendData(cmd);
//}
//发送校验和
//static void SendCheck(u16 check)
//{
//	MYUSART_SendData(check>>8);
//	MYUSART_SendData(check);
//}
//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
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
//		if(USART2_RX_STA&0X8000)//接收到一次数据
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
		if(recv_end_flag_3)//接收到一次数据
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
//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于ImageBuffer。 
//模块返回确认字
u8 PS_GetImage(void)
{
//  u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2			 
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//模块返回确认字
u8 PS_GenChar(u8 BufferID)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
u8 PS_Match(void)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。			
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2	
//说明:  模块返回确认字
u8 PS_RegModel(void)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。			
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
u8 PS_StoreChar(u8 BufferID,u16 PageID)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
u8 PS_DeletChar(u16 PageID,u16 N)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
u8 PS_Empty(void)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
u8 PS_WriteReg(u8 RegNum,u8 DATA)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
		;//printf("\r\n设置参数成功！");
	else
		;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//读系统基本参数 PS_ReadSysPara
//功能:  读取模块的基本参数（波特率，包大小等)
//参数:  无
//说明:  模块返回确认字 + 基本参数（16bytes）
u8 PS_ReadSysPara(SysPara *p)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
		;//printf("\r\n模块最大指纹容量=%d",p->PS_max);
		//printf("\r\n对比等级=%d",p->PS_level);
		//printf("\r\n地址=%x",p->PS_addr);
		//printf("\r\n波特率=%d",p->PS_N*9600);
	}
	else 
			;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
u8 PS_SetAddr(u32 PS_addr)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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

	AS608Addr=PS_addr;//发送完指令，更换地址
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
	if(ensure==0x00)
		;//printf("\r\n设置地址成功！");
	else
		;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
#if 0
//功能： 模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本,
//	该记事本逻辑上被分成 16 个页。
//参数:  NotePageNum(0~15),Byte32(要写入内容，32个字节)
//说明:  模块返回确认字
u8 PS_WriteNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
  u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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
//读记事PS_ReadNotepad
//功能：  读取FLASH用户区的128bytes数据
//参数:  NotePageNum(0~15)
//说明:  模块返回确认字+用户信息
u8 PS_ReadNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
  u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
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
//高速搜索PS_HighSpeedSearch
//功能：以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
//		  若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
//		  很好的指纹，会很快给出搜索结果。
//参数:  BufferID， StartPage(起始页)，PageNum（页数）
//说明:  模块返回确认字+页码（相配指纹模板）
u8 PS_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
//读有效模板个数 PS_ValidTempleteNum
//功能：读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
u8 PS_ValidTempleteNum(u16 *ValidN)
{
//	u16 temp;
  u8  ensure;
	u8  *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//命令包标识
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
		;//printf("\r\n有效指纹个数=%d",(data[10]<<8)+data[11]);
	}
	else
		;//printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址（正确地址）	
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
//	if(USART2_RX_STA&0X8000)//接收到数据
//	{		
//		if(//判断是不是模块返回的应答包				
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
	if(recv_end_flag_3)//接收到数据
	{		
		if(//判断是不是模块返回的应答包				
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
//模块应答包确认码信息解析
//功能：解析确认码错误信息返回信息
//参数: ensure
const char *EnsureMessage(u8 ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="数据包接收错误";break;
		case  0x02:
			p="传感器上没有手指";break;
		case  0x03:
			p="录入指纹图像失败";break;
		case  0x04:
			p="指纹图像太干、太淡而生不成特征";break;
		case  0x05:
			p="指纹图像太湿、太糊而生不成特征";break;
		case  0x06:
			p="指纹图像太乱而生不成特征";break;
		case  0x07:
			p="指纹图像正常，但特征点太少（或面积太小）而生不成特征";break;
		case  0x08:
			p="指纹不匹配";break;
		case  0x09:
			p="没搜索到指纹";break;
		case  0x0a:
			p="特征合并失败";break;
		case  0x0b:
			p="访问指纹库时地址序号超出指纹库范围";
		case  0x10:
			p="删除模板失败";break;
		case  0x11:
			p="清空指纹库失败";break;	
		case  0x15:
			p="缓冲区内没有有效原始图而生不成图像";break;
		case  0x18:
			p="读写 FLASH 出错";break;
		case  0x19:
			p="未定义错误";break;
		case  0x1a:
			p="无效寄存器号";break;
		case  0x1b:
			p="寄存器设定内容错误";break;
		case  0x1c:
			p="记事本页码指定错误";break;
		case  0x1f:
			p="指纹库满";break;
		case  0x20:
			p="地址错误";break;
		default :
			p="模块返回确认码有误";break;
	}
 return p;	
}

/*************************************************************************
 DESC:显示错误信息
************************************************************************/
void ShowErrMessage(u8 ensure)
{

}
/***********************************************************************
DESC:添加指纹
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
DESC:对比指纹
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
DESC:删除指纹
*******************************************************************************/

void Del_FR(void)
{
	u8  ensure;
//	u16 num;OLED_ShowStr(0,6,(unsigned char*)" FINGER DELL OK ",2);
	//LCD_Fill(0,100,lcddev.width,160,WHITE);
	//Show_Str_Mid(0,100,"Delete fingerprint",16,240);//??????
	//Show_Str_Mid(0,120,"Input ID and touch Enter!",16,240);//????ID???揈nter�
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



