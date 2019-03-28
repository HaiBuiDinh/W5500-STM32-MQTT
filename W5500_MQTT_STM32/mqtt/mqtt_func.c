#include "mqtt_func.h"

/* Include MQTT library */
#include "mqtt_interface.h"
#include "MQTTClient.h"

#define TCP_SOCKET      0
#define BUFFER_SIZE	2048

unsigned char tempBuffer[BUFFER_SIZE] = {0};

unsigned char targetIP[4] = {192,168,1,234};
unsigned int targetPort = 1883;

struct opts_struct
{
	char* clientid;
	int nodelimiter;
	char* delimiter;
	enum QoS qos;
	char* username;
	char* password;
	char* host;
	int port;
	int showtopics;
} opts =
{ 
  (char*)"subscriber", 0, (char*)"\n", QOS0, "admin", "admin123", (char*)"localhost", 1883, 0 
};

void ConnectToServer (void)
{
    int rc = 0;
    MQTTClient c;
    Network n;
    
    NewNetwork(&n, TCP_SOCKET);
    ConnectNetwork(&n, targetIP, targetPort);
    
    unsigned char buf[100];
    MQTTClientInit(&c, &n, 1000, buf, 100, tempBuffer, 2048);
    
    MQTTPacket_connectData data =  MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    
    data.clientID.cstring = opts.clientid;
    data.username.cstring = opts.username;
    data.password.cstring = opts.password;
    
    data.keepAliveInterval = 60;
    data.cleansession = 1;
    
    rc = MQTTConnect(&c, &data);
    printf("Connected %d\rr\n", rc); //Co the bo qua dong nay
    opts.showtopics = 1;
    
    char payload[100];
    sprintf(payload, "Hello huhu");
    
    MQTTMessage msg;
    msg.payload = payload;
    msg.payloadlen = strlen(payload) + 1;
    msg.qos = QOS0;
    msg.retained = 0;
    
    rc = MQTTPublish(&c, "abc", &msg);
    
    while(1)
    {
      MQTTYield(&c, data.keepAliveInterval);
    }

}