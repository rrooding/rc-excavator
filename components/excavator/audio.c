#include "driver/i2s.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "audio_internal.h"
#include "engine_start.h"

#define TAG "EX_AUDIO"

#define SAMPLE_RATE 44100

Audio *AudioCreate() {
  Audio *newAudio = malloc(sizeof(Audio));

  AudioInitialize(newAudio);

  return newAudio;
}

void AudioFree(Audio *this) {
  i2s_driver_uninstall(this->i2s_num);
  free(this);
}

void AudioInitialize(Audio *this) {
  ESP_LOGI(TAG, "Configuring I2S audio");

  this->i2s_num = 0;
  this->index = 0;

  i2s_config_t i2s_config = {
    .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = 16,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S, // I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = false
  };

  ESP_ERROR_CHECK(i2s_driver_install(this->i2s_num, &i2s_config, 0, NULL));

  ESP_ERROR_CHECK(i2s_set_pin(this->i2s_num, NULL));

  ESP_ERROR_CHECK(i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN));
}

void AudioPlay(Audio *this) {
  size_t i2s_bytes_write;
  for(;;) {
    i2s_write(this->i2s_num, engine_start_wav + 44 + this->index, 4, &i2s_bytes_write, portMAX_DELAY);
    this->index += 4;
    if(this->index >= engine_start_wav_len) {
      this->index = 0;
      return;
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}
