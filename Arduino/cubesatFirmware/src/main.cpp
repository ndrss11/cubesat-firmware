#include <Arduino.h>
#include <DHT.h> //Libreria DHT para usar el DHT11
#include <Adafruit_BMP085.h> //Libreria BMP085 para usar el BMP085
#include <Adafruit_MPU6050.h> //Libreria MPU6050 para usar el MPU6050
#include <Wire.h> //No busque el porque de esta libreria pero es necesaria para poder usar el MPU6050, Si funciona ni le mueva
#include <Adafruit_Sensor.h> //Libreria para poder utlizar sensore de Adafruit, probablemente hagan mas lento el codigo para la facilidad para usar cualquie sensor compensa, ademas el tiempo de lectura no estan importante asi que todo fineee
#include <TinyGPS++.h> //Libreria para poder usar el modulo de GPS
#include <SoftwareSerial.h>//S
#include <SPI.h> //LoRa
#include <LoRa.h> //LoRa


DHT dhtSensor(5, DHT11); //Creamos una clase DHT donde especificamos el pin donde conectamos el sensor y el modelo que usamos, la clase se llama dhtSensor
//Variables tipo float para almacenar la lectura del DHT
float humDHT = 0;
float tempDHT = 0;
//Creamos una funcion para leer humedad y temperatura
void dhtLectura(float &temp, float &hum);

//La comunicacion del bmp es I2C y bendita Lady ADA que configura todo en su libreria, solo debemos crear una clase sin definir nada mas
Adafruit_BMP085 bmpSensor;
//Lo que si debemos definir son las variables donde se almacenan los datos del sensor
float tempBMP = 0;
float pressBMP = 0;
float altBMP = 0;
float seaPressBMP = 0;
//Creamos una funcion para leer temperatura, presion, altitud y nivel del mar
void bmpLectura(float &temp, float &press, float &alt, float &seaPress);

//Otro sensor de Adafruit gad
Adafruit_MPU6050 mpuSensor;
//Variables del MPU
float accXBMP = 0;
float accyBMP = 0;
float acczBMP = 0;
float gyrXBMP = 0;
float gyrYBMP = 0;
float gyrZBMP = 0;
//Funcion para leer acc en x - y - z, al igual que giro en x - y- z
void mpuLectura(float &accel_x, float &accel_y, float &accel_z, float &gyro_x, float &gyro_y, float &gyro_z);

//Ahora si viene lo bueno el gps, segun tinygps++ hace todo pero que dolor de cabeza
TinyGPSPlus gps;
//Creamos un puerto serial de software para poder comunicarnos con el modulo de GPS
SoftwareSerial gpsSerial(3, 4); //Rx, Tx
//Funcion para alimentar al gps con valores
void mostrarDatosGPS();

//Variable String que guarda todas las lecturas de los sensores
String datoCompleto;    //Todo en un solo dato locura extrema

String lecturaSerial;

void setup(){
  Serial.begin(115200);
  dhtSensor.begin(); //Inicializamos el DHT
  bmpSensor.begin(); //Inicializamos el bmp
  mpuSensor.begin(); //Inicializamos el mpu
  //Configuramos el mpu 
  mpuSensor.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpuSensor.setMotionDetectionThreshold(1);
  mpuSensor.setMotionDetectionDuration(20);
  mpuSensor.setInterruptPinLatch(true);
  mpuSensor.setInterruptPinPolarity(true);
  mpuSensor.setMotionInterrupt(true);
  //Iniciamos la comunicacion serial con el gps
  Serial.print("time UTC,LAT,LON,ALT,HUMDHT,TEMPDHT,TEMPBMP,PRESSBMP,ALTBMP,SEAPRESSBMP,ACCX,ACCY,ACCZ,GYRX,GYRY,GYRZ\n");
  
  gpsSerial.begin(9600);

  LoRa.setPins(10,9,8); //SS, RST, DIO0
  Serial.print("LoRa Sender");

  if(!LoRa.begin(915E6)) Serial.println("Ha fallado LoRa");

  LoRa.setSyncWord(0xF3);

}

void loop(){
  

  
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    if (gps.encode(c)) { // Procesar cada carácter recibido
      dhtLectura(humDHT, tempDHT); //Lamamos la funcion dhtLectura para actualizar las variables globales del DHT
      bmpLectura(tempBMP, pressBMP, altBMP, seaPressBMP); //Llamamos la funcion bmpLectura para actualizar las variables globales del bmp
      mpuLectura(accXBMP, accyBMP, acczBMP, gyrXBMP, gyrYBMP, gyrZBMP); //Llamamos la funcion mpuLectura para actualizar las varibales globales del mpu
      mostrarDatosGPS();
      lecturaSerial = Serial.readString();  // Lee el mensaje enviado por la Raspberry Pi
      if(lecturaSerial.equals("cubesat")) {
        Serial.print(datoCompleto);
        LoRa.beginPacket();
        LoRa.print(datoCompleto);
        LoRa.endPacket();  
      }
    }    
  }
}

