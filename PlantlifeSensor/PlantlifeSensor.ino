#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <AdafruitIO.h>
#include <AdafruitIO_WiFi.h>

#define IO_USERNAME  "pete_hodgson"
#define IO_KEY       "b330273ce401adec08d9c559a86348bbf5dc24da"
#define IO_SENSOR_FEED "plantlife-sensor"
#define IO_HEARTBEAT_FEED "plantlife-heartbeat"

AdafruitIO_WiFi *io;
AdafruitIO_Feed *sensorFeed;
AdafruitIO_Feed *heartbeatFeed;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(74800);

  Serial.println("Plantlife Sensor 1.0");

  // turn transmit power down to try and reduce overall current draw
  // max dBm is 20.5, min is 0
  WiFi.setOutputPower(10); 

  WiFiManager wifiManager;

  Serial.println("connecting to network...");
  if( !wifiManager.autoConnect("plantlife-sensor") ){
    Serial.println("WifiManager auto-connect failed");
    return;
  }

  Serial.println("connected to network:");

  Serial.println(WiFi.SSID());
  Serial.println(WiFi.psk());

  printMac();

  char ssid[WiFi.SSID().length()+1];
  WiFi.SSID().toCharArray(ssid,sizeof(ssid));

  char pass[WiFi.psk().length()+1];
  WiFi.psk().toCharArray(pass,sizeof(pass));

  Serial.println(ssid);
  Serial.println(pass);
  
  io = new AdafruitIO_WiFi(IO_USERNAME, IO_KEY, ssid, pass);
  sensorFeed = io->feed(IO_SENSOR_FEED);
  heartbeatFeed = io->feed(IO_HEARTBEAT_FEED);

  Serial.println("connected to IO...");
  io->connect();

  // feed->onMessage(handleMessage);

  // wait for a connection
  // while(io->status() < AIO_CONNECTED) {
  //   Serial.print(".");
  // }

  // we are connected
  Serial.println();
  Serial.println(io->statusText());
}

void printMac(){
  static byte mac[6];
  WiFi.macAddress(mac);

  Serial.print("WiFi MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}


void loop() {
  io->run();

  // if( debouncer.rose() ){
  //   handleButtonPress();
  // }
  
  // feed->save('slime');

  heartbeatFeed->save('ping');

  delay(1000);
}


void handleMessage(AdafruitIO_Data *data) {
  // TODO
}
