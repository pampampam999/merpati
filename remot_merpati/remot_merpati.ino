// library On The Air (OTA)
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

// library On The Air (OTA)
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// Library wifi manager
#include <ESPAsyncWiFiManager.h> 

//
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Setup SSID (Manual) dan Broker MQTT
const char* ssid = "DINI";
const char* password = "asdfghjkl";
const char* mqtt_server = "broker.mqtt-dashboard.com";

// Setup GPS
SoftwareSerial gpsSerial(D2, D1); // RX, TX
TinyGPSPlus gps;

//Setup AsyncWebServer for OTA
AsyncWebServer server(80);
DNSServer dns;

// Inisialisasi Variable Pendukung

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
unsigned long lastMsg1 = 0;
unsigned long lastMsgLokasi = 0;
unsigned long lastMsgTerbang = 0;
unsigned long lastMsgGPS = 0;
unsigned long lastMsgWaktuTerbang = 0;

#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
bool buttonLokasi = false;
bool buttonTerbang = false;

// Global Variable
//char lokasi_awal[] = "-7.277170,-7.277170"; 
float latitude = 0;
float longitude = 0;
float altitude = 0;

// Button lokasi dan start_terbang
const int btn_lokasi = D5;
const int btn_terbang = D6;

// Led Lokasi dan Start
const int pin_led_lokasi = D3;
const int pin_led_terbang = D4;
bool ledLokasi = false;
bool ledStart = false;

// Flag button
bool lokasiPressed = false;
bool terbangPressed = false;

// Fungsi koneksi ke WIFI - menggunakan portal
void setup_wifimanager(){
  AsyncWiFiManager wifiManager(&server,&dns);

  // Digunakan untuk reet configurasi
  //wifiManager.resetSettings();
  
  bool res;
  // res = wifiManager.autoConnect(); // Auto generated AP name from chipid, anonymous
  // res = wifiManager.autoConnect("AutoConnectAP"); // Anonymous AP
  res = wifiManager.autoConnect("RemoteMerpatiAP","12345678");

  if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
}

// Fungsi koneksi ke WIFI - manual
// void setup_wifi() {

//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   randomSeed(micros());

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// Fungsi ketika ada feedback dari MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
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

}

//Fungsi reconnect ketika putus dengan Broker
void reconnect() {
  // Loop until we're reconnected
  int jmlReconnect = 1; int batasReconnect = 4;
  bool setWifi =false;
  while (!client.connected() && jmlReconnect <= batasReconnect) { // 1 kali reconnect 5 detik
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" try=");
      Serial.print(jmlReconnect);
      Serial.println(", try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      jmlReconnect++;
      if(jmlReconnect >= batasReconnect+1){
        setWifi = true;
      }
    }
  }
  
  jmlReconnect=0; // Jumlah reconnect di reset karena sudah connect
  if(setWifi == true){
    setup_wifimanager();
    setWifi == false;
  }
}

// Fungsi print data GPS
void getGPS(){
  latitude = gps.location.lat();      // get data latitude
  longitude = gps.location.lng();     // get data longitude
  altitude = gps.altitude.meters();   // get data altitude

  Serial.print("Latitude: ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" meters");
  Serial.print("Location Accuracy: ");
  Serial.print(gps.hdop.hdop());
  Serial.println(" meters");
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);         // inisial BUILTIN_LED sebagai output
  pinMode(btn_lokasi, INPUT_PULLUP);    // Inisial pin Button kiri / Lokasi dengan input secara default nilai HIGH jika tidak di tekan atau (Aktif Low)
  pinMode(btn_terbang, INPUT_PULLUP);   // Inisial pin Button kanan / Star menerbangkan dengan input secara default nilai HIGH jika tidak di tekan atau (Aktif Low)
  pinMode(pin_led_lokasi, OUTPUT);      // inisial pin led lokasi (kuning) sebagai output
  pinMode(pin_led_terbang, OUTPUT);     // inisial pin led terbang (hijau) sebagai output

  Serial.begin(9600);    // insial bautrate serial                   
  gpsSerial.begin(9600); //inisial gps serial , serial gps d1 d2

  // memilih mode connect pada wifi true=menggunakan wifi manager , false=manual koneksi secara langsung(setting di atas)
  bool wifiManagerEnable = true;
  if(wifiManagerEnable == true){
    setup_wifimanager();
  }else{
    //setup_wifi();
  }

  // MQTT
  client.setServer(mqtt_server, 1883);  // set port mqtt
  client.setCallback(callback);         // set jika ada response pada fungsi callback

  //OTA Setup - simple response
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  // OTA Start
  AsyncElegantOTA.begin(&server);           // Start AsyncElegantOTA
  
  Serial.println("HTTP server started");
  
  // Start server
  server.begin();                           
  
  digitalWrite(pin_led_lokasi,LOW);
  digitalWrite(pin_led_terbang,HIGH);
}

