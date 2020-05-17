WiFiClient espClient;
PubSubClient client(espClient);

char mqttbuffer[256] = ""; 

//Callback function for incomming mqtt messages
void callback_new(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  char* token_payload;
  for(int i = 0; i < length; i++) {
    mqttbuffer[i] = payload[i];
  }
  mqttbuffer[length] = '\0';
  Serial.println(mqttbuffer);
}

//subscribe to the channels you want to listen to
void subscribeChannels() {
  //client.subscribe(REPLACE WITH CHANNEL NAME);
}

void connect_mqtt() {
  Serial.println("MQTT INFO: Attempting MQTT connection...");
  if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
    Serial.println("MQTT INFO: connected");
    subscribeChannels();
  }
  else {
    Serial.print("ERROR: failed, rc=");
    Serial.print(client.state());
    Serial.print("DEBUG: try again in ");Serial.print(MQTT_RETRY/1000);Serial.println(" seconds");
  }
}
