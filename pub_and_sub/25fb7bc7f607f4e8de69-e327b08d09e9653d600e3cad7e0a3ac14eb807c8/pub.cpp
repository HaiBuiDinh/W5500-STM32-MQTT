#include "MQTTClient.h"
#include "linux.cpp"

void usage(char** argv)
{
    printf("Usage: %s topic channel_token\n", argv[0]);
    exit(-1);
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
    const char* topic = argv[1];
    printf("Connecting to %s:%d\n", hostname, port);
    int rc = ipstack.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\n", rc);

    printf("MQTT connecting\n");
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"myid";
    data.username.cstring = argv[2];
    rc = client.connect(data);
    if (rc != 0)
        printf("rc from MQTT connect is %d\n", rc);
    printf("MQTT connected\n");


    MQTT::Message message;

    char buf[100];
    sprintf(buf, "Hello World!");
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;

    rc = client.publish(topic, message);
    if (rc != 0)
        printf("Error %d from sending QoS 2 message\n", rc);

    rc = client.disconnect();
    if (rc != 0)
        printf("rc from disconnect was %d\n", rc);

    ipstack.disconnect();

    return 0;
}
