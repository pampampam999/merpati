#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <TinyGPS++.h>
#include <string.h>


// Untuk SplitString 
#define MAX_TOKENS 10  // Jumlah maksimum token yang diharapkan

// Untuk doubletochar
char charValue[20]; // Adjust the array size according to your needs

//TMP button
int buttoncoba = D7;

// Setup GPS
SoftwareSerial gpsSerial(D1, D2);  // RX and TX , kebalikan dari remote
TinyGPSPlus gps;
// Global Variable GPS
float latitude = 0; 
float longitude = 0;
float altitude = 0;
String resultGPSAkhir = "";

// Setup Serial dari Arduino
SoftwareSerial arduino(D7,D8); //RX TX untuk komunikasi dari arduino

// Setup SSID dan Broker MQTT

const char* ssid = "Merpati";                              // SSID Merpati
const char* password = "123456789";                     // Password 123456789
const char* mqtt_server = "broker.mqtt-dashboard.com";

// Inisialisasi Variable Pendukung

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
char lokasi_akhir[] = "-3.333333,4.444444"; // angka 3 dan 4 digunakan sebagai penanda saja
char lokasi_awal[]= "-1.111111,-2.222222";  // angka 1 dan 2 digunakan sebagai penanda saja
char gabungan[50]; // Buffer untuk hasil gabungan gps
int kirim = 0;

// Variable pendukung komunikasi arduino
String data;
char c;


// Fungsi splitstring berdasarkan sparator
int splitString(const char *input, const char *delimiter, char **tokens) {
    char str[strlen(input) + 1];
    strcpy(str, input);

    int tokenCount = 0;
    char *token = strtok(str, delimiter);
    
    while (token != NULL && tokenCount < MAX_TOKENS) {
        tokens[tokenCount] = new char[strlen(token) + 1];
        strcpy(tokens[tokenCount], token);
        tokenCount++;
        token = strtok(NULL, delimiter);
    }

    return tokenCount;
}

void printTokens(char **tokens, int count) {
    for (int i = 0; i < count; i++) {
        Serial.println(tokens[i]);
        delete[] tokens[i]; // Clean up memory
    }
}

double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    // Radius bumi dalam kilometer
    double R = 6371000.0;

    // Ubah derajat menjadi radian
    double lat1_rad = lat1 * PI / 180.0;
    double lon1_rad = lon1 * PI / 180.0;
    double lat2_rad = lat2 * PI / 180.0;
    double lon2_rad = lon2 * PI / 180.0;

    // Perbedaan koordinat
    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;

    // Formula Haversine
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1_rad) * cos(lat2_rad) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // Hitung jarak
    double distance = R * c;
    return distance;
}


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

  // Lokasi awal di kosongkan
  //Serial.println(strlen(lokasi_awal));
  // for(int i=0;strlen(lokasi_awal);i++){
  //   lokasi_awal[i]=0;
  // }

  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    lokasi_awal[i]=(char)payload[i];
    Serial.print(lokasi_awal[i]);
  }
  Serial.println();

  //Serial.println(lokasi_awal);
  

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

// Fungsi print data GPS
void getGPS(){
  latitude = gps.location.lat();      // get data latitude
  longitude = gps.location.lng();     // get data longitude
  altitude = gps.altitude.meters();   // get data altitude

  char combinedString[50]; // Buffer untuk hasil penggabungan
  
  // Menggabungkan nilai latitude dan longitude menjadi satu dalam bentuk string
  dtostrf(latitude, 0, 6, combinedString);
  int length = strlen(combinedString);
  combinedString[length] = ',';
  dtostrf(longitude, 0, 6, combinedString + length + 1);


  //Serial.print("Gabungan dari long,lat akhir (Combined): ");      // Mengecek variabel local funtion
  Serial.println(combinedString); // Output hasil penggabungan
  
  //Copy ke lokasi _akhir
  strcpy(lokasi_akhir, combinedString);
  Serial.print("Gabungan dari long,lat akhir (lokasi_akhir): ");  // Mengecek variable global lokasi_akhir yang telah di copy
  Serial.println(lokasi_akhir); // Output hasil penggabungan


  Serial.print("Latitude: ");
  Serial.print(latitude, 6);
  Serial.print(", Longitude: ");
  Serial.print(longitude, 6);
  Serial.print(", Altitude: ");
  Serial.print(altitude);
  Serial.print(" meters");
  Serial.print(", Location Accuracy: ");
  Serial.print(gps.hdop.hdop());
  Serial.println(" meters");
}

char* doubleToChar(double value, int precision) {
  dtostrf(value, 0, precision, charValue);
  return charValue;
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);         // Initialize the BUILTIN_LED pin as an output
  
  Serial.begin(9600);                   // Serial begin
  arduino.begin(9600);                  // Serial dari arduino begin             
  gpsSerial.begin(9600);                //inisial gps serial , serial gps d1 d2

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //TMP Button
  pinMode(buttoncoba,INPUT_PULLUP);
}

