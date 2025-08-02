#include <Wire.h>
#include <vl53l8cx.h>
#include "esp_camera.h"
#include "Arduino.h"
#include "SD.h"
#include "SPI.h"
#include "driver/rtc_io.h"
#include "camera_pins.h"

#define VSPI_MISO   40 
#define VSPI_MOSI   38 
#define VSPI_SCLK   39
#define VSPI_SS     41 
#define SD_ENABLE   48

int pictureNumber = 0;
camera_config_t config;

VL53L8CX sensor_vl53l8cx(&Wire, -1, -1);

void camera_init(){
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;

  config.pixel_format = PIXFORMAT_GRAYSCALE;  
  config.frame_size = FRAMESIZE_96X96; 
  config.jpeg_quality = 10;
  config.fb_count = 2;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

  esp_err_t err = esp_camera_init(&config);
  if(err != ESP_OK){
    Serial.println("Kamera Initialisierung ist Fehlgeschlagen");
    Serial.println(err);
    return;
  }
}

void sd_card_init(){
  pinMode(SD_ENABLE,OUTPUT);
  digitalWrite(SD_ENABLE, LOW);
  
  SPIClass spi = SPIClass();
  spi.begin(VSPI_SCLK,VSPI_MISO,VSPI_MOSI,VSPI_SS);
  if(!SD.begin(VSPI_SS, spi)){
      Serial.println("SD Karten Initialisierung ist Fehlgeschlagen");
      return;
  }

  uint8_t cardType =SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("Keine SD Karte da");
    return;
  }
}

File camera_take_and_save_picture(){
  /* durch das Problemen die ich in Aufgabe eins beschrieben habe (README.me taskOne)
    simuliere ich hier an der stelle ein Foto
  */
  String mockPath = "/mock_picture_" + String(millis()) + ".jpeg";
  File mockFile = SD.open(mockPath.c_str(), FILE_WRITE);
  
  if(mockFile) {
      mockFile.println("Mock image data");
      mockFile.close();
  }
  
  return mockFile;
}

void config_all(){
  Serial.println("Kamera Initialisieren");
  camera_init();
  delay(3);

  Serial.println("SD Karte Initialisieren");
  sd_card_init();
  delay(3);
}


void get_closest_distance() {
  VL53L8CX_ResultsData Results;
  uint8_t NewDataReady = 0;
  uint8_t status;

  status = sensor_vl53l8cx.check_data_ready(&NewDataReady);

  if ((!status) && (NewDataReady != 0)) {
    sensor_vl53l8cx.get_ranging_data(&Results);
    float min = 100.0;
    for(int i = 0; i < VL53L8CX_RESOLUTION_8X8*VL53L8CX_NB_TARGET_PER_ZONE; i++) {
      if((&Results)->target_status[i]!=255){
        float distance = (&Results)->distance_mm[i];
        if(min > distance) {
          camera_take_and_save_picture();  
          return;
        }
      }
    }
  }
}


void setup(){
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(1000000); //Sensor has max I2C freq of 1MHz
  sensor_vl53l8cx.set_i2c_address(0x51); // need to change address first, because default address is shared with LTR329
  sensor_vl53l8cx.begin();
  sensor_vl53l8cx.init();
  sensor_vl53l8cx.set_ranging_frequency_hz(30);
  sensor_vl53l8cx.set_resolution(VL53L8CX_RESOLUTION_8X8);
  sensor_vl53l8cx.start_ranging();

  config_all();
}

void loop(){
  get_closest_distance();
  delay(10); 
}

