#include "spi_cf.h"
#include "server.h"
#include "client.h"
#include "flash_f4.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"

#include "loopback.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5500.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//Include MQTT library
#include "MQTTSNConect.h"

void Delay(__IO uint32_t nCount);
uint8_t  socket_sn = 0;

int main(void)
{
  //Khoi tao cac thong so co ban MAC, IP, GW, SN,...
  W5500_Init();
  
  MQTTPacket_connectData option = MQTTSNPacket_connectData_initializer;
  while (1)
  {}
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}