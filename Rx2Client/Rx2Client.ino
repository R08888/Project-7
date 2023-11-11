#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;
int led = 13;
unsigned long int millisBefore = 0;

int turn = 1;
int SNR1dump = 0, SNR2dump = 0;

String dataTotal;
String dataFromClient1;
String dataFromClient2;
String dataFromClient3;
String RSSIclient1, RSSIclient2;

void sendRequest(String request) {
  String dataTotal = request;
  int dataLength = dataTotal.length();
  dataLength++;
  uint8_t total[dataLength];
  dataTotal.toCharArray(total, dataLength);
  rf95.send(total, dataLength);
  // Serial.print(",");
  // Serial.println(rf95.lastRssi(), DEC);
  rf95.waitPacketSent();
}

void waitForAnsware() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.waitAvailableTimeout(500)) {
    if (rf95.recv(buf, &len)) {
      // Serial.print("Received at Server: ");
      String receivedData = (char*)buf;
      // Serial.println(receivedData);

      if (receivedData.startsWith("sensor1")) {
        // Data dari Client 1
        dataFromClient1 = receivedData;  
        RSSIclient1 = rf95.lastRssi();
        turn = 2;
        // Serial.println("Data from Client 1: " + dataFromClient1);
      } else if (receivedData.startsWith("sensor7")) {
        // Data dari Client 2
        dataFromClient2 = receivedData;
        RSSIclient2 = rf95.lastRssi();
        turn = 1;
        // Serial.println("Data from Client 2: " + dataFromClient2);
      }
    } else {
      Serial.println("recv failed");
    }
  }
}

void data() {

  Serial.println( dataFromClient1 + ",sensor5:" + RSSIclient1 + ",sensor6:" + SNR1dump + "," + dataFromClient2 + ",sensor11:" + RSSIclient2 + ",sensor12:" + SNR2dump + ",DATAEND");
  // Serial.println("Data from Client 1: " + dataFromClient1 + "RSSI" + RSSIclient1);
  // Serial.println("Data from Client 2: " + dataFromClient2 + "RSSI" + RSSIclient2);
  SNR1dump++, SNR2dump++;
  delay(20000);
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(915.0);
}

void loop() {

  switch (turn) {
    case 1:
      // Serial.println("Send Request to Client 1");
      sendRequest("C1");  // Kirim permintaan ke Client 1
      waitForAnsware();
      delay(500);

    case 2:
      // Serial.println("Send Request to Client 2");
      sendRequest("C2");
      waitForAnsware();
      // turn = 1;
      delay(500);
  }
  data();
}
