#include <Adafruit_MPU6050.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_BME280 bme; // I2C

TwoWire CustomI2C0(i2c0, 20, 21);

float readTemperature() {
  return truncf(bme.readTemperature() * 10) / 10;
}

float readHumidity() {
  return truncf(bme.readHumidity() * 10) / 10;
}

float readPressure() {
  return (int)(bme.readPressure() / 100.0F);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  while (!Serial) {
    delay(10); 
  }

  if (!mpu.begin(0x68, &CustomI2C0)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  CustomI2C0.begin(); // Initialize your custom I2C interface

  // Initialize the BME280 sensor with custom SDA and SCL pins (GP20 and GP21)
  if (!bme.begin(0x76, &CustomI2C0)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate angle using accelerometer data (pitch and roll)
  float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z));
  float roll = atan2(a.acceleration.y, a.acceleration.z);

  // Convert angles to degrees
  pitch = pitch * 180.0 / PI;
  roll = roll * 180.0 / PI;

  /* Print out the values */
 Serial.print("Angle Pitch:");
  Serial.print(pitch);
  Serial.print(", Roll:");
  Serial.println(roll);

  float temperatuur = readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperatuur);
  Serial.println(" *C");

  float luchtvochtigheid = readHumidity();
  Serial.print("Humidity = ");
  Serial.print(luchtvochtigheid);
  Serial.println(" %");

  int luchtdruk = readPressure();
  Serial.print("Pressure = ");
  Serial.print(luchtdruk);
  Serial.println(" hPa");

  delay(1500);
}
