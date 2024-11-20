#include <SoftwareSerial.h>

#include <TinyGPS.h>
/*Ejemplo del uso de GPS en arduino, utilizando la Librerias TinyGPS*/

TinyGPS gps;
SoftwareSerial ss(4, 3);

void setup(){
    Serial.begin(115200);
    ss.begin(4800);

    Serial.print(TinyGPS::library_version());
    Serial.println("por: Mikal Hart");
    Serial.println();
}

void loop(){
    bool datoNuevo = false;
    unsigned long caracteres;
    unsigned short oracion, fallo;void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


    for(unsigned long start = millis(); millis() - start < 1000;){
        while(ss.available()){
            char c = ss.read();
            if(gps.encode(c)) datoNuevo = true;
        }
    }

    if(datoNuevo){
        float f_lat, f_lon;
        unsigned long age;

        gps.f_get_position(&f_lat, &f_lon, &age);
        Serial.print("LAT=");
        Serial.print(f_lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : f_lat, 6); 
        Serial.print(" LON=");
        Serial.print(f_lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : f_lon, 6);
        Serial.print(" SAT=");
        Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
        Serial.print(" PREC=");
        Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());        
    }

    gps.stats(&caracteres, &oracion, &fallo);
    Serial.print(" Caracteres=");
    Serial.print(caracteres);
    Serial.print(" Oracion=");
    Serial.print(oracion);
    Serial.print(" CSUM ERR=");
    Serial.print(fallo);
    if(caracteres == 0) Serial.println("**Ningun caracter recibido por GPS: verificar cableado ");
}