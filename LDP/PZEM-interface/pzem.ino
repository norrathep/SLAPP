#include <HardwareSerial.h>
#include <PZEM004Tv30.h>
#include <stdint.h>
#define RXD1 16
#define TXD1 17

#define RXD2 2
#define TXD2 3

#define TXD3 5

PZEM004Tv30 pzem(Serial1, RXD1, TXD1);

void setup() {
  Serial.begin(115200, SERIAL_8N1, RXD2, TXD2);

  pinMode(9,OUTPUT);

}

void pzemTest() {

  float energy = pzem.energy();
  //Serial.print("<p>");
  if(!isnan(energy)){
    int energy_int = (int) energy;
    uint8_t p = ((uint8_t) energy_int)%256;
    // simulate energy
    p = p+random(1,20);
    Serial.write(p);
    char out[3];
    sprintf(out, "%03u", p);

    //Serial.print(out);
  } else {
    uint8_t p = 22;
    Serial.write(p);
    //Serial.print("22");
  }
  //Serial.println("</p>");
}


void loop() {
  Serial.readStringUntil('\n');
  pzemTest();
  //delay(100);
}
