#include "setup_client.h"
#include "stm32f4xx_it.h"
#include "wizchip_conf.h"
#include "socket.h"
unsigned long MilliTimer;

/*
void SysTick_Handler(void) {
	MilliTimer++;
}
*/

void TimerInit(Timer* timer)
{
  timer->end_time = 0;
}

char TimerIsExpired(Timer* timer)
{
  long left =  timer->end_time - MilliTimer;
  
  return (left <0);
}

void TimerCountdownMS(Timer* timer, unsigned int timeout)
{
  timer->end_time =  MilliTimer + timeout;
}

void TimerCountdown(Timer* timer, unsigned int timeout)
{
  timer->end_time =  MilliTimer + (timeout * 1000);
}

int TimerLeftMS(Timer* timer)
{
  long left = timer->end_time - MilliTimer;
  return (left < 0) ? 0 : left;
}

void NewNetwork(Network* n) {
	n->my_socket = 0;
	n->mqttread = w5500_read;
	n->mqttwrite = w5500_write;
	n->disconnect = w5500_disconnect;
}

int w5500_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
  if((getSn_SR(n->my_socket) == SOCK_ESTABLISHED) && (getSn_RX_RSR(n->my_socket)>0))
  {
  return recv(n->my_socket, buffer, len);
  }
  return 0;
}

int w5500_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
  if(getSn_SR(n->my_socket) == SOCK_ESTABLISHED)
  {
  return send(n->my_socket, buffer, len);
  }
  return 0;
}

void w5500_disconnect(Network* n)
{
  disconnect(n->my_socket);
}