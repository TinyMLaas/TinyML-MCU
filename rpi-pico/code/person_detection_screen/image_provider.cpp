/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "image_provider.h"
#include "model_settings.h"

#include "DEV_Config.h"
#include "arducam_s.h"
#include "st7735.h"

#include "pico/stdio.h"
#include "pico/stdlib.h"


struct arducam_config config;
TfLiteStatus ScreenInit() {
  stdio_init_all();
  sleep_ms(1000);
  ST7735_Init();
  ST7735_DrawImage(0, 0, 80, 160, arducam_logo);
  sleep_ms(1000);

  config.sccb            = i2c0;
  config.sccb_mode       = I2C_MODE_16_8;
  config.sensor_address  = 0x24;
  config.pin_sioc        = PIN_CAM_SIOC;
  config.pin_siod        = PIN_CAM_SIOD;
  config.pin_resetb      = PIN_CAM_RESETB;
  config.pin_xclk        = PIN_CAM_XCLK;
  config.pin_vsync       = PIN_CAM_VSYNC;
  config.pin_y2_pio_base = PIN_CAM_Y2_PIO_BASE;
  config.pio             = pio0;
  config.pio_sm          = 0;
  config.dma_channel     = 0;
  arducam_init(&config);

  ST7735_FillScreen(ST7735_BLACK);

  return kTfLiteOk;
}

TfLiteStatus GetImage(int image_width,
                      int image_height, int channels, int8_t *image_data) {
  

  
  arducam_capture_frame(&config, (uint8_t *)image_data);
  

  
  uint8_t *displayBuf = new uint8_t[96 * 96 * 2];
  uint16_t index      = 0;
  for (int x = 0; x < 96 * 96; x++) {
    uint16_t imageRGB   = ST7735_COLOR565(image_data[x], image_data[x], image_data[x]);
    displayBuf[index++] = (uint8_t)(imageRGB >> 8) & 0xFF;
    displayBuf[index++] = (uint8_t)(imageRGB)&0xFF;
  }
  ST7735_DrawImage(0, 0, 96, 96, displayBuf);
  delete[] displayBuf;

  for (int i = 0; i < image_width * image_height * channels; ++i) {
    image_data[i] = (uint8_t)image_data[i] - 128;
  }
  return kTfLiteOk;
}