void loop() {
  //Coba Button
  bool buttonPressed = digitalRead(buttoncoba);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Mengirimkan data lokasi awal remote dan lokasi akhir rumah merpati
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    if(kirim==1){
      lastMsg = now;
      ++value;
      
      // Menggabungkan lokasi_awal dan lokasi_akhir dengan koma di tengahnya
      for (int i = 0; i < sizeof(lokasi_awal) - 1; i++) {
        gabungan[i] = lokasi_awal[i];
      }

      // Menambahkan koma
      gabungan[sizeof(lokasi_awal) - 1] = ',';

      // Menambahkan lokasi_akhir setelah koma
      for (int i = sizeof(lokasi_awal); i < sizeof(gabungan); i++) {
        gabungan[i] = lokasi_akhir[i - sizeof(lokasi_awal)];
      }

      // Menambahkan karakter null-terminator pada akhir string
      gabungan[sizeof(gabungan) - 1] = '\0';

      snprintf (msg, MSG_BUFFER_SIZE, "%s",gabungan);
      
      Serial.print("Publish message [lokasi]: ");
      Serial.println(msg);
      client.publish("/pam/123/lokasi", msg);
    }
  }

  // Mengirimkan Perhitungan Jarak lokasi_awal dan lokasi_akhir dalam meter
  if(kirim==1){
    // Memisahkan variable gabungan di split dengan koma ke dalam variable token
    char delimiter[] = ",";     // Sparator
    char *tokens[MAX_TOKENS];   // Max berapa space yang di perlukan , sudah di set 10
    int tokenCount = splitString(gabungan, delimiter, tokens); 
    //printTokens(tokens, tokenCount); // untuk print hasil dari token

    // Merubah tokens menjadi double
    double lat1 = atof(tokens[0]);
    double lon1 = atof(tokens[1]);
    double lat2 = atof(tokens[2]);
    double lon2 = atof(tokens[3]);

    // Mencari dengan haverin distance
    double panjangTrack = haversine_distance(lat1,lon1,lat2,lon2); // Jarak dalam bentuk KM
    //Serial.print(panjangTrack); Serial.println(" Meter");
    Serial.print("Publish message [jarak Meter]: ");
    Serial.println(panjangTrack);
    char *charPanjangJarak = doubleToChar(panjangTrack, 5);


    // Publish ke dalam MQTT /pam/123/jarak
    client.publish("/pam/123/jarak",charPanjangJarak);

    kirim=0;  // merubah flag karena pengiriman data untuk di publis ke MQTT selesai
  }

  // Jika ada data dari arduino
  while(arduino.available()>0){ 
    c = arduino.read();
    data+=c;
  }

  // jika data dari arduino sudah selesai 1 baris
  
  if(data.length()>0){
    Serial.print(data);

    // Publish RFID ke Web
    Serial.print("Publish [RFID]: ");
    Serial.print(data);


    //Convert string to char
    const char* rfid = data.c_str();

    char modifiedArray[data.length() + 1];  // +1 for null-terminator
    char* modifiedPtr = modifiedArray;

    for (size_t i = 0; i < data.length(); ++i) {
        if (rfid[i] != '\n' && !isspace(static_cast<unsigned char>(rfid[i]))) {
            *modifiedPtr++ = rfid[i];
        }
    }
    *modifiedPtr = '\0';  // Null-terminate the modified string



    client.publish("/pam/123/datang", modifiedArray);
    data="";
    

    //publish ke MQTT datang

    
  }


  // Coba button , aktifkan jika ingin mengecek function getGPS() dengan triger D7 as button
  if(buttonPressed == LOW){
    getGPS();
    delay(500);
  }

  
   // Jika ada data dari GPS
  while (gpsSerial.available() > 0 /*&& lokasiPressed == true*/ )    // Jika menerima data dari GPS
  {
    //Serial.println("ada gps");
    // if(lokasiPressed == true){
    // }

    // Jika menerima data dari GPS dan saat butten lokasi di tekan
    if (gps.encode(gpsSerial.read())){ 
        if (gps.location.isValid()){                  // Jika data GPS Valid
          //int nowGPS = millis();                      // mencatat waktu ketika GPS valid
          //if(nowGPS - lastMsgGPS > 1000){             // menjalankan fungsi jika waktu lebih dari 1 detik dati terakhir kali di jalankan
          //  lastMsgGPS = nowGPS;                      // mecatat waktu terakhir di jalankan
            //digitalWrite(pin_led_lokasi,HIGH);        // Menyalakan LED lokasi(kuning) ketika sGPS Ready
            getGPS();
            //lokasiPressed = false;
            //digitalWrite(pin_led_terbang,!lokasiPressed);
          //}
        }
    }
    
  }  
}