void loop() {
  // loop() harus di inputkannuntuk selalu reading jika ada update dari On The Air
  AsyncElegantOTA.loop();
  
  buttonLokasi = digitalRead(btn_lokasi);     //  Membaca status button lokasi
  buttonTerbang = digitalRead(btn_terbang);   //  Membaca status button start

//  if(ledLokasi == false){
//    ledLokasi = !ledLokasi;
//    ledStart = !ledStart;
//  }else{
//    ledLokasi = !ledLokasi;
//    ledStart = !ledStart;
//  }
//  digitalWrite(pin_led_lokasi,ledLokasi);
//  digitalWrite(pin_led_start,ledStart);
//  delay(1000);  

  // Jika button terbang di tekan
  if(buttonTerbang == LOW){
      Serial.println("Terbang Pressed");
      terbangPressed = true;                // Memberikan flag button telah di tekan yang akan di proses di if berikutnya
  }

  // Jika button lokasi di tekan 1
  if(buttonLokasi == LOW){
      Serial.println("Lokasi Pressed"); 
      lokasiPressed = true;                 // Memberikan flag button telah di tekan yang akan di proses di if berikutnya
  }

  // Jika lokasi di tekan 2
  unsigned long nowLokasi = millis();                               // mengambil waktu sekarang (format waktu di mulai dari menyalanya hardware)
  if(nowLokasi - lastMsgLokasi > 1000 && lokasiPressed == true){    // Jika lokasi di tekan , dan memberikan debouncing dengan waktu, Jika jarak antara terakhir kali button di tekan dengan sekarang lebih dari 1 detik
    lastMsgLokasi = nowLokasi;                                      // memasukkan waktu sekarang sebagai waktu terakhir di tekan
    Serial.println("Menjalankan isi lokasi");
    digitalWrite(pin_led_lokasi,HIGH);                              
  }

  // Jika terbang di tekan 2
  unsigned long nowTerbang = millis();                               // mengambil waktu sekarang (format waktu di mulai dari menyalanya hardware)
  if(nowTerbang - lastMsgTerbang > 1000 && terbangPressed == true){    // Jika lokasi di tekan , dan memberikan debouncing dengan waktu, Jika jarak antara terakhir kali button di tekan dengan sekarang lebih dari 1 detik
    lastMsgTerbang = nowTerbang;                                      // memasukkan waktu sekarang sebagai waktu terakhir di tekan
    Serial.println("Menjalankan isi terbang");
    digitalWrite(pin_led_lokasi,LOW);
    terbangPressed = false;                              
  }

 

  // unsigned long now1= millis();
  // if(now1 - lastMsg1 > 5000){
  //   //lokasiPressed =true;
  //   lastMsg1 = now1;
  // }

  // Jika MQTT disconnect
  if (!client.connected()) {
    reconnect();              // Memanggil fungsi reconnect
  }
  client.loop();              // Loop untuk memonitoring

  // GPS
  while (gpsSerial.available() > 0 /*&& lokasiPressed == true*/)    // Jika menerima data dari GPS
  {
    
    // if(lokasiPressed == true){
    // }

    // Jika menerima data dari GPS dan saat butten lokasi di tekan
    if (gps.encode(gpsSerial.read())){
        if (gps.location.isValid()){                  // Jika data GPS Valid
          //int nowGPS = millis();                      // mencatat waktu ketika GPS valid
          //if(nowGPS - lastMsgGPS > 1000){             // menjalankan fungsi jika waktu lebih dari 1 detik dati terakhir kali di jalankan
          //  lastMsgGPS = nowGPS;                      // mecatat waktu terakhir di jalankan
            digitalWrite(pin_led_lokasi,HIGH);        // Menyalakan LED lokasi(kuning) ketika sGPS Ready
            getGPS();
            //lokasiPressed = false;
            //digitalWrite(pin_led_terbang,!lokasiPressed);
          //}
        }
    }
    
  }

   //ketika lokasi di tekan 3
  unsigned long now = millis();
  if (now - lastMsg > 1000 && lokasiPressed==true) {
    lastMsg = now;
    ++value;
    //snprintf (msg, MSG_BUFFER_SIZE, "%s",lokasi_awal);
    snprintf (msg, MSG_BUFFER_SIZE, "%f,%f",latitude,longitude);
    Serial.print("Publish message1: ");
    Serial.println(msg);
    client.publish("/pam/123/lokasi_awal", msg);
    lokasiPressed = false;
  }

  //ketika terbang di tekan 3
  unsigned long nowWaktuTerbang = millis();
  if (nowWaktuTerbang - lastMsgWaktuTerbang > 1000 && terbangPressed==true) {
    lastMsgWaktuTerbang = nowWaktuTerbang;
    ++value;
    //snprintf (msg, MSG_BUFFER_SIZE, "%s",lokasi_awal);
    snprintf (msg, MSG_BUFFER_SIZE, "1");
    Serial.print("Publish message1: ");
    Serial.println(msg);
    client.publish("/pam/123/waktu_matchup", msg);
    terbangPressed = false;
  }


}
