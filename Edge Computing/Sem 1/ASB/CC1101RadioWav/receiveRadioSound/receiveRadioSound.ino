#include <RadioLib.h>
#include <pico/stdlib.h>
#include "hardware/pwm.h"

#define PACKET_SIZE 62                              // Packet size. This is maximum for C1101
#define MAXIMUM_SKIPS 2                             // Because this microcontroller has two jobs: receiving data and sending PWM signal, it might not keep up
                                                    // So this value tells how many times its allowed to skip received data and not send the PWM signal
#define DEBUG_SERIAL_PRINTS false

// CC1101 has the following connections:
// CS pin:    17
// GDO0 pin:  21
// RST pin:   unused
// GDO2 pin:  22
CC1101 radio = new Module(17, 21, RADIOLIB_NC, 22);

unsigned char sound[PACKET_SIZE]{0};
unsigned char soundReceived[PACKET_SIZE]{0};

const int     sampleRate  = 2500;                   // Sample rate in Hz
const long    sampleDelay = 1000000 / sampleRate;   // Delay between samples in microseconds

unsigned long timePoint   = 0;                      // Time when next PWM signals should be sent
unsigned int  ind         = 0;                      // Index of currently played (sound via PWM) packet's data 

void setSettings(CC1101 *radio)
{
  //Since packets failing CRC are rejected, we don't want this behaviour
  int state = radio->setCrcFiltering(false);
  if (state == RADIOLIB_ERR_NONE) 
  {
    if (DEBUG_SERIAL_PRINTS)
      Serial.println(F("success!"));
  } 
  else 
  {
    if (DEBUG_SERIAL_PRINTS)
    {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
    while (true);
  }

  state = radio->setRxBandwidth(325.0);
  if (state != RADIOLIB_ERR_NONE)
  {
    if (DEBUG_SERIAL_PRINTS)
    {
      Serial.print(F("Setting RX bandwidth failed, code "));
      Serial.println(state);
    }
    while (true);
  }

  state = radio->setFrequencyDeviation(45.0);
  if (state != RADIOLIB_ERR_NONE) 
  {
    if (DEBUG_SERIAL_PRINTS)
    {
      Serial.print(F("Setting frequency deviation failed, code "));
      Serial.println(state);
    }
    while (true);
  }

  state = radio->setBitRate(40.0);
  if (state != RADIOLIB_ERR_NONE) 
  {
    if (DEBUG_SERIAL_PRINTS)
    {
      Serial.print(F("Setting bit rate failed, code "));
      Serial.println(state);
    }
    while (true);
  }
}

void setup()
{
  gpio_set_function(0, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(0);
  pwm_set_wrap(slice_num, 255);
  pwm_set_clkdiv(slice_num, 5);              // Set clock divisor to achieve desired PWM frequency
  pwm_set_enabled(slice_num, true);
  
  if (DEBUG_SERIAL_PRINTS)
  {
    Serial.begin(921600);
    
    while(!Serial)
      delay(10);
  }

  if (DEBUG_SERIAL_PRINTS)
    Serial.print(F("[CC1101] Initializing ... "));
  
  int state = radio.begin(915.0);
  if (state == RADIOLIB_ERR_NONE)
  {
    if (DEBUG_SERIAL_PRINTS)
      Serial.println(F("success!"));
  } 
  else 
  {
    if (DEBUG_SERIAL_PRINTS)
    {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
    while (true);
  }
  
  setSettings(&radio);

  radio.startReceive();

  timePoint = micros();
}

void loop() 
{
  unsigned long passed = micros();
  if (passed > timePoint)
  {
    timePoint += sampleDelay;
    pwm_set_gpio_level(0, sound[ind++]);

    for (int x = 0; x != MAXIMUM_SKIPS; ++x)
      if (passed > timePoint)
      {
        timePoint += sampleDelay;
        ++ind;
        if (DEBUG_SERIAL_PRINTS)
          Serial.println("Skipped");
      }

    if (ind >= PACKET_SIZE)
    {
      if (DEBUG_SERIAL_PRINTS)
        Serial.println(F("reset"));

      ind -= PACKET_SIZE;
      memcpy(sound, soundReceived, PACKET_SIZE); // Copy received data
      
      int receivedSize = min(radio.getPacketLength(true), PACKET_SIZE);
      radio.readData(soundReceived, receivedSize);
      
      // Making sure our clock is still synchronized with sender's clock
      constexpr int syncInterval = sampleRate * 30; // Every 30 seconds
      static    int sync         = syncInterval - 1;
      if (++sync == syncInterval)
      {
        timePoint = micros() + sampleDelay;
        sync = 0;
      }
      
      // Fill lacking data with last received byte (setting it to 0 would make a loud sound)
      if (receivedSize < PACKET_SIZE)
      {
        Serial.print(receivedSize);
        memset(soundReceived + receivedSize - 1 , soundReceived[receivedSize - 1], PACKET_SIZE - receivedSize);
      }
      radio.startReceive();
    }
  }
}
