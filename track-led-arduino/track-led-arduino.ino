// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).

#include <Adafruit_NeoPixel.h>

#include <WiFiNINA.h> 
#include <PubSubClient.h>
#include "credentials.h"

#define PIN      6
#define N_LEDS  50

const char* ssid = WIFI_NETWORK;
const char* password = WIFI_PASSWORD;
const char* mqttServer = MQTT_SERVER;

char subTopic[] = "test/track/led";

char *args[3];
char *ptr = NULL;
int cur_i;
int cur_tail;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GBR + NEO_KHZ800);

void setup_wifi() 
{
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

/*
 * payload structure is 20 bytes. first four are time per lap (duration),
 * then number of laps, then green, blue, red
 * and by first four, I mean first - so we want
 * [0], [4], etc
 * probably will adjust payload - want to include a lot more options
 */
void callback(char* topic, byte* payload, unsigned int length) 
{
  /*
  char paychar[length];// = (char*)payload;
  for (int i=0;i<length;i++) {
    Serial.println(payload[i]);
    Serial.println(i);
    Serial.println("--");
    paychar[i] = (char)payload[i];
  }
  //Serial.println(paychar);
  byte index = 0;
  ptr = strtok(paychar, ";");
  while(ptr != NULL) {
    args[index] = ptr;
    index++;
    ptr = strtok(NULL, ";");
  }
  */
  Serial.println("here");
  chase(strip.Color(payload[8], payload[12], payload[16]), payload[0]*1000, payload[4], 3); // Red *1000 since we want to send seconds but arg is ms
  Serial.println("finished");
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    // Create a random client ID
    String clientId = "ArduinoClient-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) 
    {
      client.subscribe(subTopic);
    } else 
    {
      delay(5000);
    }
  }
}



void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
  strip.clear();
  strip.begin();
}

void loop() {
  //delay(2000);
  // put your main code here, to run repeatedly:
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}

// just add duration as a second variable
// and delay duration/NUM_LEDS
static void chase(uint32_t c, int duration, int laps, int del) {
  for (uint16_t i = 0; i < N_LEDS*laps + del; i++) {
    cur_i = i%N_LEDS;
    cur_tail = (i-del)%N_LEDS;
    //Serial.println(i);
    //overlap is great but the last lap shouldn't have any overlapped lights in the next section
    if (i < N_LEDS*laps) {
        strip.setPixelColor(cur_i, c);
    }
    strip.setPixelColor(cur_tail, 0, 0, 0);
    strip.show();
    delay(duration/N_LEDS);
  }
  strip.clear();
  strip.show();
  /*for (uint16_t j = 0; j < laps; j++) {
    for (uint16_t i = 0; i < N_LEDS + del; i++) {
      strip.setPixelColor(i, c); // Draw new pixel
      strip.setPixelColor(i - del, 0, 0, 0); // Erase pixel a few steps back
      strip.show();
      delay(duration/N_LEDS);
    }
  }*/
}
