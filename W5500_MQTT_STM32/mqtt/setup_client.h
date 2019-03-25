#ifndef _SETUP_CLIENT_H_
#define _SETUP_CLIENT_H_

typedef struct Timer Timer;

struct Timer {
  unsigned long systick_period;
  unsigned long end_time;
};

typedef struct Network Network;

struct Network
{
  int my_socket;
  int (*mqttread) (Network*, unsigned char*, int, int);
  int (*mqttwrite) (Network*, unsigned char*, int, int);
  void (*disconnect) (Network*);
};

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer* timer, unsigned int);
void TimerCountdown(Timer* timer, unsigned int);
int TimerLeftMS(Timer*);

int w5500_read(Network*, unsigned char*, int, int);
int w5500_write(Network*, unsigned char*, int, int);
void w5500_disconnect(Network*);
void NewNetwork(Network*);
 
#endif
