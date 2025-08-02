#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "driver/rtc_io.h"
#include "camera_pins.h"
#include <EEPROM.h>

#define EEPROM_SIZE 1

#define VSPI_MISO   40 
#define VSPI_MOSI   38 
#define VSPI_SCLK   39
#define VSPI_SS     41 
#define SD_ENABLE   48
SPIClass spi = SPIClass();

int pictureNumber = 0;
camera_config_t config;


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

    if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10; //10-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

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

void camera_take_and_save_picture(){
  Serial.println("Kamera wird warm gemacht");
   camera_fb_t * fb = NULL;

  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);

  Serial.println("Ein Foto soll gemacht werden");
   fb = esp_camera_fb_get();

  if(!fb){
  Serial.println("Ein Foto machen fehlgeschlagen");
  return;
  }

  String path = "/picture" + String(pictureNumber) +".jpeg";
  Serial.printf("Picture file name: %s\n", path.c_str());  

    EEPROM.begin(EEPROM_SIZE);
    pictureNumber = EEPROM.read(0) + 1;

File file = SD.open(path.c_str(), FILE_WRITE);

if(!file){
  Serial.println("Das öffnen vom Ordner ist fehlgeschlagen");
  return;
}
else {
  file.write(fb->buf, fb->len);
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
}
file.close();
esp_camera_fb_return(fb);


}

void config_all(){
  Serial.println("Kamera Initialisieren");
  camera_init();
  delay(3);

  Serial.println("SD Karte Initialisieren");
  sd_card_init()
  delay(3);
}
  
void setup() {
  Serial.begin(115200);   //Datenrate in BpS

  configAll();

  takeAPictureAndSave();
}

void loop() {
}

