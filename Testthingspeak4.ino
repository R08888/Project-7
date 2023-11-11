#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"

#define serial_komunikasi Serial1

char ssid[] = "TexasKost";
char pass[] = "wisnuudahkerja";
int keyIndex = 0;
WiFiClient client;

unsigned long myChannelNumber1 = 2258170;
const char* myWriteAPIKey1 = "XL21UA8DTMVUXJLR";

unsigned long myChannelNumber2 = 2336127;
const char* myWriteAPIKey2 = "TKAHD6KL66OGR8NQ";

String myStatus = "";

String value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11, value12, data;
int turn = 1;

void parseData(String data) {
  int sensor1StartIndex = data.indexOf("sensor1:") + 8;
  int sensor1EndIndex = data.indexOf(",sensor2");
  value1 = data.substring(sensor1StartIndex, sensor1EndIndex);

  int sensor2StartIndex = data.indexOf("sensor2:") + 8;
  int sensor2EndIndex = data.indexOf(",sensor3");
  value2 = data.substring(sensor2StartIndex, sensor2EndIndex);

  int sensor3StartIndex = data.indexOf("sensor3:") + 8;
  int sensor3EndIndex = data.indexOf(",sensor4");
  value3 = data.substring(sensor3StartIndex, sensor3EndIndex);

  int sensor4StartIndex = data.indexOf("sensor4:") + 8;
  int sensor4EndIndex = data.indexOf(",sensor5");
  value4 = data.substring(sensor4StartIndex, sensor4EndIndex);

  int sensor5StartIndex = data.indexOf("sensor5:") + 8;
  int sensor5EndIndex = data.indexOf(",sensor6");
  value5 = data.substring(sensor5StartIndex, sensor5EndIndex);

  int sensor6StartIndex = data.indexOf("sensor6:") + 8;
  int sensor6EndIndex = data.indexOf(",sensor7");
  value6 = data.substring(sensor6StartIndex, sensor6EndIndex);
  //=====================================================================
  int sensor7StartIndex = data.indexOf("sensor7:") + 8;
  int sensor7EndIndex = data.indexOf(",sensor8");
  value7 = data.substring(sensor7StartIndex, sensor7EndIndex);

  int sensor8StartIndex = data.indexOf("sensor8:") + 8;
  int sensor8EndIndex = data.indexOf(",sensor9");
  value8 = data.substring(sensor8StartIndex, sensor8EndIndex);

  int sensor9StartIndex = data.indexOf("sensor9:") + 8;
  int sensor9EndIndex = data.indexOf(",sensor10");
  value9 = data.substring(sensor9StartIndex, sensor9EndIndex);

  int sensor10StartIndex = data.indexOf("sensor10:") + 9;
  int sensor10EndIndex = data.indexOf(",sensor11");
  value10 = data.substring(sensor10StartIndex, sensor10EndIndex);

  int sensor11StartIndex = data.indexOf("sensor11:") + 9;
  int sensor11EndIndex = data.indexOf(",sensor12");
  value11 = data.substring(sensor11StartIndex, sensor11EndIndex);

  int sensor12StartIndex = data.indexOf("sensor12:") + 9;
  int sensor12EndIndex = data.indexOf(",DATAEND");
  value12 = data.substring(sensor12StartIndex, sensor12EndIndex);
}

void setup() {
  Serial.begin(115200);
  serial_komunikasi.begin(9600, SERIAL_8N1, 27, 26);

  while (!Serial) {
    ;
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  data = Serial1.readString();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }

    Serial.println("\nConnected.");
  }


  if (data.length() > 0) {
    parseData(data);
    // Serial.print(data);

    if (turn == 1) {
      ThingSpeak.setField(1, value1);
      ThingSpeak.setField(2, value2);
      ThingSpeak.setField(3, value3);
      ThingSpeak.setField(4, value4);
      ThingSpeak.setField(5, value5);
      ThingSpeak.setField(6, value6);

      int x1 = ThingSpeak.writeFields(myChannelNumber1, myWriteAPIKey1);

      if (x1 == 200) {
        Serial.println("Channel 1 update successful.");
      } else if (x1 != 200) {
        Serial.println("Problem updating channel. HTTP error code " + String(x1));
      }
      turn = 2;
      delay(1000);
    }
    if (turn == 2) {

      ThingSpeak.setField(1, value7);
      ThingSpeak.setField(2, value8);
      ThingSpeak.setField(3, value9);
      ThingSpeak.setField(4, value10);
      ThingSpeak.setField(5, value11);
      ThingSpeak.setField(6, value12);

      int x2 = ThingSpeak.writeFields(myChannelNumber2, myWriteAPIKey2);

      if (x2 == 200) {
        Serial.println("Channel 2 update successful.");
      } else if (x2 != 200) {
        Serial.println("Problem updating channel. HTTP error code " + String(x2));
      }
      turn = 1;
            delay(1000);
    }
    // ThingSpeak.setStatus(myStatus);

    Serial.print("sensor1:");
    Serial.println(value1);
    Serial.print("sensor2:");
    Serial.println(value2);
    Serial.print("sensor3:");
    Serial.println(value3);
    Serial.print("sensor4:");
    Serial.println(value4);
    Serial.print("sensor5:");
    Serial.println(value5);
    Serial.print("sensor6:");
    Serial.println(value6);

    Serial.print("sensor7:");
    Serial.println(value7);
    Serial.print("sensor8:");
    Serial.println(value8);
    Serial.print("sensor9:");
    Serial.println(value9);
    Serial.print("sensor10:");
    Serial.println(value10);
    Serial.print("sensor11:");
    Serial.println(value11);
    Serial.print("sensor12:");
    Serial.println(value12);
  }
  delay(20000);
}
