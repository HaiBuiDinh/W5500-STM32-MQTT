#include <stdio.h>
#include "MQTTClient.h"
#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <sys/time.h>

volatile int toStop = 0;

void usage(char** argv)
{
    printf("Usage: %s topic channel_token\n", argv[0]);
    exit(-1);
}

void cfinish(int sig)
{
    signal(SIGINT, NULL);
    toStop = 1;
}

int main(int argc, char** argv)
{
    int rc = 0;
    unsigned char buf[100];
    unsigned char readbuf[100];

    if (argc < 3)
        usage(argv);

    char* host = "mqtt.beebotte.com";
    int port = 1883;
    char* topic = argv[1];
    char* clientid = "myid";
    char* username = argv[2];

    Network n;
    Client c;

    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);

    NewNetwork(&n);
    ConnectNetwork(&n, host, port);
    MQTTClient(&c, &n, 1000, buf, 100, readbuf, 100);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = clientid;
    data.username.cstring = username;
    //data.password.cstring = opts.password;

    data.keepAliveInterval = 10;
    data.cleansession = 1;
    printf("Connecting to %s %d\n", host, port);

    rc = MQTTConnect(&c, &data);
    printf("Connected %d\n", rc);

    printf("Publishing to %s\n", topic);

    char payload[100];
    sprintf(payload, "Hello World - youppi!");
    MQTTMessage pubmsg;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(payload) + 1;
    pubmsg.qos = QOS0;
    pubmsg.retained = 0;
    rc = MQTTPublish (&c, topic, &pubmsg);

    while (!toStop)
    {
        MQTTYield(&c, 1000);
        rc = MQTTPublish (&c, topic, &pubmsg);
    }

    printf("Stopping\n");

    MQTTDisconnect(&c);
    n.disconnect(&n);

    return 0;
}
