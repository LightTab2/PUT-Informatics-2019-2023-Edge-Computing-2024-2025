#include <OneWire.h>
#include <DS18B20.h>
// Pin termometru
#define ONEWIRE_PIN 2
// Adres termometru
byte address[8] = {0x28, 0x12, 0xB, 0x79, 0x97, 0x11, 0x3, 0xF8}; // Kod
//Utworzenie obiektu termometru
OneWire onewire(ONEWIRE_PIN);
DS18B20 termometr(&onewire);
void setup() {
 while(!Serial);
 Serial.begin(9600);

 termometr.begin(); // Inicializacja termometru
 termometr.request(address);
}
void loop() {
 float pomiary[16];
 int min_pos = 0;
 int max_pos = 0;
 for(int i = 0 ; i < 16 ; i++) // pobor 16 pomiarow
 {
 while(!termometr.available());
 if (termometr.available())
 {
 float temperature = termometr.readTemperature(address);
 pomiary[i] = temperature;
 termometr.request(address);
 //poszukiwanie najnizszej wartosci
 if(pomiary[i]<pomiary[min_pos])
 min_pos=i;
 //poszukiwanie maksymalnej wartosci
 if(pomiary[i]>pomiary[min_pos])
 max_pos=i;
 }
 }
 float temp_sum = 0;
 // zsumowanie pomiarow pomijajac najnizsza oraz najwyzsza wartosc
 for(int i = 0 ; i < 16; i++)
 {
 if(i != min_pos || i != min_pos)
 temp_sum=temp_sum+pomiary[i];
 }
 float wynik = temp_sum/14; //srednia wybranych pomiarow
 Serial.println(wynik);
}
