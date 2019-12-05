#include <DHT.h>
#include <UTFTGLUE.h>              //use GLUE class and constructor
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen_kbv.h>
#include <IRremote.h>

MCUFRIEND_kbv tft;
int XP = 6, YP = A1, XM = A2, YM = 7;  //most common configuration
#define DHTPIN 31     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define PORTRAIT  0
#define LANDSCAPE

#define WHITE 0xFFFF
#define RED   0xF800
#define GRAY  0x8410
#define BLACK 0x0000

#define TOUCH_ORIENTATION  LANDSCAPE
DHT dht(DHTPIN, DHTTYPE);
const int RECV_PIN = 53;
IRrecv irrecv(RECV_PIN);
decode_results results;
bool requst = false;

TouchScreen_kbv ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint_kbv tp;                            //global point

int SERIALKEY;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  dht.begin();
  tft.begin(tft.readID());
  tft.setRotation(1);
  tft.fillScreen(WHITE);
  delay(2000);
  printMain();
  
}

void loop() {
  readRequest();
  //printIR();
  delay(1000);
}

void printMain()
{
  tft.setCursor(25, 25);
  tft.setTextColor(BLACK, WHITE);
  tft.setTextSize(2);
  tft.println("Time:");
  getTime();

  tft.setTextSize(2);
  tft.setCursor(25, 150);
  tft.print("Indoor Information:");
  getTemp();

  //  dp.setColor(145, 255, 215);
  tft.fillRoundRect(300, 35, 140, 100, 6, 0x97FA);
  //
  tft.setCursor(315, 80);
  tft.print("Weather(1)");

  tft.fillRoundRect(300, 150, 140, 100, 6, 0x97FA);
  tft.setCursor(315, 190);
  tft.print("BETA TEST");
}

void getTemp()
{
  float temp_hum_val[2] = {0};
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    tft.setCursor(25, 175);
    tft.setTextSize(2);

    tft.print("Humidity: ");
    String s1 = (String)temp_hum_val[0];
    tft.setCursor(25, 195);
    tft.print(s1);
    tft.setCursor(100, 195);
    tft.print("%");

    tft.setCursor(25, 225);
    tft.print("Temperature");
    String s2 = (String)temp_hum_val[1];
    tft.setCursor(25, 245);
    tft.print(s2);
    tft.setCursor(100, 245);
    tft.print("*C");
  }
}

void getTime()
{
  SERIALKEY = 1010;
  Serial.println(SERIALKEY);
  if (Serial.available())
  {
    tft.setCursor(25, 50);
    tft.print(Serial.readString());
  }
}

void readRequest()
{
  getTime();
  getTemp();
  if (irrecv.decode(&results))
  {
    String request_code = String(results.value, HEX);
    //Serial.println(request_code);
    if (request_code.equalsIgnoreCase("ff30cf"))
    {
      SERIALKEY = 2000;
      Serial.println(SERIALKEY);
      Serial.flush();
      if (Serial.available())
      {
        tft.fillScreen(WHITE);
        tft.setTextColor(BLACK, WHITE);
        tft.setTextSize(2);
        tft.setCursor(25, 50);
        tft.print(Serial.readString());
        delay(20000);
      }
      //        if (irrecv.decode(&results))
      //        {
      //          request_code = String(results.value, HEX);
      //          if (request_code == "FF6897")
      //            break;
      //        }
    }
    tft.fillScreen(WHITE);
    tft.setTextColor(BLACK, WHITE);
    printMain();
  }
  irrecv.resume();
}

void printIR()
{
  if (irrecv.decode(&results))
  {
    String request_code = String(results.value, HEX);
    Serial.println(request_code);
  }
  irrecv.resume();
}
