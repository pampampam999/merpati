#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial RFID(D5, D6);  // RX and TX
SoftwareSerial arduino(D7,D8); //RX TX untuk komunikasi dari arduino

// Setup SSID dan Broker MQTT

const char* ssid = "DINI";
const char* password = "asdfghjkl";
const char* mqtt_server = "broker.mqtt-dashboard.com";

// Inisialisasi Variable Pendukung

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
char lokasi_akhir[] = "-7.277170,-7.277170";
char lokasi_awal[]= "-1.111111,-1.111111";
int kirim = 0;

// Variable pendukung komunikasi arduino
String data;
char c;


// Fungsi koneksi ke WIFI
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Fungsi ketika ada feedback dari MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    lokasi_awal[i]=(char)payload[i];
    Serial.print(lokasi_awal[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

  // Jika dapat data lokasi awal dari remot
  kirim =1;

}

//Fungsi reconnect ketika putus dengan Broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("/pam/123/lokasi_awal");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);         // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);                   // Serial begin
  arduino.begin(9600);                  // Serial dari arduino begin             
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    if(kirim==1){
      lastMsg = now;
      ++value;
      snprintf (msg, MSG_BUFFER_SIZE, "%s,%s",lokasi_awal,lokasi_akhir);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("/pam/123/lokasi1", msg);
      kirim=0;
    }
  }

  // Jika ada data dari arduino
  while(arduino.available()>0){ 
    c = arduino.read();
    data+=c;
  }

  // jika data dari arduino sudah selesai 1 baris
  if(data.length()>0){
    Serial.print(data);
    data="";
  }  
}
