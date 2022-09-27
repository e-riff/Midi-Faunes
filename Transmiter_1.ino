#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

typedef struct MouvData MouvData;
struct MouvData {
  int16_t ax;
  int16_t ay;
  int16_t az;
  int16_t gx;
  int16_t gy;
  int16_t gz;
};

MouvData test;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
MPU6050 accelgyro;

void setup()
{
//  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  Wire.begin();  //I2C bus
  accelgyro.initialize();
  
  delay(2000);
}


void loop()
{
  
  accelgyro.getMotion6(&test.ax, &test.ay, &test.az, &test.gx, &test.gy, &test.gz);
  radio.write(&test, sizeof(MouvData));
//  Serial.println(test.ax);
  delay(10);
  
}
