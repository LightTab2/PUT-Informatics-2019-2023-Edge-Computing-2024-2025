#include <RadioLib.h>

#define PACKET_SIZE 62                              // Packet size. This is maximum for C1101
#define DEBUG_SERIAL_PRINTS false

constexpr int BUFFER_SIZE = 1024 - (1024 % PACKET_SIZE);
// CC1101 has the following connections:
// CS pin:    17
// GDO0 pin:  21
// RST pin:   unused
// GDO2 pin:  22
CC1101 radio = new Module(17, 21, RADIOLIB_NC, 22);

const int  sampleRate      = 2500;                  // Sample rate in Hz
const int  sampleDelay     = 1000000 / sampleRate;  // Delay between samples in microseconds
const long sampleDelayWait = 62 * sampleDelay;

unsigned char buffer[BUFFER_SIZE];                  // Buffer array to store incoming data
unsigned int  bufferIndex = 0;                      // Index to keep track of buffer position to send

//We need to send data in intervals related to sampleRate, these variables keep track of time:
unsigned long tick = 0;   
unsigned long startMicros = 0;

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
  Serial.begin(921600);
  while (!Serial)
    delay(10);

  if (DEBUG_SERIAL_PRINTS)
    Serial.print(F("[CC1101] Initializing ... "));
  
  int state = radio.begin(915.0);
  setSettings(&radio);
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

  // set node address
  // NOTE: Calling this method will automatically enable
  //       address filtering. CC1101 also allows to set
  //       number of broadcast address (0/1/2).
  //       The following sets one broadcast address 0x00.
  //       When setting two broadcast addresses, 0x00 and
  //       0xFF will be used.
  /*
  if (state == RADIOLIB_ERR_NONE) 
    Serial.print(F("[CC1101] Setting node address ... "));
  
  state = radio.setNodeAddress(0x01, 1);
  if (state == RADIOLIB_ERR_NONE) 
  {
    if (state == RADIOLIB_ERR_NONE)
      Serial.println(F("success!"));
  }
  else
  {
    if (state == RADIOLIB_ERR_NONE)
    {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
    while (true);
  }
  */
  startMicros = micros();
}

void loop()
{
  bool data = false;
  byte byteArr[PACKET_SIZE];

  if ((micros() - startMicros) > tick * sampleDelayWait)
  {
    if (bufferIndex >= BUFFER_SIZE)
    {
      if (Serial.available()) 
      {
        Serial.readBytes(buffer, BUFFER_SIZE);
        data = true;
      }
      bufferIndex = 0;
    }
    if (data)
      memcpy(byteArr, buffer + bufferIndex, PACKET_SIZE);
    else
      memset(byteArr, 0, PACKET_SIZE);

    //Debug: sending from memory
    //audio data is defined in a separate file
    //memcpy(byteArr, audio_data + (tick * 62) % audio_length, 62);

    bufferIndex += PACKET_SIZE;
    radio.transmit(byteArr, PACKET_SIZE);
    if (DEBUG_SERIAL_PRINTS)
      Serial.println(F("success!"));
    ++tick;
  }
}
