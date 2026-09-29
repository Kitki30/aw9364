#pragma once
#include "soc/gpio_num.h"
#include <cstddef>
#include <cstdio>

#define AW9364_PULSE 2 // 0,5us minimum recomended, use little bit more for safety

class AW9364 {
    public:
        // Inits gpio for AW9364, returns true for success, false for fail
        bool init(gpio_num_t onewire_pin);

        // Deinits RMT channel and encoder
        void deinit();

        // Set brightness level (1 - Brightest, 16 - Darkest, 0 - Off, refer to AW9364 docs), return true on success
        bool set_level(uint8_t brightness_level);

        // Get current brightness level (will return 0 if not init, this value is cached in ESP32's RAM, if something else than this driver changes it, this will have old value)
        uint8_t get_level();
    private:
        bool _is_init = false;
        gpio_num_t _gpio_pin;
        uint8_t _brightness_level = 0;
};