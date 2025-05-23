#include "esp_camera.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char *ssid = "Bambahs";
const char *password = "vineet1234";

AsyncWebServer server(80);

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
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
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.fb_count = 2;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  server.on("/food", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Food dispensing triggered");
    request->send(200, "text/plain", "Food Dispensed");
  });

  server.on("/water", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Water dispensing triggered");
    request->send(200, "text/plain", "Water Dispensed");
  });

  server.on("/door", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Door toggle triggered");
    request->send(200, "text/plain", "Door Toggled");
  });

  server.on("/video", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", "<html><body><img src='/mjpeg' /></body></html>");
  });

  startCameraServer();
  server.begin();

  Serial.println("Server started. Use '/video' to view live feed.");
}

void loop() {
  delay(10000);
}
