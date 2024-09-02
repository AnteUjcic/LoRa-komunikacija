#include <SPI.h>
#include <Wire.h>
#include <LoRa.h>
#include <math.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define LORA_BAND    433

#define MOSI    27   
#define SCK     5    
#define SS      18   
#define DI0     26   
#define RST     23   
#define MISO    19   

double roll, pitch, yaw;
Adafruit_MPU6050 mpu;
const int x_out = 2; 
const int y_out = 13; 
const int z_out = 14; 

void setup() {
  Serial.begin(115200);


while (!Serial)
    delay(10); 

  Serial.println("Adafruit MPU6050 test!");

  // Nesto san razbi
/*  if (!mpu.begin()) {
    Serial.println("No MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  */
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
////////////
  Serial.println("");
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(2000);


  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(LORA_BAND * 1E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("init ok");
}

void loop() {
  static int counter = 0;
int x_adc_value, y_adc_value, z_adc_value; 
  double x_g_value, y_g_value, z_g_value;

  x_adc_value = analogRead(x_out); 
  y_adc_value = analogRead(y_out); 
  z_adc_value = analogRead(z_out); 
 
  x_g_value = ( ( ( (double)(x_adc_value * 3.3)/4096) - 1.65 ) / 0.330 ); 
  y_g_value = ( ( ( (double)(y_adc_value * 3.3)/4096) - 1.65 ) / 0.330 ); 
  z_g_value = ( ( ( (double)(z_adc_value * 3.3)/4096) - 1.80 ) / 0.330 );
  roll = ( ( (atan2(y_g_value,z_g_value) * 180) / 3.14 ) + 180 ); 
  pitch = ( ( (atan2(z_g_value,x_g_value) * 180) / 3.14 ) + 180 ); 
  yaw = ( ( (atan2(x_g_value,y_g_value) * 180) / 3.14 ) + 180 ); 
Serial.println(yaw);
Serial.println(pitch);
Serial.println(roll);


LoRa.beginPacket();
LoRa.print(yaw);
LoRa.print(pitch);
LoRa.print(roll);
LoRa.print(",");
LoRa.endPacket();


  String countStr = String(counter, DEC);
  Serial.println(countStr);

  counter++;
  delay(10);
}
