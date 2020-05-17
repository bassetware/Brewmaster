#include "credentials.h"
#define SW_VERSION  "0.1.0"
#define MASTER_NAME "Brewmaster"

//Wifi Section
const char* WIFI_SSID = SECRET_WIFI_SSID;
const char* WIFI_PASSWORD = SECRET_WIFI_PASSWORD;
#define WIFI_RETRY 5000

//Webupdate section
#define ENABLE_WEBUPDATE_SERVICE

//MQTT Section
//#define ENABLE_MQTT //comment out for disabling MQTT service
const PROGMEM char* MQTT_CLIENT_ID = "skeleton";
const PROGMEM char* MQTT_SERVER_IP = "192.168.178.2";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = SECRET_MQTT_USER;
const PROGMEM char* MQTT_PASSWORD = SECRET_MQTT_PASSWORD;
#define MQTT_RETRY 2000

//NTP section
#define ENABLE_NTP //comment for disabling NTP Time service
int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
#define NTP_INTERVAL 63 //Seconds between two sync events
const PROGMEM char *ntpServer = "pool.ntp.org";
#define NTP_TIMEOUT 1500
//#define PRINT_TIME 5000

//Webserver section
#define ENABLE_WEBSERVER
#define WEB_SERVER_PORT 80

//Webupdate section
#define ENABLE_WEBUPDATE
#define UPDATE_URL "/update"

//AP Section
const char *ap_ssid = "brewmaster";
const char *ap_password = "brewmaster_pw";

#define SCREEN_W 480
#define SCREEN_H 320

const int HX711_dout = 12; //mcu > HX711 dout pin
const int HX711_sck = 13; //mcu > HX711 sck pin
float scale_m = 10.44944838; //Calibration Value slope
float scale_b = -229.6466696; //Calibration Value y-intercept
#define SCALE_INTERVAL 100 // take reading every 50 MQTT_SERVER_IP
#define SCALE_RINGCOUNT 20
long scale_nextreading;
long scale_ringbuffer[SCALE_RINGCOUNT];
int scale_ring_pos = 0;
long scale_raw;
long scale_tare;
#define STD_TARE_VAL 511661
#define KEG_DETECT_VAL 500 //grams


const char *keg_password = "kegbrew";
boolean fillaccess = false;
boolean scaned = false;
boolean client_connected = false;
#define B1 32
#define B2 33
#define B3 25
#define B4 26