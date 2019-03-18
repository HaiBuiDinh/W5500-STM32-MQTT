#include "client.h"
#include "socket.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void client_init(void)
{
  //define input parameters
  uint8_t  client_sn = 0;
  uint8_t  client_protocol = Sn_MR_TCP;
  uint16_t client_port = 8080;
  uint8_t  client_flag = 0;
  
  //data send
  uint8_t *data_send = "Hello";
  //return of  socket
  int8_t client_result;
  int8_t check_connect;
  uint8_t server_IP[4] = {192, 168, 32, 118};
  uint16_t server_port = 8080;
  
  //return of send
  int32_t send_result;
  
  client_result = socket(client_sn, client_protocol, client_port, client_flag);
  
  //check initialzation process
  if (client_result != 0)
  {
    printf("\r\n ---- ");
  }
  
  //connecting to server
  //int8_t connect(uint8_t sn, uint8_t * addr, uint16_t port)
  check_connect=connect(client_sn, server_IP, server_port);
  if (check_connect != SOCK_OK)
  {
    printf("\r\n  ---- ");
  }
  
  //if connect ok, send data
  send_result = send(client_sn, data_send, 1);
  
  if (send_result == 0)
  {
  printf("\r\n ---- ");
  }
  
  disconnect(client_sn);
  close(client_sn);
  
  
}