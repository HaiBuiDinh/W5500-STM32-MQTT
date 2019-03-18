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

void Delay(__IO uint32_t nCount);
uint8_t  socket_sn = 0;

int main(void)
{
  //Khoi tao cac thong so co ban MAC, IP, GW, SN,...
  W5500_Init();
  //Khong khoi tao socket nua, dong vai tro nhu mot client
  while (1)
  {
    //Lang nghe xem co ket noi nao toi khong SOCK_LISTEN(0x14)
    listen(socket_sn);
    switch (getSn_SR(socket_sn))
    {
    case SOCK_ESTABLISHED: //(0x17)
      break;
    case SOCK_CLOSE_WAIT: //(0x13)
      disconnect(socket_sn);
      if (getSn_SR(socket_sn) == SOCK_CLOSED)
      {
        server_init();
      }
      break;
    default:
      break;
    }
  }
}
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}