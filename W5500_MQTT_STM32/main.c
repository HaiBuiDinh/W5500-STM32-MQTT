/* Include User's library */
#include "spi_cf.h"
#include "server.h"
#include "client.h"
#include "flash_f4.h"
#include "setup_client.h"

/* Include CMSIS library */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"

/* Include Wiz Library */
#include "loopback.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5500.h"

/* Include MQTT library */
//#include "MQTTClient.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void Delay(__IO uint32_t nCount);
uint8_t  socket_sn = 0;

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x11, 0x11},
                            .ip = {192, 168, 32, 121},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 32, 1},
                            .dns = {8, 8, 8, 8},
                            .dhcp = NETINFO_STATIC};
unsigned char targetIP[4] = {192,168,1,234};
unsigned int targetPort = 1883;

int main(void)
{
  uint8_t return_connect = 0;
  //Khoi tao cac thong so co ban MAC, IP, GW, SN,...
  W5500_Init();
  wizchip_setnetinfo(&gWIZNETINFO);
  server_init();
  return_connect = connect(socket_sn, targetIP, targetPort);
  if(return_connect != SOCK_OK) //SOCK_OK = 0x01
  {return_connect =3;}
  while (1)
  {}
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}