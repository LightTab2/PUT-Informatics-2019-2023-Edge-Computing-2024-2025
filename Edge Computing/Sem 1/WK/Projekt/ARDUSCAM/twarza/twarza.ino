// Load model
//#include <pokerface_inferencing.h>

#define OV2640_MINI_2MP
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"
#include <string>

uint8_t read_fifo_burst(ArduCAM myCAM);

int imagion(uint8_t *in_buffer, size_t offset, size_t length, float *out_ptr)
{
    for (int i = 0; i != length; ++i)
      out_ptr[i] = reinterpret_cast<float &>(in_buffer[i + offset]);
    return 0;
}

constexpr bool bDebug = false; 
// Pin definition
constexpr int CS = 7;

constexpr int width       = 48,                                  height       = 48;
constexpr int width_half  = width >> 1,                          height_half  = height >> 1;
constexpr int width_skip  = 160 - width_half,                    height_skip  = 120 - height_half;
constexpr int width_skip2 = 160 + width_half + (width_half % 2), height_skip2 = 120 + height_half + (height_half % 2);

constexpr int size = width * height;
uint8_t* image = new uint8_t[size];

// Camera object
ArduCAM myCAM(OV2640, CS);

void setup() 
{
    Wire.begin();
    Serial.begin(921600);
    while (!Serial) 
      delay(1);

    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);

    SPI.begin();

    myCAM.write_reg(0x07, 0x80); // Reset the CPLD
    delay(100);
    myCAM.write_reg(0x07, 0x00);
    delay(1000);
    while (1) 
    {
        myCAM.write_reg(ARDUCHIP_TEST1, 0x55); // Check SPI bus
        uint8_t temp = myCAM.read_reg(ARDUCHIP_TEST1);
        if (temp != 0x55) 
        {
            Serial.println(F("ACK CMD SPI interface Error! END"));
            delay(1000);
            continue;
        } 
        else
        {
            if (bDebug)
              Serial.println(F("ACK CMD SPI interface OK. END"));
            break;
        }
    }

    uint8_t vid, pid;
    uint8_t temp;
    
    while(1)
    {
      //Check if the camera module type is OV2640
      myCAM.wrSensorReg8_8(0xff, 0x01);
      myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
      myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
      if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
          Serial.println(F("ACK CMD Can't find OV2640 module! END"));
        delay(1000);
        continue;
    }
    else
    {
      if (bDebug)
        Serial.println(F("ACK CMD OV2640 detected. END"));
      break;
    } 
  }

    myCAM.set_format(BMP);

    myCAM.InitCAM();
    myCAM.wrSensorReg8_8(0xff, 0x00);
    delayMicroseconds(15);
    myCAM.wrSensorReg8_8(0xda, 0b01000000);
    delayMicroseconds(15);
    myCAM.wrSensorReg8_8(0xff, 0x01);
    delayMicroseconds(15);
    //?? Magia
    myCAM.wrSensorReg16_8(0x3818, 0x81);
    myCAM.wrSensorReg16_8(0x3621, 0xA7);

    delay(1000);
    myCAM.clear_fifo_flag();
}

void loop() 
{
    if (Serial.available()) 
    {
        uint8_t command = Serial.read();
        if (command == '1') // Command to capture and send photo
        { 
          myCAM.flush_fifo();
          myCAM.clear_fifo_flag();
          myCAM.start_capture();
          
          while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) { delay(1); }

          read_fifo_burst(myCAM);
          myCAM.clear_fifo_flag();
        }
    }

    delay(100);
}

// Function to read FIFO and send data over serial
uint8_t read_fifo_burst(ArduCAM myCAM)
{
  unsigned long passedTimeStart = micros();
  uint8_t temp = 0, temp_last = 0;
  uint32_t length = 0;
  length = myCAM.read_fifo_length() - 4;
  //Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //512 kb
  {
    Serial.println(F("ACK CMD Over size. END"));
    return 0;
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("ACK CMD Size is 0. END"));
    return 0;
  }
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  delayMicroseconds(120);

  Serial.println(size, DEC);

  // Crop
  int array_index = -1;
  for (int y = 0; y != 240; ++y)
    for (int x = 0; x != 320; ++x)
      {
        uint8_t byte = SPI.transfer(0x00);
        if ((x < width_skip || x >= width_skip2) || (y < height_skip || y >= height_skip2))
          continue;

        image[++array_index] = byte;
        Serial.write(byte); 
      }
  myCAM.CS_HIGH();

  if (bDebug)
    Serial.println("Image saved");

  /*ei_impulse_result_t result{ 0 };
  
  signal_t sig;
  sig.total_length = size;
  sig.get_data = mbed::callback(&imagion, image);

  EI_IMPULSE_ERROR ei_error = run_classifier(&sig, &result, false);
  
  if (ei_error != EI_IMPULSE_OK) 
  {
      Serial.print("Failed to run impulse: ");
      Serial.println(ei_error);
      return 0;
  }  
  unsigned long passedTime = micros() - passedTimeStart;
  std::string str = "Predykcja modelu: ";
  str += result.classification[0].label;
  str += "(";
  str += std::to_string(result.classification[0].value);
  str += ")\t";
  str += result.classification[1].label;
  str += "(";
  str += std::to_string(result.classification[1].value);
  str += ")";
  str += "\nCzas: ";
  str += std::to_string(passedTime);

  Serial.println(str.c_str());
*/
  return 1;
}
