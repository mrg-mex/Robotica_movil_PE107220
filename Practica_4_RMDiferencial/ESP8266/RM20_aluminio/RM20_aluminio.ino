#include <ESP8266WiFi.h>  //libreria wifi espino
#include <WiFiUDP.h>      //libreria UDP
#include <Wire.h>         //Libreria I2C

//**************************Código para conexión WIFI y UDP*************************
// wifi connection variables
const char* ssid = "MiRed";
const char* password = "123456789"; 
boolean wifiConnected = false;
byte a=128, b=128;

// UDP variables
unsigned int localPort = 8080;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[255]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back
//**********************************************************************************

//**************************Código para I2C*****************************************
#define CMD                 (byte)0x00                        // Values of 0 eing sent using write have to be cast as a byte to stop them being misinterperted as NULL
                                                              // This is a but with arduino 1
#define MD25ADDRESS         0x58                             // Address of the MD25(ATACANTE)
#define SOFTWAREREG         0x0D                              // Byte to read the software version
#define SPEED1              (byte)0x00                        // Byte to send speed to first motor
#define SPEED2              0x01                              // Byte to send speed to second motor
#define ENCODERONE          0x02                              // Byte to read motor encoder 1
#define ENCODERTWO          0x06                              // Byte to read motor encoder 2
#define VOLTREAD            0x0A                              // Byte to read battery volts
#define RESETENCODERS       0x20                              // Byte to Reset encoders
//**********************************************************************************


void setup() {
  // Initialise Serial connection
  Serial.begin(115200);
  Wire.begin();
  // Initialise wifi connection
  wifiConnected = connectWifi();
  // only proceed if wifi connection successful
  if (wifiConnected) 
    udpConnected = connectUDP();
  
}

void loop() {
  // check if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {

      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize)
      {
        int len = UDP.read(packetBuffer,255);
        if(len>0)
      {
        packetBuffer[len]=0;
        
        a = packetBuffer[0];
        b = packetBuffer[1];
        
        Serial.print("a: ");
        Serial.print(a);
        Serial.print("   b: ");
        Serial.println(b);  
      }
      }
      
      //delay(10);
    }
   }
     
    Wire.beginTransmission(MD25ADDRESS);                    // Drive motor 2 at speed value stored in x
    Wire.write(SPEED2);
    Wire.write(a);                                           
    Wire.endTransmission();
 
   Wire.beginTransmission(MD25ADDRESS);                    // Drive motor 2 at speed value stored in x
    Wire.write(SPEED1);
    Wire.write(b);                                           
    Wire.endTransmission();
}

// connect to UDP – returns true if successful or false if not
boolean connectUDP() {
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }

  return state;
}
// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;

  WiFi.begin(ssid, password);/*
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }*/
  return state;
}
