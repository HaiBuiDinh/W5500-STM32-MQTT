#include "spi_cf.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static uint8_t W5500_ReadByte(void);
static void W5500_WriteByte(uint8_t byte);
static void W5500_SPI_Init(void);
static void W5500_CS_Select(void);
static void W5500_CS_Deselect(void);
static void W5500_Mem_Init(void);

/****************SPI1-Master*******************
		 Connect W5500 to SPI1 follow:
                  PA4 --> W5500_NSS
		  PA5 --> W5500_SCK
		  PA6 --> W5500_MISO
		  PA7 --> W5500_MOSI
**********************************************/
static void W5500_SPI_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable SPI and GPIO Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        
	//Configure GPIO 	
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
        
        //Configure Interrupt
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction) 1);
        
        /*
        GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_13;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
        GPIO_WriteBit(GPIOA, GPIO_Pin_13, (BitAction) 0);
        */
        
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
              
	//Set PA5,6,7 as Output push-pull -SCK, MISO, MOSI
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Configure SPI 2line_full duplex, master mode, 8bit, MSB first
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction 	= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode 		= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize 		= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 		= SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA		= SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS		= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit		= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial 	= 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, DISABLE);
	SPI_Cmd(SPI1, ENABLE);
}

//Đăng kí một số chức năng trong file wizchip_conf.h
void reg_wizchip_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void));
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_decel)(void));
void reg_wizchip_bus_cbfunc(iodata_t(*bus_rb)(uint32_t addr), void(*bus_wb)(uint32_t addr, iodata_t wb));
void reg_wizchip_spi_cbfunc(uint8_t(*spi_rb)(void), void(*spi_wb)(uint8_t wb));
void reg_wizchip_spiburst_cbfunc(void(*spi_rb)(uint8_t* pBuf, uint16_t len), void(*spi_wb)(uint8_t* pBuf, uint16_t len));

static void W5500_CS_Select(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4); //Chân select cần được đẩy về 0
}

static void W5500_CS_Deselect(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);  //Khi đẩy select lên 1 thì sẽ deactive W5500
}

static void W5500_WriteByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, byte);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);
}

static uint8_t W5500_ReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, 0xFF);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return(SPI_I2S_ReceiveData(SPI1));
}

/* Đoạn nay chi de show, phuc vu cho debug
static void W5500_Network_Init(void)
{
	uint8_t chipid[6];
        
	wiz_NetInfo gWIZNETINFO;
	uint8_t mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11}; //<Source MAC Address>
	uint8_t ip[4]  = {192, 168, 32, 120}; //<Source IP Address>
	uint8_t sn[4]  = {255, 255, 255, 0}; //<Subnet Mask>
	uint8_t gw[4]  = {192, 168, 32, 1}; //<Gateway IP Address>
	uint8_t dns[4] = {8, 8, 8, 8}; //<DNS server IP Address>
	
	//Copy data to W5500
	memcpy(gWIZNETINFO.ip, ip, 4);
	memcpy(gWIZNETINFO.sn, sn, 4);
	memcpy(gWIZNETINFO.gw, gw, 4);
	memcpy(gWIZNETINFO.mac, mac, 6);
	memcpy(gWIZNETINFO.dns, dns, 4);
	gWIZNETINFO.dhcp = NETINFO_STATIC; //<1 - static, 2 - DHCP>

	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
	
	//Display Network Information use USART
	ctlwizchip(CW_GET_ID, (void*)chipid);
	printf("\r\n=== %s NET CONF ===\r\n", (char*)chipid);
	printf("MAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], 
	gWIZNETINFO.mac[2],  gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
	printf("==============================\r\n");
	
	wizchip_init(0, 0);
	
	wiz_NetTimeout w_NetTimeout;
	w_NetTimeout.retry_cnt = 50;
	w_NetTimeout.time_100us = 1000;
	wizchip_settimeout(&w_NetTimeout);
}
*/

//Khoi tao cai dat lien quan toi W5500

void W5500_Init(void)
{
	W5500_SPI_Init();
	
	for (int i=0x5FFF; i>0; i--); //Delay mot khoang thoi gian de khoi tao
	W5500_Mem_Init();
	reg_wizchip_cris_cbfunc(0, 0);
	reg_wizchip_cs_cbfunc(W5500_CS_Select, W5500_CS_Deselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
	
	//W5500_Network_Init();
}

static void W5500_Mem_Init(void)
{
  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
  
  if (ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1)
  {
    printf("WIZCHIP Initialized fail.\r\n");
    return;
  }
}

void delay_us(uint16_t time)
{
  uint16_t i,j;
  for(i = 0; i<time; i++)
  {
    j=32;
    while(j>1)j--;
  }
}