#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "1010";
const char* password = "kocopass";

const int pResistor = A0;

String lightInt_str;
char lightInt[50];

const char* mqtt_server = "172.20.10.9";

WiFiClient vanieriot;
PubSubClient client(vanieriot);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP-8266 IP address: ");
  Serial.println(WiFi.localIP());
  
}
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messagein;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messagein += (char)message[i];
  }

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("vanieriot")) {
      Serial.println("connected");
    
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      // Wait 5 seconds before retrying
      delay(3000);
    }
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(pResistor, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  int lightIntVal = analogRead(pResistor);
  lightInt_str = String(lightIntVal);
  lightInt_str.toCharArray(lightInt, lightInt_str.length() + 1);
  
  
  if (!client.connected()) {
    reconnect();
  }
  if(client.loop())
    
    client.connect("vanieriot");
    client.publish("ESP/pResistor", lightInt);
  
  delay(1000);
}
