#include "aw9364.hpp"
#include <cstddef>
#include <cstdint>
#include <stdlib.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h" 
#include "driver/gpio.h"

static const char* TAG = "AW9364";

// TODO: Better logging, add esp_err_t names to logs

// Init gpio for aw9364
bool AW9364::init(gpio_num_t onewire_pin) {
    // Check if not init already
    if (_is_init) {
        ESP_LOGW(TAG, "init() called on already init driver");
        return true;
    }

    // I wanted to do RMT first, but something wasnt working correctly, few us of cpu time is not too long tho
    gpio_config_t gpio_conf = {
        .pin_bit_mask = (1ULL << onewire_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&gpio_conf);
    gpio_set_level(onewire_pin, 0);

    _gpio_pin = onewire_pin;

    _brightness_level = 0;
    _is_init = true;
    return true;
}

void AW9364::deinit() {
    if (_is_init) {
        gpio_set_level(_gpio_pin, 0);
    }
    _is_init = false;
    
    gpio_config_t gpio_conf = {
        .pin_bit_mask = (1ULL << _gpio_pin),
        .mode = GPIO_MODE_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&gpio_conf);
}

// Set 
bool AW9364::set_level(uint8_t brightness_level) {
    if (!_is_init) {
        ESP_LOGW(TAG, "AW9364 is not init, please use init() before calling set_backlight()");
        return false;
    }

    if (brightness_level > 16) {
        ESP_LOGW(TAG, "Invalid brightness level value: %u, min: 0, max: 16", brightness_level);
        return false;
    }

    if (brightness_level == 0) {
        _brightness_level = 0;
        gpio_set_level(_gpio_pin, 0);
        return true;
    }

    // Code fragment from SensorLib
    int from = 16 - _brightness_level;
    int to   = 16 - brightness_level;
    int num  = (16 + to - from) % 16;
    // End fragment (Logic of the IC itself was also taken from SensorLib, the docs are weird)

    for (int i = 0; i < num; i++) {
        gpio_set_level(_gpio_pin, 0);
        esp_rom_delay_us(AW9364_PULSE); // Not efficient, but better than fking with RMT decoders
        gpio_set_level(_gpio_pin, 1);
        esp_rom_delay_us(AW9364_PULSE);
    }

    _brightness_level = brightness_level;
    return true;
}

uint8_t AW9364::get_level() {
    if (!_is_init) return 0;
    return _brightness_level;
}