void dhtLectura(float &temp, float &hum){
  
  humDHT = dhtSensor.readHumidity();
  //Serial.print(String(hum));
  tempDHT = dhtSensor.readTemperature();
  //Serial.print(String(temp));

   if (isnan(temp) || isnan(hum)) {
    temp = 0.0;
    hum = 0.0;
    return;
  }
  return;
} 

void bmpLectura(float &temp, float &press, float &alt, float &seaPress){
  
  tempBMP = bmpSensor.readTemperature();
  //Serial.print(String(hum));
  pressBMP = bmpSensor.readPressure();
  //Serial.print(String(temp));
  altBMP = bmpSensor.readAltitude();
  seaPressBMP = bmpSensor.readSealevelPressure();

   if (isnan(tempBMP) || isnan(pressBMP) || isnan(altBMP) || isnan(seaPressBMP)) {
    tempBMP = 0.0;
    pressBMP = 0.0;
    altBMP = 0.0;
    seaPressBMP = 0.0;
    return;
  }
  return;
} 

void mpuLectura(float &accel_x, float &accel_y, float &accel_z, float &gyro_x, float &gyro_y, float &gyro_z){
  
  if(mpuSensor.getMotionInterruptStatus()){
    sensors_event_t a, g, temp;
    mpuSensor.getEvent(&a, &g, &temp);

    accel_x = a.acceleration.x; 
    accel_y = a.acceleration.y;
    accel_z = a.acceleration.z;
    gyro_x =  g.gyro.x;
    gyro_y = g.gyro.y;
    gyro_z = g.gyro.z;
  }
  return;
} 

void mostrarDatosGPS() {
/*
  if (gps.time.isUpdated()) {
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.print(gps.time.second());
    Serial.print(",");
  }
  if (gps.location.isUpdated()) {
    Serial.print(gps.location.lat(), 6); // Latitud con 6 decimales
    Serial.print(",");
    Serial.print(gps.location.lng(), 6); // Longitud con 6 decimales
    Serial.print(",");
    Serial.print(gps.altitude.meters()); // altitud con 6 decimales
    //Serial.print(", ");

  }
  
  if (gps.speed.isUpdated()) {
    Serial.print(gps.speed.kmph());
    Serial.println("");  
  }
  */
  
/*
  if (gps.time.isUpdated() && gps.location.isUpdated()) {
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.print(gps.time.second());
    Serial.print(",");
    Serial.print(gps.location.lat(), 6); // Latitud con 6 decimales
    Serial.print(",");
    Serial.print(gps.location.lng(), 6); // Longitud con 6 decimales
    Serial.print(",");
    Serial.print(gps.altitude.meters()); // altitud con 6 decimales
    Serial.print(",");
    Serial.print(String(humDHT) + "," + String(tempDHT) + "," + String(tempBMP) + "," + String(pressBMP) + "," + String(altBMP) + "," + String(seaPressBMP)
      + "," + String(accXBMP) + "," + String(accyBMP) + "," + String(acczBMP) + "," + String(gyrXBMP) + "," + String(gyrYBMP) + "," + String(gyrZBMP) + "\n");
  
  }*/
 /*
 if (gps.time.isUpdated() && gps.location.isUpdated()) {
    datoCompleto = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second())+ "," + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," + String(gps.altitude.meters()) + ","+ String(humDHT) + "," + String(tempDHT) + "," + String(tempBMP) + "," + String(pressBMP) + "," + String(altBMP) + "," + String(seaPressBMP)
      + "," + String(accXBMP) + "," + String(accyBMP) + "," + String(acczBMP) + "," + String(gyrXBMP) + "," + String(gyrYBMP) + "," + String(gyrZBMP) ;
  }*/
   datoCompleto = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second())+ "," + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," + String(gps.altitude.meters()) + ","+ String(humDHT) + "," + String(tempDHT) + "," + String(tempBMP) + "," + String(pressBMP) + "," + String(altBMP) + "," + String(seaPressBMP)
      + "," + String(accXBMP) + "," + String(accyBMP) + "," + String(acczBMP) + "," + String(gyrXBMP) + "," + String(gyrYBMP) + "," + String(gyrZBMP) ;
}