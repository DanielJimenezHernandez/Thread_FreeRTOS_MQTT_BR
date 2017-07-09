#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

void MQTTConnectUDP(int udpPort, char * udpAddr);
void MQTTUdpSend(uint8_t *pdata, uint8_t datalen);

#endif /*MQTTCLIENT_H*/
