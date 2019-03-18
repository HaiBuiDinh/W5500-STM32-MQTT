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
uint16_t status_RX = 0, status_RX_1 = 0;

int main(void)
{
  //ham dung de fix memory of RX register "setSn_RXBUF_SIZE", voi TX thi cung tuong tu.
  uint8_t data_buf[2048] = {0}; //Max size of buffer = 2048 (tested) 

  uint8_t connected_cf[3] = {'O', 'k', '\n'};
  uint8_t received_cf[5] = {'D', 'o', 'n', 'e', '\n'};
  uint32_t flashdestination;
  flashdestination = APPLICATION_ADDRESS;
  //Khoi tao cac thong so co ban MAC, IP, GW, SN,...
  W5500_Init();
  //Disconnet cho chac cu
  disconnect(socket_sn);
  //Khoi tao socket 0 voi thong so:
  //Mode: Sn_MR_TCP
  //Port: 2525
  server_init();
  //Neu ket noi thanh cong -> gui thong bao
  //Vao vong lap de xu ly tap tin  
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