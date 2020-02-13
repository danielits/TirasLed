#include <FastLED.h>

#define NUM_LEDS 159
#define DATA_PIN 5 //D5

byte leftColOn = 0;
byte rightColOn = 0;
byte leftCol = 46;
byte rightCol = 46;


byte varOn = 1;
byte varCols = 1;
byte varEfecto = 0;

byte varBright = 1;
byte varSpeed = 10;
byte varSpeedColor = 30;
byte varWidth = 10;
byte varBW = 7;
byte varNum = 2;
byte varTravelType = 1;

static uint8_t hue = 0;

CRGBArray<NUM_LEDS> leds;

void fadeStrip() {
  leds.nscale8(250);
  leds = CHSV(hue, 255, 255);
  LEDS.show();
}

int red = 0;
int green = 0;
int blue = 0;


#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

ESP8266WebServer server(80);

void setup() {

  Serial.begin(112500);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(255);
  
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  //wifiManager.resetSettings();
  if (!wifiManager.autoConnect("AutoConnectAP", "")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("Connected!");
  Serial.println("Local IP");
  Serial.println(WiFi.localIP());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  server.begin();
  server.on("/", fileindex);
  server.on("/index.html", fileindex);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("bootstrap.min.css", bootstrap);
  server.on("/bootstrap-cp.min.css", colorPicker);
  server.on("bootstrap-cp.min.css", colorPicker);
  server.on("/main.css", maincss);
  server.on("main.css", maincss);
  server.on("/bootstrap.min.js", bootstrapjs);
  server.on("bootstrap.min.js", bootstrapjs);
  server.on("/bootstrap-cp.min.js", colorPickerjs);
  server.on("bootstrap-cp.min.js", colorPickerjs);
  server.on("/jquery-3.4.1.min.js", jqueryjs);
  server.on("jquery-3.4.1.min.js", jqueryjs);
  server.on("/popper.min.js", popperjs);
  server.on("popper.min.js", popperjs);

  server.on("/initParams", initParams);
  //server.on("/readParams", readParams);

  server.on("/setEnc", updateEnc);
  server.on("/setCol", updateCols);
  server.on("/setEff", setEffect);

  server.on("/setSpeed", updateSpeed);
  server.on("/setSpeedColor", updateSpeedColor);
  server.on("/setBW", updateBW);
  server.on("/setWidth", updateWidth);
  server.on("/setColor", updateColor);

  SPIFFS.begin();

}


int test = 100;
void loop() {
  EVERY_N_MILLISECONDS( test ) { hue++; }
  test--;
  server.handleClient();
  if (varOn == 1) {
    if (varEfecto == 0) {
      fadeStrip();
    }
    else if (varEfecto == 1) {
      fadeStrip();
    }
    else if (varEfecto == 2) {
      fadeStrip();
    }
    else if (varEfecto == 3) {
      fadeStrip();
    }
    else if (varEfecto == 4) {
      fadeStrip();
    }
    else if (varEfecto == 5) {
      fadeStrip();
    }
  }
  else {
    fadeStrip();
  }
}

void initParams() {
  String t_state = String(varOn) + " " + String(varCols) + " " + String(varEfecto) + " " + String(varSpeed) + " " + String(varBW) + " " + String(varWidth);
  server.send(200, "text/plane", t_state);
}

/*void readParams() { #Para actualiazar los datos mostrados sin actualizar pagina
  server.send(200, "text/plane", String(varOn));
}*/

void updateEnc() {
  String t_state = server.arg("value");
  if (t_state == "1"){
    Serial.println("Estado encendido: ON");
  }
  else {
    Serial.println("Estado encendido: OFF");
  }
  varOn = t_state.toInt();
  server.send(200, "text/plane");
}

void updateCols() {
  String t_state = server.arg("value");
  if (t_state == "1"){
    Serial.println("Columnas activadas: ON");
  }
  else {
    Serial.println("Columnas activadas: OFF");
  }
  leftColOn = t_state.toInt();
  rightColOn = t_state.toInt();
  server.send(200, "text/plane");
}

void setEffect() {
  String t_state = server.arg("value");
  Serial.print("Efecto: ");
  Serial.println(t_state);
  varEfecto = t_state.toInt();
  server.send(200, "text/plane");
}

void updateSpeed() {
  String t_state = server.arg("value");
  Serial.print("Rapidez: ");
  Serial.println(t_state);
  varSpeed = t_state.toInt();
  server.send(200, "text/plane");
}
void updateSpeedColor() {
  String t_state = server.arg("value");
  Serial.print("Rapidez colores: ");
  Serial.println(t_state);
  varSpeedColor = t_state.toInt();
  server.send(200, "text/plane");
}

void updateBW() {
  String t_state = server.arg("value");
  Serial.print("Ancho de banda: ");
  Serial.println(t_state);
  varBW = t_state.toInt();
  server.send(200, "text/plane");
}

void updateWidth() {
  String t_state = server.arg("value");
  Serial.print("Ancho: ");
  Serial.println(t_state);
  varWidth = t_state.toInt();
  server.send(200, "text/plane");
}

void updateColor() {
  varEfecto = 6;
  red = server.arg("red").toInt();
  green = server.arg("green").toInt();
  blue = server.arg("blue").toInt();
  Serial.print("Color: R=");
  Serial.print(red);
  Serial.print(" G=");
  Serial.print(green);
  Serial.print(" B=");
  Serial.println(blue);
  server.send(200, "text/plane");
}

void fileindex()
{
  File file = SPIFFS.open("/index.html", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void bootstrap()
{
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r");
  size_t sent = server.streamFile(file, "text/css");
}
void colorPicker()
{
  File file = SPIFFS.open("/bootstrap-cp.min.css.gz", "r");
  size_t sent = server.streamFile(file, "text/css");
}
void maincss()
{
  File file = SPIFFS.open("/main.css", "r");
  size_t sent = server.streamFile(file, "text/css");
}
void bootstrapjs()
{
  File file = SPIFFS.open("/bootstrap.min.js.gz", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void colorPickerjs()
{
  File file = SPIFFS.open("/bootstrap-cp.min.js.gz", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void jqueryjs()
{
  File file = SPIFFS.open("/jquery-3.4.1.min.js.gz", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void popperjs()
{
  File file = SPIFFS.open("/popper.min.js.gz", "r");
  size_t sent = server.streamFile(file, "text/html");
}
