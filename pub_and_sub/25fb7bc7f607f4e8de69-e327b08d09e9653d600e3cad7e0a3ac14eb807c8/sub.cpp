#include "MQTTClient.h"
#include "linux.cpp"

int arrivedcount = 0;

void usage(char** argv)
{
    printf("Usage: %s topic channel_token\n", argv[0]);
    exit(-1);
}

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;

    printf("Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n",
        ++arrivedcount, message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\n", message.payloadlen, (char*)message.payload);
}


int main(int argc, char* argv[])
{
    IPStack ipstack = IPStack();
    float version = 0.3;

    if (argc < 3)
        usage(argv);

    printf("Version is %f\n", version);

    MQTT::Client<IPStack, Countdown> client = MQTT::Client<IPStack, Countdown>(ipstack);

    const char* hostname = "mqtt.beebotte.com";

    int port = 1883;
    printf("Connecting to %s:%d\n", hostname, port);
    int rc = ipstack.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\n", rc);

    printf("MQTT connecting\n");

    const char* topic = argv[1];
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.username.cstring = (char*)argv[2];
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"myid";
    rc = client.connect(data);
    if (rc != 0)
        printf("rc from MQTT connect is %d\n", rc);
    printf("MQTT connected\n");

    rc = client.subscribe(topic, MQTT::QOS1, messageArrived);
    if (rc != 0)
        printf("rc from MQTT subscribe is %d\n", rc);

    while (1) {
        client.yield(5000);
    }

    rc = client.unsubscribe(topic);
    if (rc != 0)
        printf("rc from unsubscribe was %d\n", rc);

    rc = client.disconnect();
    if (rc != 0)
        printf("rc from disconnect was %d\n", rc);

    ipstack.disconnect();

    printf("Finishing with %d messages received\n", arrivedcount);

    return 0;
}

