#include "config.h"
#include <WiFi.h>
#ifdef ENABLE_NTP
#include <NtpClientLib.h>
#endif
#ifdef ENABLE_MQTT
#include <PubSubClient.h>
#endif
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
long nextframe;
int mode;

#include <FS.h>

#include "SPIFFS.h"  // For ESP32 only
#include <TFT_eSPI.h> // Hardware-specific library for ESP8266
TFT_eSPI tft = TFT_eSPI();
#include "FreeSans36.h"
#include "FreeFonts.h"
#include "qrcode.h"            // Copyright (c) //https://github.com/ricmoo/qrcode/
#include "HX711.h"
HX711 scale;



#include "scale.h"
#include "BMP_functions.h"
#include "bier.h"

#include "wifitools.h"

#ifdef ENABLE_MQTT
#include "mqtt.h"
long mqtt_lastry;
#endif



#ifdef ENABLE_WEBSERVER
WebServer server(WEB_SERVER_PORT);

const char PTYPE_HTML[] = "text/html";
const char PTYPE_PLAIN[] = "text/plain";
#include "page_header.h"


void sendPage(const char *data, int count, const char *type) {
  yield();
  int szHeader = sizeof(PAGE_HEADER);
  char *buffer = (char*)malloc(count + szHeader);
  if (buffer) {
    memcpy_P(buffer, PAGE_HEADER, szHeader);
    memcpy_P(buffer + szHeader - 1, data, count);   /* back up over the null byte from the header string */
    server.send(200, type, buffer);
    free(buffer);
  } else {
    Serial.print(F("*** Malloc failed for "));
    Serial.print(count);
    Serial.println(F(" bytes in sendPage() ***"));
  }
}



#include "p_index.h"
#include "p_bier.h"
#include "p_fill.h"

#ifdef ENABLE_WEBUPDATE
#include "webupdate.h"
#endif
#define FS_NO_GLOBALS



void setupWebServer() {
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", p_index);
  }); 
  server.on("/keg.html", handle_keg_request);
  server.on("/access.html", handle_access_request);
  server.on("/nfc.html", handle_nfc);

  server.on("/filldata.html", handle_filldata_request);
  server.on("/scripts/filldata", send_filldata);
  server.on("/scripts/nfcdata", send_nfc_data);
  

  #ifdef ENABLE_WEBUPDATE
  setupWebUpdate();
  #endif 
  server.begin();
}
#endif


void loopIPServices() {
  static long wifi_last_try = millis();
  static long mqtt_last_check = millis();
  if(WiFi.status() != WL_CONNECTED && millis() - wifi_last_try > WIFI_RETRY ) {
    wifi_last_try = millis();
    Serial.println("Wifi connection lost, trying to reconnect");
    WiFi.reconnect();
  }
  //Update Server

  #ifdef ENABLE_WEBSERVER
  //Webserver: enables Webupdate Service
  server.handleClient();
  #endif

  
  //MQTT 
  #ifdef ENABLE_MQTT
  if (!client.connected() && millis() - mqtt_last_check > MQTT_RETRY && WiFi.status() == WL_CONNECTED) {
    mqtt_last_check = millis();
    connect_mqtt();
  }
  if(client.connected()) {
    client.loop();
    mqtt_lastry = millis();
  }
  #endif
  #ifdef ENABLE_NTP
  if (wifiFirstConnected) {
    wifiFirstConnected = false;
    NTP.setInterval (NTP_INTERVAL);
    NTP.setNTPTimeout (NTP_TIMEOUT);
    NTP.begin (ntpServer, timeZone, true, minutesTimeZone); //begin only when Wifi is connected
  }

  if (syncEventTriggered) {
    processSyncEvent (ntpEvent);
    syncEventTriggered = false;
  }
  #ifdef PRINT_TIME
  static int next_ntp_print = 0;
  if(millis() > next_ntp_print) {
    Serial.print (NTP.getTimeDateString ()); Serial.print (" ");
    Serial.println (NTP.isSummerTime () ? "Summer Time. " : "Winter Time. ");
    next_ntp_print = millis() + PRINT_TIME;
  }
  #endif
  #endif

  
}

#include "menu.h"

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS initialised.");
  scale.begin(HX711_dout, HX711_sck);
  tft.begin();
  tft.setRotation(3);  // 0 & 2 Portrait. 1 & 3 landscape
  tft.fillScreen(TFT_BLACK);
  mode = 0;
  //init the keg with standard values
  keg.id = 1;
  snprintf(keg.name, sizeof(keg.name), "Standard Bier");
  snprintf(keg.filldate, sizeof(keg.filldate), "%d.%d.%d", day(now()), month(now()), year(now()));
  keg.full = 0.0;
  keg.max = 0.0;
  keg.tara = 0.0;
  keg.dichte = 1.007;
  init_menu();
  set_active_entry(0);
  LoadMenuById("0");

  



  // Now initialise the TFT

}

void loop() {
  loopIPServices(); //Poll all the IP Services you want to run in this sketch
  scale_tick();
  menu_tick();
  menu_animation(); //show animation of the menu
}
